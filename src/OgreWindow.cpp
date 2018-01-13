#include "OgreWindow.hpp"
#include "controller/CameraController.hpp"
#include "gizmo/RotateGizmo.hpp"
#include "gizmo/ScaleGizmo.hpp"
#include "gizmo/TranslateGizmo.hpp"
#include "object/ActivePointObject.hpp"
#include "object/AxisGridLineObject.hpp"
#include "object/CameraObject.hpp"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreLogManager.h>
#include <OgreMaterialManager.h>
#include <OgreMeshManager.h>
#include <OgreRectangle2D.h>
#include <OgreRenderTexture.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreTechnique.h>
#include <OgreTextureManager.h>
#include <OgreViewport.h>
#include <OgreWindowEventUtilities.h>
#include <QDebug>

OgreWindow::OgreWindow(QWindow *parent) : QWindow(parent) {
  initialize();
  installEventFilter(this);
}

OgreWindow::~OgreWindow() {
  if (mCameraController) {
    delete mCameraController;
  }
}

void OgreWindow::render() {
  Ogre::WindowEventUtilities::messagePump();
  mOgreRoot->renderOneFrame();
}

void OgreWindow::initialize() {

  mOgreRoot = Ogre::Root::getSingletonPtr();
  Ogre::RenderSystem *rs = mOgreRoot->getRenderSystem();

  Ogre::NameValuePairList parameters;

  if (rs->getName().find("GL") <= rs->getName().size()) {
    parameters["currentGLContext"] = Ogre::String("false");
  }

#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
  parameters["externalWindowHandle"] =
      Ogre::StringConverter::toString(size_t(winId()));
  parameters["parentWindowHandle"] =
      Ogre::StringConverter::toString(size_t(winId()));
#else
  parameters["externalWindowHandle"] =
      Ogre::StringConverter::toString((unsigned long)(winId()));
  parameters["parentWindowHandle"] =
      Ogre::StringConverter::toString((unsigned long)(winId()));
#endif

#if defined(Q_OS_MAC)
  parameters["macAPI"] = "cocoa";
  parameters["macAPICocoaUseNSView"] = "true";
#endif

  parameters["FSAA"] = "4";

  mOgreWindow = mOgreRoot->createRenderWindow(
      "QT Window", quint32(width()), quint32(height()), false, &parameters);

  mOgreWindow->setVSyncEnabled(true);
  mOgreWindow->setVisible(true);

  mOgreSceneManager = mOgreRoot->createSceneManager();

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

  Ogre::Viewport *pViewPort = mOgreWindow->addViewport(mOgreCamera);
  pViewPort->setBackgroundColour(Ogre::ColourValue(0.3, 0.3, 0.3));

  mOgreCamera->setAspectRatio(Ogre::Real(mOgreWindow->getWidth()) /
                              Ogre::Real(mOgreWindow->getHeight()));
  mOgreCamera->setAutoAspectRatio(true);

  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  Ogre::SceneNode *viewCameraSceneNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  mViewCamera = mOgreSceneManager->createCamera("ViewCamera");
  mViewCamera->setPosition(Ogre::Vector3(0.0f, 0.0f, 3.0f));
  mViewCamera->lookAt(Ogre::Vector3(0.0f, 0.0f, -0.0f));
  mViewCamera->setNearClipDistance(0.1f);
  mViewCamera->setFarClipDistance(1000.0f);
  //  mOgreCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);

  viewCameraSceneNode->attachObject(mViewCamera);

  createScene();

  mOgreRoot->addFrameListener(this);
}

