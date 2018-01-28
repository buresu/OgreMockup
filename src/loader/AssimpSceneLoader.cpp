#include "AssimpSceneLoader.hpp"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreHardwareBufferManager.h>
#include <OgreMaterialManager.h>
#include <OgreMesh.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSubMesh.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>
#include <OgreVector3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <QDebug>

AssimpSceneLoader::AssimpSceneLoader(Ogre::SceneNode *parent)
    : mParentNode(parent) {}

bool AssimpSceneLoader::loadFile(const Ogre::String &fileName) {

  if (!mParentNode) {
    return false;
  }

  Assimp::Importer importer;
  unsigned int flags = 0;

  flags |= aiProcess_Triangulate;
  flags |= aiProcess_FlipUVs;
  flags |= aiProcess_GenUVCoords;
  flags |= aiProcess_TransformUVCoords;

  const aiScene *scene = importer.ReadFile(fileName, flags);

  if (!scene) {
    return false;
  }

  // Build Camera Map
  if (scene->HasCameras()) {
    for (unsigned int i = 0; i < scene->mNumCameras; ++i) {
      mCameraMap[toOgreString(scene->mCameras[i]->mName)] = scene->mCameras[i];
    }
  }

  // Parse Embedded Textures
  if (scene->HasTextures()) {
    for (unsigned int i = 0; i < scene->mNumTextures; ++i) {
      parseTexture(scene->mTextures[i]);
    }
  }

  // Parse Materials
  if (scene->HasMaterials()) {
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
      parseMaterial(scene->mMaterials[i]);
    }
  }

  // Parse Meshes
  if (scene->HasMeshes()) {
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
      parseMesh(scene->mMeshes[i]);
    }
  }

  // Parse Lights
  if (scene->HasLights()) {
    for (unsigned int i = 0; i < scene->mNumLights; ++i) {
      parseLight(scene->mLights[i]);
    }
  }

  // Parse Scene
  if (scene->mRootNode) {
    parseChildNode(scene->mRootNode, mParentNode);
  }

  return true;
}

Ogre::String AssimpSceneLoader::toOgreString(const aiString &str) const {
  return Ogre::String(str.C_Str(), str.length);
}

Ogre::Vector3 AssimpSceneLoader::toOgreVector3(const aiVector3D &vec) const {
  return Ogre::Vector3(vec.x, vec.y, vec.z);
}

Ogre::ColourValue AssimpSceneLoader::toOgreColor(const aiColor4D &color) const {
  return Ogre::ColourValue(color.r, color.g, color.b, color.a);
}

Ogre::String
AssimpSceneLoader::getValidFileName(const Ogre::String &fileName) const {

  if (Ogre::StringUtil::startsWith(fileName, "//")) {
    return Ogre::StringUtil::normalizeFilePath(fileName, false);
  }

  return fileName;
}

Ogre::String AssimpSceneLoader::getValidMeshName(const aiString &name) const {

  Ogre::MeshManager *meshManager = Ogre::MeshManager::getSingletonPtr();
  Ogre::String validName = toOgreString(name);

  while (meshManager->resourceExists(validName)) {
    validName += "#";
  }

  return validName;
}

Ogre::String
AssimpSceneLoader::getValidTextureName(const aiString &name) const {

  Ogre::TextureManager *textureManager =
      Ogre::TextureManager::getSingletonPtr();
  Ogre::String validName = toOgreString(name);

  while (textureManager->resourceExists(validName)) {
    validName += "#";
  }

  return validName;
}

Ogre::String
AssimpSceneLoader::getValidMaterialName(const aiString &name) const {

  Ogre::MaterialManager *materialManager =
      Ogre::MaterialManager::getSingletonPtr();
  Ogre::String validName = toOgreString(name);

  while (materialManager->resourceExists(validName)) {
    validName += "#";
  }

  return validName;
}

Ogre::String AssimpSceneLoader::getValidCameraName(const aiString &name) const {

  Ogre::SceneManager *creator = mParentNode->getCreator();
  Ogre::String validName = toOgreString(name);

  while (creator->hasCamera(validName)) {
    validName += "#";
  }

  return validName;
}

Ogre::String AssimpSceneLoader::getValidLightName(const aiString &name) const {

  Ogre::SceneManager *creator = mParentNode->getCreator();
  Ogre::String validName = toOgreString(name);

  while (creator->hasLight(validName)) {
    validName += "#";
  }

  return validName;
}

