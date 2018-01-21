#include "SubView.hpp"
#include "common.hpp"
#include "object/CameraObject.hpp"
#include <OgreCamera.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneNode.h>
#include <OgreViewport.h>

SubView::SubView(QWindow *parent) : OgreWindow(parent) {

  Ogre::String sceneTypeName =
      Ogre::DefaultSceneManagerFactory::FACTORY_TYPE_NAME;
  Ogre::String sceneName = "Scene001";
  Ogre::Root *root = Ogre::Root::getSingletonPtr();
  Ogre::SceneManager *sceneManager = nullptr;

  if (root->hasSceneManager(sceneName)) {
    sceneManager = root->getSceneManager(sceneName);
  } else {
    sceneManager = root->createSceneManager(sceneTypeName, sceneName);
  }

  Ogre::SceneNode *rootNode = sceneManager->getRootSceneNode();

  unsigned int mask = 0;
  mask |= ObjectType_Model;

  // Camera 1
  Ogre::Camera *camera1 = sceneManager->createCamera("CameraView1");
  camera1->setNearClipDistance(0.1f);
  camera1->setFarClipDistance(10.0f);
  camera1->setAspectRatio(Ogre::Real(640.0 / 480.0));
  camera1->setFOVy(Ogre::Degree(30));

  Ogre::Viewport *viewPort1 =
      mRenderWindow->addViewport(camera1, 0, 0, 0, 0.5, 0.5);
  viewPort1->setBackgroundColour(Ogre::ColourValue::Black);
  viewPort1->setVisibilityMask(mask);

  CameraObject *cameraObject1 =
      reinterpret_cast<CameraObject *>(sceneManager->createMovableObject(
          "CameraObject1", CameraObjectFactory::FACTORY_TYPE_NAME));
  cameraObject1->attachCamera(camera1);
  cameraObject1->setVisibilityFlags(ObjectType_Camera);

  Ogre::SceneNode *node1 = rootNode->createChildSceneNode("camera1");
  node1->setPosition(0, 0, 0);
  node1->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_WORLD);

  node1->attachObject(camera1);
  node1->attachObject(cameraObject1);

  // Camera 2
  Ogre::Camera *camera2 = sceneManager->createCamera("CameraView2");
  camera2->setNearClipDistance(0.1f);
  camera2->setFarClipDistance(10.0f);
  camera2->setAspectRatio(Ogre::Real(640.0 / 480.0));
  camera2->setFOVy(Ogre::Degree(40));
  camera2->setVisibilityFlags(ObjectType_Camera);

  Ogre::Viewport *viewPort2 =
      mRenderWindow->addViewport(camera2, 1, 0.5, 0, 0.5, 0.5);
  viewPort2->setBackgroundColour(Ogre::ColourValue::Black);
  viewPort2->setVisibilityMask(mask);

  CameraObject *cameraObject2 =
      reinterpret_cast<CameraObject *>(sceneManager->createMovableObject(
          "CameraObject2", CameraObjectFactory::FACTORY_TYPE_NAME));
  cameraObject2->attachCamera(camera2);
  cameraObject2->setVisibilityFlags(ObjectType_Camera);

  Ogre::SceneNode *node2 = rootNode->createChildSceneNode("camera2");
  node2->setPosition(0, 0, 0);
  node2->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_WORLD);
  node2->attachObject(camera2);
  node2->attachObject(cameraObject2);

  // Camera 3
  Ogre::Camera *camera3 = sceneManager->createCamera("CameraView3");
  camera3->setNearClipDistance(0.1f);
  camera3->setFarClipDistance(10.0f);
  camera3->setAspectRatio(Ogre::Real(640.0 / 480.0));
  camera3->setFOVy(Ogre::Degree(50));
  camera3->setVisibilityFlags(ObjectType_Camera);

  Ogre::Viewport *viewPort3 =
      mRenderWindow->addViewport(camera3, 2, 0, 0.5, 0.5, 0.5);
  viewPort3->setBackgroundColour(Ogre::ColourValue::Black);
  viewPort3->setVisibilityMask(mask);

  CameraObject *cameraObject3 =
      reinterpret_cast<CameraObject *>(sceneManager->createMovableObject(
          "CameraObject3", CameraObjectFactory::FACTORY_TYPE_NAME));
  cameraObject3->attachCamera(camera3);
  cameraObject3->setVisibilityFlags(ObjectType_Camera);

  Ogre::SceneNode *node3 = rootNode->createChildSceneNode("camera3");
  node3->setPosition(0, 0, 0);
  node3->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_WORLD);
  node3->attachObject(camera3);
  node3->attachObject(cameraObject3);

  // Camera 4
  Ogre::Camera *camera4 = sceneManager->createCamera("CameraView4");
  camera4->setNearClipDistance(0.1f);
  camera4->setFarClipDistance(10.0f);
  camera4->setAspectRatio(Ogre::Real(640.0 / 480.0));
  camera4->setFOVy(Ogre::Degree(60));
  camera4->setVisibilityFlags(ObjectType_Camera);

  Ogre::Viewport *viewPort4 =
      mRenderWindow->addViewport(camera4, 3, 0.5, 0.5, 0.5, 0.5);
  viewPort4->setBackgroundColour(Ogre::ColourValue::Black);
  viewPort4->setVisibilityMask(mask);

  CameraObject *cameraObject4 =
      reinterpret_cast<CameraObject *>(sceneManager->createMovableObject(
          "CameraObject4", CameraObjectFactory::FACTORY_TYPE_NAME));
  cameraObject4->attachCamera(camera4);
  cameraObject4->setVisibilityFlags(ObjectType_Camera);

  Ogre::SceneNode *node4 = rootNode->createChildSceneNode("camera4");
  node4->setPosition(0, 0, 0);
  node4->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_WORLD);
  node4->attachObject(camera4);
  node4->attachObject(cameraObject4);
}

SubView::~SubView() {}
