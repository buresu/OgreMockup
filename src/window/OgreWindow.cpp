#include "OgreWindow.hpp"
#include <OgreRenderWindow.h>
#include <OgreRoot.h>

OgreWindow::OgreWindow(QWindow *parent) : QWindow(parent) {
  initialize();
  installEventFilter(this);
}

OgreWindow::~OgreWindow() {
  Ogre::RenderSystem *rs = Ogre::Root::getSingletonPtr()->getRenderSystem();
  if (mRenderWindow && rs) {
    rs->destroyRenderWindow(mRenderWindow->getName());
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
    if (event->type() == QEvent::Expose) {
      if (isExposed() && mRenderWindow) {
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

  Ogre::NameValuePairList params;

  if (rs->getName().find("GL") <= rs->getName().size()) {
    params["currentGLContext"] = Ogre::String("false");
  }

  params["externalWindowHandle"] =
      Ogre::StringConverter::toString(size_t(winId()));
  params["parentWindowHandle"] =
      Ogre::StringConverter::toString(size_t(winId()));

#if defined(Q_OS_MAC)
  params["macAPI"] = "cocoa";
#endif

  params["FSAA"] = "4";

  Ogre::String wId = Ogre::StringConverter::toString(size_t(winId()));
  mRenderWindow =
      root->createRenderWindow("RenderWindow #" + wId, quint32(width()),
                               quint32(height()), false, &params);

  mRenderWindow->setVSyncEnabled(true);
  mRenderWindow->setVisible(true);

  root->addFrameListener(this);
}
