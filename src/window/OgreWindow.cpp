#include "OgreWindow.hpp"
#include <OgreLogManager.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

OgreWindow::OgreWindow(QWindow *parent) : QWindow(parent) {
  initialize();
  installEventFilter(this);
}

OgreWindow::~OgreWindow() {
  Ogre::Root *root = Ogre::Root::getSingletonPtr();
  QString ptr = QString("0x%1").arg(reinterpret_cast<quintptr>(this),
                                    QT_POINTER_SIZE * 2, 16, QChar('0'));
  root->getRenderSystem()->destroyRenderWindow("RenderWindow:" +
                                               ptr.toStdString());
}

void OgreWindow::render() {
  Ogre::WindowEventUtilities::messagePump();
  Ogre::Root::getSingletonPtr()->renderOneFrame();
}

void OgreWindow::initialize() {

  Ogre::Root *root = Ogre::Root::getSingletonPtr();
  Ogre::RenderSystem *rs = root->getRenderSystem();

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

  QString ptr = QString("0x%1").arg(reinterpret_cast<quintptr>(this),
                                    QT_POINTER_SIZE * 2, 16, QChar('0'));
  mRenderWindow = root->createRenderWindow("RenderWindow:" + ptr.toStdString(),
                                           quint32(width()), quint32(height()),
                                           false, &parameters);

  mRenderWindow->setVSyncEnabled(true);
  mRenderWindow->setVisible(true);

  root->addFrameListener(this);
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
      if (isExposed() && mRenderWindow != nullptr) {
        mRenderWindow->resize(quint32(width()), quint32(height()));
        mRenderWindow->windowMovedOrResized();
      }
    }
  }

  return false;
}

void OgreWindow::exposeEvent(QExposeEvent *) {
  if (isExposed()) {
    renderNow();
  }
}

void OgreWindow::log(const Ogre::String &msg) {
  if (Ogre::LogManager::getSingletonPtr() != nullptr) {
    Ogre::LogManager::getSingletonPtr()->logMessage(msg);
  }
}

void OgreWindow::log(const QString &msg) {
  log(Ogre::String(msg.toStdString().c_str()));
}
