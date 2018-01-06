#pragma once

#include <OgreManualObject.h>

class ActivePointObject : public Ogre::ManualObject {
public:
  explicit ActivePointObject(const Ogre::String &name);
  virtual ~ActivePointObject();
};

class ActivePointObjectFactory : public Ogre::MovableObjectFactory {
public:
  static Ogre::String FACTORY_TYPE_NAME;

  ActivePointObjectFactory() {}
  ~ActivePointObjectFactory() {}

  const Ogre::String &getType(void) const override;
  void destroyInstance(Ogre::MovableObject *obj) override;

protected:
  Ogre::MovableObject *
  createInstanceImpl(const Ogre::String &name,
                     const Ogre::NameValuePairList *params) override;
};
