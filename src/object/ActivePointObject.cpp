#include "ActivePointObject.hpp"
#include <OgreCamera.h>
#include <OgreSceneNode.h>

ActivePointObject::ActivePointObject(const Ogre::String &name)
    : Ogre::ManualObject(name) {

  int vertex = 32;
  float radius = 0.05f;

  begin("ActivePoint/Red", Ogre::RenderOperation::OT_TRIANGLE_LIST);

  for (int i = 0; i <= vertex; ++i) {

    Ogre::Real p1x =
        Ogre::Math::Cos(Ogre::Radian((Ogre::Math::TWO_PI * i) / vertex)) *
        radius;
    Ogre::Real p1y =
        Ogre::Math::Sin(Ogre::Radian((Ogre::Math::TWO_PI * i) / vertex)) *
        radius;
    Ogre::Real p2x =
        Ogre::Math::Cos(Ogre::Radian((Ogre::Math::TWO_PI * (i + 1)) / vertex)) *
        radius;
    Ogre::Real p2y =
        Ogre::Math::Sin(Ogre::Radian((Ogre::Math::TWO_PI * (i + 1)) / vertex)) *
        radius;

    position(0, 0, 0);
    position(p1x, p1y, 0);
    position(p2x, p2y, 0);
  }

  end();

  begin("ActivePoint/Black", Ogre::RenderOperation::OT_LINE_LIST);

  for (int i = 0; i <= vertex; ++i) {

    Ogre::Real p1x =
        Ogre::Math::Cos(Ogre::Radian((Ogre::Math::TWO_PI * i) / vertex)) *
        radius;
    Ogre::Real p1y =
        Ogre::Math::Sin(Ogre::Radian((Ogre::Math::TWO_PI * i) / vertex)) *
        radius;
    Ogre::Real p2x =
        Ogre::Math::Cos(Ogre::Radian((Ogre::Math::TWO_PI * (i + 1)) / vertex)) *
        radius;
    Ogre::Real p2y =
        Ogre::Math::Sin(Ogre::Radian((Ogre::Math::TWO_PI * (i + 1)) / vertex)) *
        radius;

    position(p1x, p1y, 0);
    position(p2x, p2y, 0);
  }

  end();
}

ActivePointObject::~ActivePointObject() {}

Ogre::String ActivePointObjectFactory::FACTORY_TYPE_NAME = "ActivePointObject";

const Ogre::String &ActivePointObjectFactory::getType() const {
  return FACTORY_TYPE_NAME;
}

void ActivePointObjectFactory::destroyInstance(Ogre::MovableObject *obj) {
  delete obj;
}

Ogre::MovableObject *
ActivePointObjectFactory::createInstanceImpl(const Ogre::String &name,
                                             const Ogre::NameValuePairList *) {
  return new ActivePointObject(name);
}
