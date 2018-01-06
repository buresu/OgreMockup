#pragma once

#include "AbstractGizmo.hpp"

class RotateGizmo : public AbstractGizmo {
public:
  enum RotateType {
    RotateType_None,
    RotateType_X,
    RotateType_Y,
    RotateType_Z,
    RotateType_Camera
  };

  explicit RotateGizmo(const Ogre::String &name);
  virtual ~RotateGizmo();

  virtual bool mousePressed(const Ogre::Ray &ray) override;
  virtual bool mouseMoved(const Ogre::Ray &ray) override;
  virtual bool mouseReleased(const Ogre::Ray &ray) override;

protected:
  virtual void render() override;

  RotateType mCurrentRotateType = RotateType_None;
  Ogre::Vector3 mStartVec = Ogre::Vector3::ZERO;
  Ogre::Quaternion mStartOrientation;
};

class RotateGizmoFactory : public Ogre::MovableObjectFactory {
public:
  static Ogre::String FACTORY_TYPE_NAME;

  RotateGizmoFactory() {}
  ~RotateGizmoFactory() {}

  const Ogre::String &getType(void) const override;
  void destroyInstance(Ogre::MovableObject *obj) override;

protected:
  Ogre::MovableObject *
  createInstanceImpl(const Ogre::String &name,
                     const Ogre::NameValuePairList *params) override;
};
