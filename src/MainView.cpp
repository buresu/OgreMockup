#include "MainView.hpp"
#include "common.hpp"
#include "controller/CameraController.hpp"
#include "gizmo/RotateGizmo.hpp"
#include "gizmo/ScaleGizmo.hpp"
#include "gizmo/TranslateGizmo.hpp"
#include "loader/AssimpSceneLoader.hpp"
#include "object/ActivePointObject.hpp"
#include "object/AxisGridLineObject.hpp"
#include "object/CameraObject.hpp"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreTechnique.h>
#include <OgreViewport.h>
#include <QDebug>

MainView::MainView(QWindow *parent) : OgreWindow(parent) {

  Ogre::String sceneTypeName =
      Ogre::DefaultSceneManagerFactory::FACTORY_TYPE_NAME;
  Ogre::String sceneName = "Scene001";
  Ogre::Root *root = Ogre::Root::getSingletonPtr();

  if (root->hasSceneManager(sceneName)) {
    mOgreSceneManager = root->getSceneManager(sceneName);
  } else {
    mOgreSceneManager = root->createSceneManager(sceneTypeName, sceneName);
  }

  Ogre::SceneNode *childSceneNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  mOgreCamera = mOgreSceneManager->createCamera("MainCamera");
  mOgreCamera->setPosition(Ogre::Vector3(0.0f, 0.0f, 5.0f));
  mOgreCamera->lookAt(Ogre::Vector3(0.0f, 0.0f, -0.0f));
  mOgreCamera->setNearClipDistance(0.1f);
  mOgreCamera->setFarClipDistance(1000.0f);
  //  mOgreCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

  childSceneNode->attachObject(mOgreCamera);

  mCameraController = new CameraController(childSceneNode);

  Ogre::Viewport *pViewPort = mRenderWindow->addViewport(mOgreCamera);
  pViewPort->setBackgroundColour(Ogre::ColourValue(0.3, 0.3, 0.3));

  mOgreCamera->setAspectRatio(Ogre::Real(mRenderWindow->getWidth()) /
                              Ogre::Real(mRenderWindow->getHeight()));
  mOgreCamera->setAutoAspectRatio(true);

  createScene();
}

MainView::~MainView() {
  if (mCameraController) {
    delete mCameraController;
  }
}