Ogre::String AssimpSceneLoader::getValidNodeName(const aiString &name) const {

  Ogre::SceneManager *creator = mParentNode->getCreator();
  Ogre::String validName = toOgreString(name);

  while (creator->hasSceneNode(validName)) {
    validName += "#";
  }

  return validName;
}

Ogre::String AssimpSceneLoader::getValidEntityName(const aiString &name) const {

  Ogre::SceneManager *creator = mParentNode->getCreator();
  Ogre::String validName = toOgreString(name);

  while (creator->hasEntity(validName)) {
    validName += "#";
  }

  return validName;
}

bool AssimpSceneLoader::isMesh(const Ogre::String &name) const {
  return mMeshNameMap.count(name) > 0;
}

bool AssimpSceneLoader::isCamera(const Ogre::String &name) const {
  return mCameraMap.count(name) > 0;
}

bool AssimpSceneLoader::isLight(const Ogre::String &name) const {
  return mLightNameMap.count(name) > 0;
}

void AssimpSceneLoader::parseMesh(const aiMesh *mesh) {

  Ogre::SceneManager *creator = mParentNode->getCreator();
  Ogre::MeshManager *meshManager = Ogre::MeshManager::getSingletonPtr();
  Ogre::HardwareBufferManager *bufferManager =
      Ogre::HardwareBufferManager::getSingletonPtr();

  // Mesh Name
  const Ogre::String name = getValidMeshName(mesh->mName);
  mMeshNameMap[toOgreString(mesh->mName)] = name;

  // Mesh
  Ogre::MeshPtr ogreMesh = meshManager->createManual(
      name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  ogreMesh->sharedVertexData = new Ogre::VertexData;
  ogreMesh->sharedVertexData->vertexCount = mesh->mNumVertices;

  // Vertex Declaration
  Ogre::VertexDeclaration *decl = ogreMesh->sharedVertexData->vertexDeclaration;
  size_t offset = 0;

  decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
  offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

  decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
  offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

  // Vertex Buffer
  Ogre::HardwareVertexBufferSharedPtr vertexBuffer =
      bufferManager->createVertexBuffer(offset,
                                        ogreMesh->sharedVertexData->vertexCount,
                                        Ogre::HardwareBuffer::HBU_STATIC);

  Ogre::Vector3 min = Ogre::Vector3::ZERO;
  Ogre::Vector3 max = Ogre::Vector3::ZERO;

  float *vertices = static_cast<float *>(
      vertexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));

  std::size_t step = offset / sizeof(float);

  for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {

    std::size_t index = i * step;

    vertices[index] = mesh->mVertices[i].x;
    vertices[index + 1] = mesh->mVertices[i].y;
    vertices[index + 2] = mesh->mVertices[i].z;

    min.x = std::min(min.x, vertices[index]);
    min.y = std::min(min.y, vertices[index + 1]);
    min.z = std::min(min.z, vertices[index + 2]);
    max.x = std::max(max.x, vertices[index]);
    max.y = std::max(max.y, vertices[index + 1]);
    max.z = std::max(max.z, vertices[index + 2]);

    if (mesh->HasTextureCoords(0)) {
      vertices[index + 3] = mesh->mTextureCoords[0][i].x;
      vertices[index + 4] = mesh->mTextureCoords[0][i].y;
    }
  }

  vertexBuffer->unlock();

  ogreMesh->sharedVertexData->vertexBufferBinding->setBinding(0, vertexBuffer);

  // Index Buffer
  Ogre::HardwareIndexBufferSharedPtr indexBuffer =
      bufferManager->createIndexBuffer(Ogre::HardwareIndexBuffer::IT_32BIT,
                                       mesh->mNumFaces * 3,
                                       Ogre::HardwareBuffer::HBU_STATIC);

  uint32_t *indices = static_cast<uint32_t *>(
      indexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));

  for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
      std::size_t index = i * 3 + j;
      indices[index] = mesh->mFaces[i].mIndices[j];
    }
  }

  indexBuffer->unlock();

  // Sub Mesh
  Ogre::SubMesh *subMesh = ogreMesh->createSubMesh();

  subMesh->useSharedVertices = true;
  subMesh->indexData->indexBuffer = indexBuffer;
  subMesh->indexData->indexCount = mesh->mNumFaces * 3;
  subMesh->indexData->indexStart = 0;

  // Bounding Box
  ogreMesh->_setBounds(Ogre::AxisAlignedBox(min, max));

  ogreMesh->load();

  // Entity Name
  const Ogre::String entityName = getValidEntityName(mesh->mName);
  mEntityNameMap[toOgreString(mesh->mName)] = entityName;

  // Entity Instance
  Ogre::Entity *entity = creator->createEntity(entityName, ogreMesh);

  entity->setMaterialName(mMaterialNameMap[mesh->mMaterialIndex]);
}

