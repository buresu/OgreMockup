#include "OgreWindow.hpp"
#include <OgreRenderWindow.h>
#include <OgreRoot.h>

OgreWindow::OgreWindow(QWindow *parent) : QWindow(parent) {
  initialize();
  installEventFilter(this);
}

OgreWindow::~OgreWindow() {
  if (mRenderWindow) {
    Ogre::RenderSystem *rs = Ogre::Root::getSingletonPtr()->getRenderSystem();
    if (rs) {
      rs->destroyRenderWindow(mRenderWindow->getName());
    }
  }
}

void OgreWindow::render() { Ogre::Root::getSingletonPtr()->renderOneFrame(); }

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
        // TODO: Check the other platforms
#if !defined(Q_OS_WIN) && !defined(Q_OS_MAC)
        mRenderWindow->resize(quint32(width()), quint32(height()));
        mRenderWindow->windowMovedOrResized();
#endif
      }
    } else if (event->type() == QEvent::Expose) {
      if (isExposed()) {
        mRenderWindow->windowMovedOrResized();
        renderNow();
      }
    }
  }

  return false;
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

  parameters["FSAA"] = "0";

  Ogre::String wId = Ogre::StringConverter::toString(size_t(winId()));
  mRenderWindow =
      root->createRenderWindow("RenderWindow #" + wId, quint32(width()),
                               quint32(height()), false, &parameters);

  mRenderWindow->setVSyncEnabled(true);
  mRenderWindow->setVisible(true);

  root->addFrameListener(this);
}
