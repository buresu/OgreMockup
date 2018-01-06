#include "RotateGizmo.hpp"
#include <OgreCamera.h>
#include <OgreRay.h>
#include <OgreSceneNode.h>
#include <QDebug>

RotateGizmo::RotateGizmo(const Ogre::String &name) : AbstractGizmo(name) {
  render();
}

RotateGizmo::~RotateGizmo() {}

bool RotateGizmo::mousePressed(const Ogre::Ray &ray) {

  if (!isVisible()) {
    return false;
  }

  Ogre::Real size = 1.0;
  Ogre::Vector3 cameraVec = Ogre::Vector3::UNIT_Z;

  if (mCurrentCamera) {

    Ogre::Vector3 camPos = mCurrentCamera->getDerivedPosition();
    Ogre::Vector3 objPos = getParentNode()->getPosition();

    size = camPos.distance(objPos) * 0.1f;

    cameraVec = camPos - objPos;
    cameraVec.normalise();
  }

  // X Coordinate
  auto r = ray.intersects(
      Ogre::Plane(Ogre::Vector3::UNIT_X, getParentNode()->getPosition()));

  if (r.first) {

    auto v = ray.getPoint(r.second) - getParentNode()->getPosition();

    if (cameraVec.directionEquals(v, Ogre::Degree(90))) {
      if (v.length() > size * Ogre::Real(0.9) &&
          v.length() < size * Ogre::Real(1.1)) {
        mCurrentRotateType = RotateType_X;
      }
    }
  }

  // Y Coordinate
  r = ray.intersects(
      Ogre::Plane(Ogre::Vector3::UNIT_Y, getParentNode()->getPosition()));

  if (r.first) {

    auto v = ray.getPoint(r.second) - getParentNode()->getPosition();

    if (cameraVec.directionEquals(v, Ogre::Degree(90))) {
      if (v.length() > size * Ogre::Real(0.9) &&
          v.length() < size * Ogre::Real(1.1)) {
        mCurrentRotateType = RotateType_Y;
      }
    }
  }

  // Z Coordinate
  r = ray.intersects(
      Ogre::Plane(Ogre::Vector3::UNIT_Z, getParentNode()->getPosition()));

  if (r.first) {

    auto v = ray.getPoint(r.second) - getParentNode()->getPosition();

    if (cameraVec.directionEquals(v, Ogre::Degree(90))) {
      if (v.length() > size * Ogre::Real(0.9) &&
          v.length() < size * Ogre::Real(1.1)) {
        mCurrentRotateType = RotateType_Z;
      }
    }
  }

  // XYZ Coordinate
  r = ray.intersects(Ogre::Sphere(getParentNode()->getPosition(), size));

  if (r.first) {

    auto v = ray.getPoint(r.second) - getParentSceneNode()->getPosition();

    if (cameraVec.directionEquals(v, Ogre::Degree(90))) {

      // X
      auto degX = Ogre::Vector3::UNIT_X.angleBetween(v);
      if (degX < Ogre::Degree(95) && degX > Ogre::Degree(85)) {
        mCurrentRotateType = RotateType_X;
      }

      // Y
      auto degY = Ogre::Vector3::UNIT_Y.angleBetween(v);
      if (degY < Ogre::Degree(95) && degY > Ogre::Degree(85)) {
        mCurrentRotateType = RotateType_Y;
      }

      // Z
      auto degZ = Ogre::Vector3::UNIT_Z.angleBetween(v);
      if (degZ < Ogre::Degree(95) && degZ > Ogre::Degree(85)) {
        mCurrentRotateType = RotateType_Z;
      }
    }
  }

  // Camera Coordinate
  r = ray.intersects(Ogre::Plane(cameraVec, getParentNode()->getPosition()));

  if (r.first) {

    auto v = ray.getPoint(r.second) - getParentSceneNode()->getPosition();

    if (v.length() > size * Ogre::Real(1.2) * Ogre::Real(0.9) &&
        v.length() < size * Ogre::Real(1.2) * Ogre::Real(1.1)) {
      mCurrentRotateType = RotateType_Camera;
    }
  }

  // Rotation
  if (mCurrentRotateType != RotateType_None) {

    if (r.first) {
      mStartVec = ray.getPoint(r.second) - getParentNode()->getPosition();
      mStartOrientation = mTargetNode->getOrientation();
      render();
      return true;
    }
  }

  return false;
}

bool RotateGizmo::mouseMoved(const Ogre::Ray &ray) {

  if (mCurrentRotateType == RotateType_None || !isVisible()) {
    return false;
  }

  Ogre::Real size = 1.0;
  Ogre::Vector3 cameraVec = Ogre::Vector3::UNIT_Z;

  if (mCurrentCamera) {

    Ogre::Vector3 camPos = mCurrentCamera->getDerivedPosition();
    Ogre::Vector3 objPos = getParentNode()->getPosition();

    size = camPos.distance(objPos) * 0.1f;

    cameraVec = camPos - objPos;
  }

  Ogre::Plane camPlane(cameraVec, getParentNode()->getPosition());
  auto r = ray.intersects(camPlane);
  Ogre::Vector3 current = Ogre::Vector3::ZERO;

  if (r.first) {
    current = ray.getPoint(r.second) - getParentNode()->getPosition();
  }

  Ogre::Real sinAngle =
      mStartVec.crossProduct(current).dotProduct(mStartVec.perpendicular());
  Ogre::Real cosAngle = mStartVec.dotProduct(current);
  Ogre::Radian angle = Ogre::Math::ATan2(sinAngle, cosAngle);

  if (mCurrentRotateType == RotateType_X) {
    mTargetNode->setOrientation(mStartOrientation);
    mTargetNode->pitch(angle, Ogre::Node::TS_WORLD);
  }

  if (mCurrentRotateType == RotateType_Y) {
    mTargetNode->setOrientation(mStartOrientation);
    mTargetNode->yaw(angle, Ogre::Node::TS_WORLD);
  }

  if (mCurrentRotateType == RotateType_Z) {
    mTargetNode->setOrientation(mStartOrientation);
    mTargetNode->roll(angle, Ogre::Node::TS_WORLD);
  }

  render();

  return true;
}

