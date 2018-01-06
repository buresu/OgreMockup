#pragma once

#include <OgreManualObject.h>

class AbstractGizmo : public Ogre::ManualObject {
public:
  explicit AbstractGizmo(const Ogre::String &name);
  virtual ~AbstractGizmo();

  void attachNode(Ogre::Node *node);
  void dettachNode() { mTargetNode = nullptr; }

  virtual bool mousePressed(const Ogre::Ray &) { return false; }
  virtual bool mouseMoved(const Ogre::Ray &) { return false; }
  virtual bool mouseReleased(const Ogre::Ray &) { return false; }

  virtual void _notifyCurrentCamera(Ogre::Camera *camera) override;

protected:
  virtual void render() {}

  Ogre::Node *mTargetNode = nullptr;
  Ogre::Camera *mCurrentCamera = nullptr;
};