void OgreWindow::createScene() {

  mOgreSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

  Ogre::Entity *sphereMesh =
      mOgreSceneManager->createEntity("mySphere", Ogre::SceneManager::PT_CUBE);

  Ogre::SceneNode *childSceneNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  //  childSceneNode->attachObject(sphereMesh);

  auto dragon = mOgreSceneManager->createEntity("root", "root.mesh");

  //  Ogre::SceneNode *childSceneNode2 =
  //      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  //  childSceneNode2->attachObject(dragon);
  childSceneNode->attachObject(dragon);

  Ogre::MaterialPtr sphereMaterial =
      Ogre::MaterialManager::getSingleton().create(
          "SphereMaterial",
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);

  sphereMaterial->getTechnique(0)->getPass(0)->setAmbient(0.1f, 0.1f, 0.1f);
  sphereMaterial->getTechnique(0)->getPass(0)->setDiffuse(0.2f, 0.2f, 0.2f,
                                                          1.0f);
  sphereMaterial->getTechnique(0)->getPass(0)->setSpecular(0.9f, 0.9f, 0.9f,
                                                           1.0f);
  sphereMaterial->setAmbient(0.2f, 0.2f, 0.5f);
  sphereMaterial->setSelfIllumination(0.2f, 0.2f, 0.1f);

  sphereMesh->setMaterialName("SphereMaterial");
  childSceneNode->setPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));
  //  childSceneNode->setScale(
  //      Ogre::Vector3(0.01f, 0.01f, 0.01f)); // Radius, in theory.
  //  childSceneNode->showBoundingBox(true);
  dragon->setMaterialName("SphereMaterial");

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
  translateGizmoNode->attachObject(mTranslateGizmo);
  translateGizmoNode->setPosition(0, 0, 0);

  // Rotate Gizmo
  Ogre::SceneNode *rotateGizmoNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  mRotateGizmo =
      reinterpret_cast<RotateGizmo *>(mOgreSceneManager->createMovableObject(
          "rotateGizmo", RotateGizmoFactory::FACTORY_TYPE_NAME));

  mRotateGizmo->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
  rotateGizmoNode->attachObject(mRotateGizmo);
  rotateGizmoNode->setPosition(-3, -3, -3);

  // Scale Gizmo
  Ogre::SceneNode *scaleGizmoNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  mScaleGizmo =
      reinterpret_cast<ScaleGizmo *>(mOgreSceneManager->createMovableObject(
          "scaleGizmo", ScaleGizmoFactory::FACTORY_TYPE_NAME));

  mScaleGizmo->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
  scaleGizmoNode->attachObject(mScaleGizmo);
  scaleGizmoNode->setPosition(-2, -1, 1);

  // Active Point
  //  Ogre::SceneNode *activePointNode =
  //      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  //  ActivePointObject *activePoint = reinterpret_cast<ActivePointObject *>(
  //      mOgreSceneManager->createMovableObject(
  //          "activePoint", ActivePointObjectFactory::FACTORY_TYPE_NAME));

  //  activePointNode->attachObject(activePoint);

  // Camera
  Ogre::SceneNode *cameraNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  CameraObject *cameraObject =
      reinterpret_cast<CameraObject *>(mOgreSceneManager->createMovableObject(
          "camera", CameraObjectFactory::FACTORY_TYPE_NAME));

  cameraNode->attachObject(cameraObject);
  cameraNode->setPosition(0, -2, 2);

  mGizmoTargetNode = childSceneNode;

  mTranslateGizmo->attachNode(mGizmoTargetNode);
  mRotateGizmo->attachNode(mGizmoTargetNode);
  mScaleGizmo->attachNode(mGizmoTargetNode);

  mTranslateGizmo->setVisible(true);
  mRotateGizmo->setVisible(false);
  mScaleGizmo->setVisible(false);

  // Render to Texture
  Ogre::TexturePtr rttTexture =
      Ogre::TextureManager::getSingleton().createManual(
          "RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
          Ogre::TEX_TYPE_2D, 640, 480, 0, Ogre::PF_R8G8B8,
          Ogre::TU_RENDERTARGET);

  Ogre::RenderTexture *renderTexture =
      rttTexture->getBuffer()->getRenderTarget();

  renderTexture->addViewport(mViewCamera);
  renderTexture->getViewport(0)->setClearEveryFrame(true);
  renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
  renderTexture->getViewport(0)->setOverlaysEnabled(false);
  renderTexture->getViewport(0)->setBackgroundColour(
      Ogre::ColourValue(0.2, 0.2, 0.2));

  auto mMiniScreen = new Ogre::Rectangle2D(true);
  mMiniScreen->setCorners(.5, 1.0, 1.0, .5);
  mMiniScreen->setBoundingBox(Ogre::AxisAlignedBox::BOX_INFINITE);

  Ogre::SceneNode *miniScreenNode =
      mOgreSceneManager->getRootSceneNode()->createChildSceneNode();

  miniScreenNode->attachObject(mMiniScreen);

  Ogre::MaterialPtr renderMaterial =
      Ogre::MaterialManager::getSingleton().create(
          "RttMat", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  renderMaterial->getTechnique(0)->getPass(0)->setLightingEnabled(false);
  renderMaterial->getTechnique(0)->getPass(0)->createTextureUnitState("RttTex");

  mMiniScreen->setMaterial(renderMaterial);

  //  renderTexture->addListener(this);
}