void AssimpSceneLoader::parseTexture(const aiTexture *) {
  qDebug() << "TODO:"
           << "Parse Embedded Textures";
}

void AssimpSceneLoader::parseMaterial(const aiMaterial *material) {

  Ogre::MaterialManager *materialManager =
      Ogre::MaterialManager::getSingletonPtr();
  Ogre::TextureManager *textureManager =
      Ogre::TextureManager::getSingletonPtr();

  // Name
  aiString keyName;
  if (material->Get(AI_MATKEY_NAME, keyName) != AI_SUCCESS) {
    return;
  }

  const Ogre::String name = getValidMeshName(keyName);
  mMaterialNameMap[static_cast<unsigned int>(mMaterialNameMap.size())] = name;

  // Material
  Ogre::MaterialPtr ogreMaterial = materialManager->create(
      name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  // Shading Model
  int shadingModel = 0;
  if (material->Get(AI_MATKEY_SHADING_MODEL, shadingModel) == AI_SUCCESS) {

    if (shadingModel == aiShadingMode_Flat) {
      ogreMaterial->getTechnique(0)->getPass(0)->setShadingMode(Ogre::SO_FLAT);
    } else if (shadingModel == aiShadingMode_Phong) {
      ogreMaterial->getTechnique(0)->getPass(0)->setShadingMode(Ogre::SO_PHONG);
    } else if (shadingModel == aiShadingMode_Gouraud) {
      ogreMaterial->getTechnique(0)->getPass(0)->setShadingMode(
          Ogre::SO_GOURAUD);
    } else if (shadingModel == aiShadingMode_NoShading) {
      ogreMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    }
  }

  // Diffuse
  aiColor4D keyDiffuse;
  if (material->Get(AI_MATKEY_COLOR_DIFFUSE, keyDiffuse) == AI_SUCCESS) {
    ogreMaterial->getTechnique(0)->getPass(0)->setDiffuse(
        toOgreColor(keyDiffuse));
  }

  // Ambient
  aiColor4D keyAmbient;
  if (material->Get(AI_MATKEY_COLOR_AMBIENT, keyAmbient) == AI_SUCCESS) {
    ogreMaterial->getTechnique(0)->getPass(0)->setAmbient(
        toOgreColor(keyAmbient));
  }

  // Specular
  aiColor4D keySpecular;
  if (material->Get(AI_MATKEY_COLOR_SPECULAR, keySpecular) == AI_SUCCESS) {
    ogreMaterial->getTechnique(0)->getPass(0)->setSpecular(
        toOgreColor(keySpecular));
  }

  // Emissive
  aiColor4D keyEmissive;
  if (material->Get(AI_MATKEY_COLOR_EMISSIVE, keyEmissive) == AI_SUCCESS) {
    ogreMaterial->getTechnique(0)->getPass(0)->setEmissive(
        toOgreColor(keyEmissive));
  }

  // Shininess
  float shininess = 1.0;
  if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS) {
    ogreMaterial->getTechnique(0)->getPass(0)->setShininess(shininess);
  }

  // Wireframe
  bool keyWireframe = false;
  if (material->Get(AI_MATKEY_ENABLE_WIREFRAME, keyWireframe) == AI_SUCCESS) {
    ogreMaterial->getTechnique(0)->getPass(0)->setPolygonMode(
        Ogre::PM_WIREFRAME);
  }

  // Transparent
  float keyTransparent = 1.0;
  if (material->Get(AI_MATKEY_COLOR_TRANSPARENT, keyTransparent) ==
      AI_SUCCESS) {
    ogreMaterial->getTechnique(0)->getPass(0)->setSceneBlending(
        Ogre::SBT_TRANSPARENT_ALPHA);
    Ogre::ColourValue diffuse =
        ogreMaterial->getTechnique(0)->getPass(0)->getDiffuse();
    diffuse.a = keyTransparent;
    ogreMaterial->getTechnique(0)->getPass(0)->setDiffuse(diffuse);
  }

  // Diffuse Texture
  for (unsigned int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE);
       ++i) {

    aiString texPath;
    if (material->GetTexture(aiTextureType_DIFFUSE, i, &texPath) ==
        AI_SUCCESS) {

      const Ogre::String fileName = getValidFileName(toOgreString(texPath));

      if (!textureManager->resourceExists(fileName)) {

        std::ifstream fs(fileName, std::ios::binary | std::ios::in);

        if (fs.is_open()) {

          Ogre::DataStreamPtr stream(
              new Ogre::FileStreamDataStream(fileName, &fs, false));

          Ogre::Image image;
          image.load(stream, fileName.substr(fileName.find_last_of('.') + 1));

          textureManager->loadImage(
              fileName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
              image);
        }
      }

      if (textureManager->resourceExists(fileName)) {
        ogreMaterial->getTechnique(0)->getPass(0)->createTextureUnitState(
            toOgreString(texPath));
      }
    }
  }
}

