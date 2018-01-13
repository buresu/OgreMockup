#pragma once

#include "AbstractGizmo.hpp"

class ScaleGizmo : public AbstractGizmo {
public:
  enum ScaleType {
    ScaleType_None,
    ScaleType_X,
    ScaleType_Y,
    ScaleType_Z,
    ScaleType_Camera
  };

  explicit ScaleGizmo(const Ogre::String &name);
  virtual ~ScaleGizmo();

  virtual bool mousePressed(const Ogre::Ray &ray) override;
  virtual bool mouseMoved(const Ogre::Ray &ray) override;
  virtual bool mouseReleased(const Ogre::Ray &ray) override;

protected:
  virtual void render() override;

  ScaleType mCurrentScaleType = ScaleType_None;
  Ogre::Vector3 mStartPosition = Ogre::Vector3::ZERO;

  Ogre::Vector3 mStartScale = Ogre::Vector3::UNIT_SCALE;
};

class ScaleGizmoFactory : public Ogre::MovableObjectFactory {
public:
  static Ogre::String FACTORY_TYPE_NAME;

  ScaleGizmoFactory() {}
  ~ScaleGizmoFactory() {}

  const Ogre::String &getType(void) const override;
  void destroyInstance(Ogre::MovableObject *obj) override;

protected:
  Ogre::MovableObject *
  createInstanceImpl(const Ogre::String &name,
                     const Ogre::NameValuePairList *params) override;
};
