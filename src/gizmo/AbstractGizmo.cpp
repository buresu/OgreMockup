#include "AbstractGizmo.hpp"
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

void AbstractGizmo::_notifyCurrentCamera(Ogre::Camera *camera) {
  mCurrentCamera = camera;
  render();
}
