#include "AxisGridLineObject.hpp"

AxisGridLineObject::AxisGridLineObject(const Ogre::String &name)
    : Ogre::ManualObject(name) {

  begin("AxisGridLine/Red", Ogre::RenderOperation::OT_LINE_LIST);

  position(-8, 0, 0);
  position(8, 0, 0);

  end();

  begin("AxisGridLine/Green", Ogre::RenderOperation::OT_LINE_LIST);

  position(0, -8, 0);
  position(0, 8, 0);

  end();

  begin("AxisGridLine/Gray", Ogre::RenderOperation::OT_LINE_LIST);

  for (int i = 1; i < 9; ++i) {
    position(-8, i, 0);
    position(8, i, 0);
  }

  for (int i = 1; i < 9; ++i) {
    position(-8, -i, 0);
    position(8, -i, 0);
  }

  for (int i = 1; i < 9; ++i) {
    position(i, -8, 0);
    position(i, 8, 0);
  }

  for (int i = 1; i < 9; ++i) {
    position(-i, -8, 0);
    position(-i, 8, 0);
  }

  end();
}

AxisGridLineObject::~AxisGridLineObject() {}

Ogre::String AxisGridLineObjectFactory::FACTORY_TYPE_NAME =
    "AxisGridLineObject";

const Ogre::String &AxisGridLineObjectFactory::getType() const {
  return FACTORY_TYPE_NAME;
}

void AxisGridLineObjectFactory::destroyInstance(Ogre::MovableObject *obj) {
  delete obj;
}

Ogre::MovableObject *
AxisGridLineObjectFactory::createInstanceImpl(const Ogre::String &name,
                                              const Ogre::NameValuePairList *) {
  return new AxisGridLineObject(name);
}
