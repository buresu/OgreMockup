#pragma once

#include <OgreManualObject.h>

class AxisGridLineObject : public Ogre::ManualObject {
public:
  explicit AxisGridLineObject(const Ogre::String &name);
  virtual ~AxisGridLineObject();
};

class AxisGridLineObjectFactory : public Ogre::MovableObjectFactory {
public:
  static Ogre::String FACTORY_TYPE_NAME;

  AxisGridLineObjectFactory() {}
  ~AxisGridLineObjectFactory() {}

  const Ogre::String &getType(void) const override;
  void destroyInstance(Ogre::MovableObject *obj) override;

protected:
  Ogre::MovableObject *
  createInstanceImpl(const Ogre::String &name,
                     const Ogre::NameValuePairList *params) override;
};