bool RotateGizmo::mouseReleased(const Ogre::Ray &) {

  if (mCurrentRotateType == RotateType_None || !isVisible()) {
    return false;
  }

  mCurrentRotateType = RotateType_None;
  mStartVec = Ogre::Vector3::ZERO;
  mStartOrientation = Ogre::Quaternion::IDENTITY;

  render();

  return true;
}

void RotateGizmo::render() {

  Ogre::Real size = 1.0;
  Ogre::Vector3 cameraVec = Ogre::Vector3::UNIT_Z;

  if (mCurrentCamera) {

    Ogre::Vector3 camPos = mCurrentCamera->getDerivedPosition();
    Ogre::Vector3 objPos = getParentNode()->getPosition();

    size = camPos.distance(objPos) * 0.1f;

    cameraVec = camPos - objPos;
    cameraVec.normalise();
  }

  clear();

  Ogre::Quaternion cameraRot = Ogre::Vector3::UNIT_Z.getRotationTo(cameraVec);

  begin("Gizmo/White", Ogre::RenderOperation::OT_LINE_LIST);

  Ogre::Vector3 yVec1 = Ogre::Vector3::UNIT_Y * size * 0.15f;
  Ogre::Quaternion zRot1(Ogre::Degree(10), Ogre::Vector3::UNIT_Z);

  for (int i = 0; i < 36; ++i) {
    position(cameraRot * yVec1);
    yVec1 = zRot1 * yVec1;
    position(cameraRot * yVec1);
  }

  end();

  begin("Gizmo/Black", Ogre::RenderOperation::OT_LINE_LIST);

  yVec1 = Ogre::Vector3::UNIT_Y * size;

  for (int i = 0; i < 36; ++i) {
    position(cameraRot * yVec1);
    yVec1 = zRot1 * yVec1;
    position(cameraRot * yVec1);
  }

  end();

  // Camera Rotation
  if (mCurrentRotateType == RotateType_Camera) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/White", Ogre::RenderOperation::OT_LINE_LIST);
  }

  yVec1 = Ogre::Vector3::UNIT_Y * size * Ogre::Real(1.2);

  for (int i = 0; i < 36; ++i) {
    position(cameraRot * yVec1);
    yVec1 = zRot1 * yVec1;
    position(cameraRot * yVec1);
  }

  end();

  // X Rotation
  if (mCurrentRotateType == RotateType_X) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Red", Ogre::RenderOperation::OT_LINE_LIST);
  }

  Ogre::Vector3 zVec = Ogre::Vector3::UNIT_Z * size;
  Ogre::Quaternion xRot(Ogre::Degree(10), Ogre::Vector3::UNIT_X);

  for (int i = 0; i < 36; ++i) {

    if (cameraVec.directionEquals(zVec, Ogre::Degree(89.9f))) {
      position(zVec);
      zVec = xRot * zVec;
      position(zVec);
    } else {
      zVec = xRot * zVec;
    }
  }

  end();

  // Y Rotation
  if (mCurrentRotateType == RotateType_Y) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Green", Ogre::RenderOperation::OT_LINE_LIST);
  }

  Ogre::Vector3 xVec = Ogre::Vector3::UNIT_X * size;
  Ogre::Quaternion yRot(Ogre::Degree(10), Ogre::Vector3::UNIT_Y);

  for (int i = 0; i < 36; ++i) {
    if (cameraVec.directionEquals(xVec, Ogre::Degree(89.9f))) {
      position(xVec);
      xVec = yRot * xVec;
      position(xVec);
    } else {
      xVec = yRot * xVec;
    }
  }

  end();

  // Z Rotation
  if (mCurrentRotateType == RotateType_Z) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Blue", Ogre::RenderOperation::OT_LINE_LIST);
  }

  Ogre::Vector3 yVec = Ogre::Vector3::UNIT_Y * size;
  Ogre::Quaternion zRot(Ogre::Degree(10), Ogre::Vector3::UNIT_Z);

  for (int i = 0; i < 36; ++i) {
    if (cameraVec.directionEquals(yVec, Ogre::Degree(89.9f))) {
      position(yVec);
      yVec = zRot * yVec;
      position(yVec);
    } else {
      yVec = zRot * yVec;
    }
  }

  end();
}

Ogre::String RotateGizmoFactory::FACTORY_TYPE_NAME = "RotateGizmo";

const Ogre::String &RotateGizmoFactory::getType() const {
  return FACTORY_TYPE_NAME;
}

void RotateGizmoFactory::destroyInstance(Ogre::MovableObject *obj) {
  delete obj;
}

Ogre::MovableObject *
RotateGizmoFactory::createInstanceImpl(const Ogre::String &name,
                                       const Ogre::NameValuePairList *) {
  return new RotateGizmo(name);
}
