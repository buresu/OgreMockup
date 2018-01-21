#include "AbstractGizmo.hpp"
#include <OgreCamera.h>
#include <OgreNode.h>

AbstractGizmo::AbstractGizmo(const Ogre::String &name)
    : Ogre::ManualObject(name) {
  setDynamic(true);
}

AbstractGizmo::~AbstractGizmo() {}

void AbstractGizmo::attachNode(Ogre::Node *node) {
  mTargetNode = node;
  getParentNode()->setPosition(node->getPosition());
}

#include <QDebug>

void AbstractGizmo::_notifyCurrentCamera(Ogre::Camera *camera) {

  // FIXME
  if (camera->getName() == "MainCamera") {
    mCurrentCamera = camera;
  }
  //  qDebug() << camera->getName().c_str();
  render();
}
