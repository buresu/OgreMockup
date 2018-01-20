#include "CameraObject.hpp"
#include <OgreCamera.h>

CameraObject::CameraObject(const Ogre::String &name)
    : Ogre::ManualObject(name) {
  render();
}

CameraObject::~CameraObject() {}

void CameraObject::attachCamera(Ogre::Camera *camera) {
  mCamera = camera;
  render();
}

void CameraObject::detachCamera() {
  mCamera = nullptr;
  render();
}

void CameraObject::render() {

  Ogre::Real cameraClip = 0.5;
  Ogre::Real nearClip = 5.0;
  Ogre::Real farClip = 50.0;
  Ogre::Real aspect = 1.5;
  Ogre::Radian fovY = Ogre::Degree(45);

  if (mCamera) {
    nearClip = mCamera->getNearClipDistance();
    farClip = mCamera->getFarClipDistance();
    aspect = mCamera->getAspectRatio();
    fovY = mCamera->getFOVy();
  }

  Ogre::Radian fovX = 2 * Ogre::Math::ATan(Ogre::Math::Tan(fovY / 2) * aspect);

  clear();

  // Camera
  begin("Camera/Black", Ogre::RenderOperation::OT_LINE_LIST);

  Ogre::Real cameraX =
      cameraClip * Ogre::Math::Sin(fovX / 2) / Ogre::Math::Cos(fovX / 2);
  Ogre::Real cameraY =
      cameraClip * Ogre::Math::Sin(fovY / 2) / Ogre::Math::Cos(fovY / 2);

  position(-cameraX, cameraY, -cameraClip);
  position(cameraX, cameraY, -cameraClip);

  position(cameraX, cameraY, -cameraClip);
  position(cameraX, -cameraY, -cameraClip);

  position(cameraX, -cameraY, -cameraClip);
  position(-cameraX, -cameraY, -cameraClip);

  position(-cameraX, -cameraY, -cameraClip);
  position(-cameraX, cameraY, -cameraClip);

  position(0, 0, 0);
  position(-cameraX, cameraY, -cameraClip);

  position(0, 0, 0);
  position(cameraX, cameraY, -cameraClip);

  position(0, 0, 0);
  position(cameraX, -cameraY, -cameraClip);

  position(0, 0, 0);
  position(-cameraX, -cameraY, -cameraClip);

  end();

  begin("Camera/Black", Ogre::RenderOperation::OT_TRIANGLE_STRIP);

  Ogre::Real offset = Ogre::Real(0.025);
  Ogre::Real triSize = cameraX * Ogre::Real(0.5);

  position(0, cameraY + offset + triSize, -cameraClip);
  position(-triSize, cameraY + offset, -cameraClip);
  position(triSize, cameraY + offset, -cameraClip);

  end();

  // Near
  Ogre::Real nearX =
      nearClip * Ogre::Math::Sin(fovX / 2) / Ogre::Math::Cos(fovX / 2);
  Ogre::Real nearY =
      nearClip * Ogre::Math::Sin(fovY / 2) / Ogre::Math::Cos(fovY / 2);

  begin("Camera/Yellow", Ogre::RenderOperation::OT_LINE_LIST);

  position(-nearX, nearY, -nearClip);
  position(nearX, nearY, -nearClip);

  position(nearX, nearY, -nearClip);
  position(nearX, -nearY, -nearClip);

  position(nearX, -nearY, -nearClip);
  position(-nearX, -nearY, -nearClip);

  position(-nearX, -nearY, -nearClip);
  position(-nearX, nearY, -nearClip);

  end();

  // Far
  Ogre::Real farX =
      farClip * Ogre::Math::Sin(fovX / 2) / Ogre::Math::Cos(fovX / 2);
  Ogre::Real farY =
      farClip * Ogre::Math::Sin(fovY / 2) / Ogre::Math::Cos(fovY / 2);

  begin("Camera/Yellow", Ogre::RenderOperation::OT_LINE_LIST);

  position(-farX, farY, -farClip);
  position(farX, farY, -farClip);

  position(farX, farY, -farClip);
  position(farX, -farY, -farClip);

  position(farX, -farY, -farClip);
  position(-farX, -farY, -farClip);

  position(-farX, -farY, -farClip);
  position(-farX, farY, -farClip);

  end();

  begin("Camera/Yellow", Ogre::RenderOperation::OT_LINE_LIST);

  position(-nearX, nearY, -nearClip);
  position(-farX, farY, -farClip);

  position(nearX, nearY, -nearClip);
  position(farX, farY, -farClip);

  position(nearX, -nearY, -nearClip);
  position(farX, -farY, -farClip);

  position(-nearX, -nearY, -nearClip);
  position(-farX, -farY, -farClip);

  end();
}

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
