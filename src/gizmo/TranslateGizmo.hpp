#pragma once

#include "AbstractGizmo.hpp"

class TranslateGizmo : public AbstractGizmo {
public:
  enum TranslateType {
    TranslateType_None,
    TranslateType_X,
    TranslateType_Y,
    TranslateType_Z,
    TranslateType_Camera
  };

  explicit TranslateGizmo(const Ogre::String &name);
  virtual ~TranslateGizmo();

  virtual bool mousePressed(const Ogre::Ray &ray) override;
  virtual bool mouseMoved(const Ogre::Ray &ray) override;
  virtual bool mouseReleased(const Ogre::Ray &ray) override;

protected:
  virtual void render() override;

  TranslateType mCurrentTranslateType = TranslateType_None;
  Ogre::Vector3 mStartPosition = Ogre::Vector3::ZERO;

  Ogre::Vector3 mTargetStart = Ogre::Vector3::ZERO;
};

class TranslateGizmoFactory : public Ogre::MovableObjectFactory {
public:
  static Ogre::String FACTORY_TYPE_NAME;

  TranslateGizmoFactory() {}
  ~TranslateGizmoFactory() {}

  const Ogre::String &getType(void) const override;
  void destroyInstance(Ogre::MovableObject *obj) override;

protected:
  Ogre::MovableObject *
  createInstanceImpl(const Ogre::String &name,
                     const Ogre::NameValuePairList *params) override;
};