void MainView::createScene() {

  mOgreSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

  Ogre::SceneNode *childSceneNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  //  auto dragon = mOgreSceneManager->createEntity("root", "root.mesh");
  //  childSceneNode->attachObject(dragon);

  AssimpSceneLoader loader(childSceneNode);
  //  qDebug() << loader.loadFile("teapot.obj");
  qDebug() << loader.loadFile("okinawa.fbx");

  //  qDebug() << loader.loadFile("test.blend");

  auto itr = childSceneNode->getChildIterator();
  while (itr.hasMoreElements()) {
    Ogre::SceneNode *node = reinterpret_cast<Ogre::SceneNode *>(itr.getNext());
    qDebug() << node->getName().c_str();
    auto itr2 = node->getAttachedObjectIterator();
    while (itr2.hasMoreElements()) {
      Ogre::MovableObject *obj =
          reinterpret_cast<Ogre::MovableObject *>(itr2.getNext());
      qDebug() << obj->getName().c_str();
      if (mOgreSceneManager->hasCamera(obj->getName())) {

        Ogre::Camera *camera = reinterpret_cast<Ogre::Camera *>(obj);

        CameraObject *cameraObject = reinterpret_cast<CameraObject *>(
            mOgreSceneManager->createMovableObject(
                camera->getName() + "001",
                CameraObjectFactory::FACTORY_TYPE_NAME));
        cameraObject->attachCamera(camera);

        camera->getParentSceneNode()->attachObject(cameraObject);
      }
    }
  }

  Ogre::SceneNode *pLightNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();
  Ogre::Light *light = mOgreSceneManager->createLight("MainLight");
  pLightNode->attachObject(light);
  pLightNode->setPosition(20.0f, -80.0f, 50.0f);

  // Camera Controller
  Ogre::SceneNode *centerSceneNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  mCameraController->setTargetNode(centerSceneNode);
  mCameraController->setCameraStyle(CameraController::CS_ORBIT);

  // Axis Grid Line
  AxisGridLineObject *axisGridLine = reinterpret_cast<AxisGridLineObject *>(
      mOgreSceneManager->createMovableObject(
          "AxisGrid", AxisGridLineObjectFactory::FACTORY_TYPE_NAME));

  Ogre::SceneNode *axisGridLineNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();
  axisGridLineNode->attachObject(axisGridLine);

  // Translate Gizmo
  Ogre::SceneNode *translateGizmoNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  mTranslateGizmo =
      reinterpret_cast<TranslateGizmo *>(mOgreSceneManager->createMovableObject(
          "translateGizmo", TranslateGizmoFactory::FACTORY_TYPE_NAME));

  mTranslateGizmo->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
  mTranslateGizmo->setVisibilityFlags(ObjectType_Gizmo);

  translateGizmoNode->attachObject(mTranslateGizmo);
  translateGizmoNode->setPosition(0, 0, 0);

  // Rotate Gizmo
  Ogre::SceneNode *rotateGizmoNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  mRotateGizmo =
      reinterpret_cast<RotateGizmo *>(mOgreSceneManager->createMovableObject(
          "rotateGizmo", RotateGizmoFactory::FACTORY_TYPE_NAME));

  mRotateGizmo->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
  mRotateGizmo->setVisibilityFlags(ObjectType_Gizmo);

  rotateGizmoNode->attachObject(mRotateGizmo);
  rotateGizmoNode->setPosition(-3, -3, -3);

  // Scale Gizmo
  Ogre::SceneNode *scaleGizmoNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  mScaleGizmo =
      reinterpret_cast<ScaleGizmo *>(mOgreSceneManager->createMovableObject(
          "scaleGizmo", ScaleGizmoFactory::FACTORY_TYPE_NAME));

  mScaleGizmo->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
  mScaleGizmo->setVisibilityFlags(ObjectType_Gizmo);

  scaleGizmoNode->attachObject(mScaleGizmo);
  scaleGizmoNode->setPosition(-2, -1, 1);

  // Active Point
  //  Ogre::SceneNode *activePointNode =
  //      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  //  ActivePointObject *activePoint = reinterpret_cast<ActivePointObject *>(
  //      mOgreSceneManager->createMovableObject(
  //          "activePoint", ActivePointObjectFactory::FACTORY_TYPE_NAME));

  //  activePointNode->attachObject(activePoint);

  mGizmoTargetNode = childSceneNode;

  mTranslateGizmo->attachNode(mGizmoTargetNode);
  mRotateGizmo->attachNode(mGizmoTargetNode);
  mScaleGizmo->attachNode(mGizmoTargetNode);

  mTranslateGizmo->setVisible(true);
  mRotateGizmo->setVisible(false);
  mScaleGizmo->setVisible(false);
}

void MainView::keyPressEvent(QKeyEvent *event) {

  if (event->key() == Qt::Key_1) {
    mTranslateGizmo->setVisible(true);
    mRotateGizmo->setVisible(false);
    mScaleGizmo->setVisible(false);
    mTranslateGizmo->attachNode(mGizmoTargetNode);
    mRotateGizmo->attachNode(mGizmoTargetNode);
    mScaleGizmo->attachNode(mGizmoTargetNode);
  } else if (event->key() == Qt::Key_2) {
    mTranslateGizmo->setVisible(false);
    mRotateGizmo->setVisible(true);
    mScaleGizmo->setVisible(false);
    mTranslateGizmo->attachNode(mGizmoTargetNode);
    mRotateGizmo->attachNode(mGizmoTargetNode);
    mScaleGizmo->attachNode(mGizmoTargetNode);
  } else if (event->key() == Qt::Key_3) {
    mTranslateGizmo->setVisible(false);
    mRotateGizmo->setVisible(false);
    mScaleGizmo->setVisible(true);
    mTranslateGizmo->attachNode(mGizmoTargetNode);
    mRotateGizmo->attachNode(mGizmoTargetNode);
    mScaleGizmo->attachNode(mGizmoTargetNode);
  }

  if (mCameraController) {
    mCameraController->keyPressed(event);
    renderNow();
  }
}

