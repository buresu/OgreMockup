#pragma once

#include <OgreSceneNode.h>
#include <assimp/types.h>

class aiMesh;
class aiTexture;
class aiMaterial;
class aiCamera;
class aiLight;
class aiNode;

class AssimpSceneLoader {
public:
  explicit AssimpSceneLoader(Ogre::SceneNode *parent);
  ~AssimpSceneLoader() {}

  bool loadFile(const Ogre::String &fileName);

protected:
  Ogre::String toOgreString(const aiString &str) const;
  Ogre::Vector3 toOgreVector3(const aiVector3D &vec) const;
  Ogre::ColourValue toOgreColor(const aiColor4D &color) const;

  Ogre::String getValidFileName(const Ogre::String &fileName) const;
  Ogre::String getValidMeshName(const aiString &name) const;
  Ogre::String getValidTextureName(const aiString &name) const;
  Ogre::String getValidMaterialName(const aiString &name) const;
  Ogre::String getValidCameraName(const aiString &name) const;
  Ogre::String getValidLightName(const aiString &name) const;
  Ogre::String getValidNodeName(const aiString &name) const;
  Ogre::String getValidEntityName(const aiString &name) const;

  bool isMesh(const Ogre::String &name) const;
  bool isCamera(const Ogre::String &name) const;
  bool isLight(const Ogre::String &name) const;

  void parseMesh(const aiMesh *mesh);
  void parseTexture(const aiTexture *texture);
  void parseMaterial(const aiMaterial *material);
  void parseCamera(const aiCamera *camera, Ogre::SceneNode *node);
  void parseLight(const aiLight *light);
  void parseTransform(const aiMatrix4x4 &matrix, Ogre::SceneNode *node);
  void parseChildNode(const aiNode *node, Ogre::SceneNode *parent);

  Ogre::SceneNode *mParentNode = nullptr;

  // Instance Map
  std::map<Ogre::String, aiCamera *> mCameraMap;

  // Name Map
  std::map<Ogre::String, Ogre::String> mTextureNameMap;
  std::map<unsigned int, Ogre::String> mMaterialNameMap;
  std::map<Ogre::String, Ogre::String> mMeshNameMap;
  std::map<Ogre::String, Ogre::String> mEntityNameMap;
  std::map<Ogre::String, Ogre::String> mLightNameMap;
};