void OgreWindow::renderLater() { requestUpdate(); }

void OgreWindow::renderNow() {
  if (isExposed()) {
    mAnimating ? renderLater() : render();
  }
}

bool OgreWindow::eventFilter(QObject *target, QEvent *event) {

  if (target == this) {
    if (event->type() == QEvent::Resize) {
      if (isExposed() && mOgreWindow != nullptr) {
        mOgreWindow->resize(quint32(width()), quint32(height()));
        mOgreWindow->windowMovedOrResized();
      }
    }
  }

  return false;
}

void OgreWindow::keyPressEvent(QKeyEvent *event) {

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

void OgreWindow::keyReleaseEvent(QKeyEvent *event) {
  if (mCameraController) {
    mCameraController->keyReleased(event);
    renderNow();
  }
}

void OgreWindow::wheelEvent(QWheelEvent *event) {
  if (mCameraController) {
    mCameraController->mouseWheelRolled(event);
    renderNow();
  }
}

void OgreWindow::mousePressEvent(QMouseEvent *event) {

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

void OgreWindow::mouseMoveEvent(QMouseEvent *event) {

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

  static int lastX = event->x();
  static int lastY = event->y();
  int relX = event->x() - lastX;
  int relY = event->y() - lastY;
  lastX = event->x();
  lastY = event->y();

  if (mCameraController && (event->buttons() & Qt::LeftButton)) {
    mCameraController->mouseMoved(relX, relY);
    renderNow();
  }
}

void OgreWindow::mouseReleaseEvent(QMouseEvent *event) {

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

  QPoint pos = event->pos();

  Ogre::Ray mouseRay = mOgreCamera->getCameraToViewportRay(
      Ogre::Real(pos.x()) / mOgreWindow->getWidth(),
      Ogre::Real(pos.y()) / mOgreWindow->getHeight());

  Ogre::RaySceneQuery *pSceneQuery =
      mOgreSceneManager->createRayQuery(mouseRay);
  pSceneQuery->setSortByDistance(true);
  Ogre::RaySceneQueryResult vResult = pSceneQuery->execute();
  for (size_t ui = 0; ui < vResult.size(); ui++) {
    if (vResult[ui].movable) {
      if (vResult[ui].movable->getMovableType().compare("Entity") == 0) {
        emit entitySelected(
            reinterpret_cast<Ogre::Entity *>(vResult[ui].movable));
      }
    }
  }

  mOgreSceneManager->destroyQuery(pSceneQuery);
}

void OgreWindow::exposeEvent(QExposeEvent *) {
  if (isExposed()) {
    renderNow();
  }
}

bool OgreWindow::frameRenderingQueued(const Ogre::FrameEvent &event) {
  if (mCameraController) {
    mCameraController->frameRendered(event);
  }
  return true;
}

void OgreWindow::log(Ogre::String msg) {
  if (Ogre::LogManager::getSingletonPtr() != nullptr) {
    Ogre::LogManager::getSingletonPtr()->logMessage(msg);
  }
}

void OgreWindow::log(QString msg) {
  log(Ogre::String(msg.toStdString().c_str()));
}