void MainView::keyReleaseEvent(QKeyEvent *event) {
  if (mCameraController) {
    mCameraController->keyReleased(event);
    renderNow();
  }
}

void MainView::wheelEvent(QWheelEvent *event) {
  if (mCameraController) {
    mCameraController->mouseWheelRolled(event);
    renderNow();
  }
}

void MainView::mousePressEvent(QMouseEvent *event) {

  Ogre::Real x = Ogre::Real(event->windowPos().x()) / width();
  Ogre::Real y = Ogre::Real(event->windowPos().y()) / height();
  Ogre::Ray ray = mOgreCamera->getCameraToViewportRay(x, y);

  if (mTranslateGizmo->mousePressed(ray)) {
    renderNow();
    return;
  }

  if (mRotateGizmo->mousePressed(ray)) {
    renderNow();
    return;
  }

  if (mScaleGizmo->mousePressed(ray)) {
    renderNow();
    return;
  }

  if (mCameraController) {
    mCameraController->mousePressed(event);
    renderNow();
  }
}

void MainView::mouseMoveEvent(QMouseEvent *event) {

  Ogre::Real x = Ogre::Real(event->windowPos().x()) / width();
  Ogre::Real y = Ogre::Real(event->windowPos().y()) / height();
  Ogre::Ray ray = mOgreCamera->getCameraToViewportRay(x, y);

  if (mTranslateGizmo->mouseMoved(ray)) {
    renderNow();
    return;
  }

  if (mRotateGizmo->mouseMoved(ray)) {
    renderNow();
    return;
  }

  if (mScaleGizmo->mouseMoved(ray)) {
    renderNow();
    return;
  }

  if (mCameraController && (event->buttons() & Qt::LeftButton)) {
    mCameraController->mouseMoved(event);
    renderNow();
  }
}

void MainView::mouseReleaseEvent(QMouseEvent *event) {

  Ogre::Real x = Ogre::Real(event->windowPos().x()) / width();
  Ogre::Real y = Ogre::Real(event->windowPos().y()) / height();
  Ogre::Ray ray = mOgreCamera->getCameraToViewportRay(x, y);

  mTranslateGizmo->mouseReleased(ray);
  mRotateGizmo->mouseReleased(ray);
  mScaleGizmo->mouseReleased(ray);

  if (mCameraController) {
    mCameraController->mouseReleased(event);
    renderNow();
  }

  Ogre::RaySceneQuery *pSceneQuery = mOgreSceneManager->createRayQuery(ray);
  pSceneQuery->setSortByDistance(true);
  Ogre::RaySceneQueryResult vResult = pSceneQuery->execute();
  for (size_t ui = 0; ui < vResult.size(); ui++) {
    if (vResult[ui].movable) {

      qDebug() << reinterpret_cast<Ogre::ManualObject *>(vResult[ui].movable)
                      ->getName()
                      .c_str();

      reinterpret_cast<Ogre::ManualObject *>(vResult[ui].movable)
          ->getParentSceneNode()
          ->showBoundingBox(true);

      if (vResult[ui].movable->getMovableType().compare("Entity") == 0) {
      }
    }
  }

  mOgreSceneManager->destroyQuery(pSceneQuery);
}

bool MainView::frameRenderingQueued(const Ogre::FrameEvent &event) {
  if (mCameraController) {
    mCameraController->frameRendered(event);
  }
  return true;
}