void AssimpSceneLoader::parseCamera(const aiCamera *camera,
                                    Ogre::SceneNode *node) {

  Ogre::SceneManager *creator = mParentNode->getCreator();

  // Camera Name
  const Ogre::String name = getValidCameraName(camera->mName);

  // Camera
  Ogre::Camera *ogreCamera = creator->createCamera(name);

  float aspect = 1.f;

  if (camera->mAspect > 0.f) {
    aspect = camera->mAspect;
  }

  Ogre::Radian fovY =
      2 * Ogre::Math::ATan(Ogre::Math::Tan(camera->mHorizontalFOV) / aspect);
  Ogre::Quaternion upOrientation =
      toOgreVector3(camera->mUp).getRotationTo(Ogre::Vector3::UNIT_Y);
  Ogre::Quaternion forwardOrientation =
      toOgreVector3(camera->mLookAt)
          .getRotationTo(Ogre::Vector3::NEGATIVE_UNIT_Z);

  ogreCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
  ogreCamera->setNearClipDistance(camera->mClipPlaneNear);
  ogreCamera->setFarClipDistance(camera->mClipPlaneFar);
  ogreCamera->setAspectRatio(aspect);
  ogreCamera->setFOVy(fovY);

  node->rotate(upOrientation.Inverse(), Ogre::Node::TS_WORLD);
  node->rotate(forwardOrientation.Inverse(), Ogre::Node::TS_WORLD);

  node->attachObject(ogreCamera);
}

void AssimpSceneLoader::parseLight(const aiLight *light) {

  Ogre::SceneManager *creator = mParentNode->getCreator();

  // Camera Name
  const Ogre::String name = getValidLightName(light->mName);
  mLightNameMap[toOgreString(light->mName)] = name;

  // Light
  Ogre::Light *ogreLight = creator->createLight(name);
}

void AssimpSceneLoader::parseTransform(const aiMatrix4x4 &m,
                                       Ogre::SceneNode *node) {

  Ogre::Vector3 position, scale;
  Ogre::Quaternion orientation;
  Ogre::Matrix4 matrix(m.a1, m.a2, m.a3, m.a4, m.b1, m.b2, m.b3, m.b4, m.c1,
                       m.c2, m.c3, m.c4, m.d1, m.d2, m.d3, m.d4);

  matrix.decomposition(position, scale, orientation);

  node->rotate(orientation, Ogre::Node::TS_WORLD);
  node->scale(scale);
  node->translate(position, Ogre::Node::TS_WORLD);
}

void AssimpSceneLoader::parseChildNode(const aiNode *node,
                                       Ogre::SceneNode *parent) {

  // Root Node
  if (node->mParent == nullptr) {

    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
      parseChildNode(node->mChildren[i], parent);
    }

    return;
  }

  // Creator
  Ogre::SceneManager *creator = mParentNode->getCreator();

  // Node Name
  const Ogre::String name = toOgreString(node->mName);
  const Ogre::String nodeName = getValidNodeName(node->mName);

  // Node
  Ogre::SceneNode *ogreNode = parent->createChildSceneNode(nodeName);

  if (isMesh(name)) {

    Ogre::Entity *entity = creator->getEntity(mEntityNameMap[name]);
    ogreNode->attachObject(entity);

  } else if (isCamera(name)) {

    parseCamera(mCameraMap[name], ogreNode);

  } else if (isLight(name)) {

    Ogre::Light *light = creator->getLight(mLightNameMap[name]);
    ogreNode->attachObject(light);
  }

  parseTransform(node->mTransformation, ogreNode);

  for (unsigned int i = 0; i < node->mNumChildren; ++i) {
    parseChildNode(node->mChildren[i], parent);
  }
}
