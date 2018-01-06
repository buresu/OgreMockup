#include "CameraObject.hpp"

CameraObject::CameraObject(const Ogre::String &name)
    : Ogre::ManualObject(name) {

  float widthSize = 0.2f;
  float heightSize = 0.1f;
  float depthSize = 0.2f;

  begin("Camera/Black", Ogre::RenderOperation::OT_LINE_LIST);

  position(0, 0, 0);
  position(-widthSize * 0.5f, heightSize * 0.5f, -depthSize);

  position(0, 0, 0);
  position(widthSize * 0.5f, heightSize * 0.5f, -depthSize);

  position(0, 0, 0);
  position(widthSize * 0.5f, -heightSize * 0.5f, -depthSize);

  position(0, 0, 0);
  position(-widthSize * 0.5f, -heightSize * 0.5f, -depthSize);

  position(-widthSize * 0.5f, heightSize * 0.5f, -depthSize);
  position(widthSize * 0.5f, heightSize * 0.5f, -depthSize);

  position(widthSize * 0.5f, heightSize * 0.5f, -depthSize);
  position(widthSize * 0.5f, -heightSize * 0.5f, -depthSize);

  position(widthSize * 0.5f, -heightSize * 0.5f, -depthSize);
  position(-widthSize * 0.5f, -heightSize * 0.5f, -depthSize);

  position(-widthSize * 0.5f, -heightSize * 0.5f, -depthSize);
  position(-widthSize * 0.5f, heightSize * 0.5f, -depthSize);

  end();

  begin("Camera/Black", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

  float offset = 0.01f;
  float triSize = 0.05f;

  position(0, heightSize * 0.5f + offset + triSize, -depthSize);
  position(-triSize * 1.4f, heightSize * 0.5f + offset, -depthSize);
  position(triSize * 1.4f, heightSize * 0.5f + offset, -depthSize);

  end();
}

CameraObject::~CameraObject() {}

Ogre::String CameraObjectFactory::FACTORY_TYPE_NAME = "CameraObject";

const Ogre::String &CameraObjectFactory::getType() const {
  return FACTORY_TYPE_NAME;
}

void CameraObjectFactory::destroyInstance(Ogre::MovableObject *obj) {
  delete obj;
}

Ogre::MovableObject *
CameraObjectFactory::createInstanceImpl(const Ogre::String &name,
                                        const Ogre::NameValuePairList *) {
  return new CameraObject(name);
}
