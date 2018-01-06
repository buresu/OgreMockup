#include "TranslateGizmo.hpp"
#include <OgreCamera.h>
#include <OgreNode.h>
#include <OgreRay.h>

TranslateGizmo::TranslateGizmo(const Ogre::String &name) : AbstractGizmo(name) {
  render();
}

TranslateGizmo::~TranslateGizmo() {}

bool TranslateGizmo::mousePressed(const Ogre::Ray &ray) {

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
  }

  // YZ Coordinate
  auto r = ray.intersects(
      Ogre::Plane(Ogre::Vector3::UNIT_X, getParentNode()->getPosition()));

  if (r.first) {

    Ogre::Vector3 p = ray.getPoint(r.second);
    Ogre::Vector3 v = p - getParentNode()->getPosition();

    if (Ogre::Math::Abs(v.z) < size * Ogre::Real(0.1) && v.y > 0 &&
        v.y < size * Ogre::Real(1.2)) {
      mCurrentAxisType = AxisType_X;
      mCurrentTranslateType = TranslateType_Y;
      mStartPosition = p;
    } else if (Ogre::Math::Abs(v.y) < size * Ogre::Real(0.1) && v.z > 0 &&
               v.z < size * Ogre::Real(1.2)) {
      mCurrentAxisType = AxisType_X;
      mCurrentTranslateType = TranslateType_Z;
      mStartPosition = p;
    }
  }

  // XZ Coordinate
  r = ray.intersects(
      Ogre::Plane(Ogre::Vector3::UNIT_Y, getParentNode()->getPosition()));

  if (r.first) {

    Ogre::Vector3 p = ray.getPoint(r.second);
    Ogre::Vector3 v = p - getParentNode()->getPosition();

    if (Ogre::Math::Abs(v.z) < size * Ogre::Real(0.1) && v.x > 0 &&
        v.x < size * Ogre::Real(1.2)) {
      mCurrentAxisType = AxisType_Y;
      mCurrentTranslateType = TranslateType_X;
      mStartPosition = p;
    } else if (Ogre::Math::Abs(v.x) < size * Ogre::Real(0.1) && v.z > 0 &&
               v.z < size * Ogre::Real(1.2)) {
      mCurrentAxisType = AxisType_Y;
      mCurrentTranslateType = TranslateType_Z;
      mStartPosition = p;
    }
  }

  // XY Coordinate
  r = ray.intersects(
      Ogre::Plane(Ogre::Vector3::UNIT_Z, getParentNode()->getPosition()));

  if (r.first) {

    Ogre::Vector3 p = ray.getPoint(r.second);
    Ogre::Vector3 v = p - getParentNode()->getPosition();

    if (Ogre::Math::Abs(v.y) < size * Ogre::Real(0.1) && v.x > 0 &&
        v.x < size * Ogre::Real(1.2)) {
      mCurrentAxisType = AxisType_Z;
      mCurrentTranslateType = TranslateType_X;
      mStartPosition = p;
    } else if (Ogre::Math::Abs(p.x) < size * Ogre::Real(0.1) && v.y > 0 &&
               v.y < size * Ogre::Real(1.2)) {
      mCurrentAxisType = AxisType_Z;
      mCurrentTranslateType = TranslateType_Y;
      mStartPosition = p;
    }
  }

  if (mCurrentTranslateType != TranslateType_None) {
    mTargetStart = mTargetNode->getPosition();
    render();
    return true;
  }

  return false;
}

bool TranslateGizmo::mouseMoved(const Ogre::Ray &ray) {

  if (mCurrentTranslateType == TranslateType_None || !isVisible()) {
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

  if (mCurrentTranslateType == TranslateType_X &&
      mCurrentAxisType == AxisType_Y) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Y, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - mStartPosition;

      getParentNode()->setPosition(mTargetStart);
      getParentNode()->translate(v.x, 0, 0);
      mTargetNode->setPosition(mTargetStart);
      mTargetNode->translate(v.x, 0, 0);
    }
  }

  if (mCurrentTranslateType == TranslateType_X &&
      mCurrentAxisType == AxisType_Z) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Z, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - mStartPosition;

      getParentNode()->setPosition(mTargetStart);
      getParentNode()->translate(v.x, 0, 0);
      mTargetNode->setPosition(mTargetStart);
      mTargetNode->translate(v.x, 0, 0);
    }
  }

  if (mCurrentTranslateType == TranslateType_Y &&
      mCurrentAxisType == AxisType_X) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_X, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - mStartPosition;

      getParentNode()->setPosition(mTargetStart);
      getParentNode()->translate(0, v.y, 0);
      mTargetNode->setPosition(mTargetStart);
      mTargetNode->translate(0, v.y, 0);
    }
  }

  if (mCurrentTranslateType == TranslateType_Y &&
      mCurrentAxisType == AxisType_Z) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Z, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - mStartPosition;

      getParentNode()->setPosition(mTargetStart);
      getParentNode()->translate(0, v.y, 0);
      mTargetNode->setPosition(mTargetStart);
      mTargetNode->translate(0, v.y, 0);
    }
  }

  if (mCurrentTranslateType == TranslateType_Z &&
      mCurrentAxisType == AxisType_X) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_X, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - mStartPosition;

      getParentNode()->setPosition(mTargetStart);
      getParentNode()->translate(0, 0, v.z);
      mTargetNode->setPosition(mTargetStart);
      mTargetNode->translate(0, 0, v.z);
    }
  }

  if (mCurrentTranslateType == TranslateType_Z &&
      mCurrentAxisType == AxisType_Y) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Y, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - mStartPosition;

      getParentNode()->setPosition(mTargetStart);
      getParentNode()->translate(0, 0, v.z);
      mTargetNode->setPosition(mTargetStart);
      mTargetNode->translate(0, 0, v.z);
    }
  }

  return true;
}

bool TranslateGizmo::mouseReleased(const Ogre::Ray &) {

  if (mCurrentTranslateType == TranslateType_None || !isVisible()) {
    return false;
  }

  mCurrentAxisType = AxisType_None;
  mCurrentTranslateType = TranslateType_None;
  mStartPosition = Ogre::Vector3::ZERO;

  mTargetStart = Ogre::Vector3::ZERO;

  render();

  return true;
}

void TranslateGizmo::render() {

  Ogre::Real size = 1.0;
  Ogre::Vector3 cameraVec = Ogre::Vector3(0, 0, 1);

  if (mCurrentCamera) {

    Ogre::Vector3 camPos = mCurrentCamera->getDerivedPosition();
    Ogre::Vector3 objPos = getParentNode()->getPosition();

    size = camPos.distance(objPos) * 0.1f;

    cameraVec = camPos - objPos;
    cameraVec.normalise();
  }

  clear();

  Ogre::Vector3 zVec1(0, 0, 1);
  Ogre::Quaternion cameraRot = zVec1.getRotationTo(cameraVec);

  begin("Gizmo/White", Ogre::RenderOperation::OT_LINE_LIST);

  Ogre::Vector3 yVec1 = Ogre::Vector3(0, 1, 0) * size * 0.1f;
  Ogre::Quaternion zRot1(Ogre::Degree(10), Ogre::Vector3(0, 0, 1));

  for (int i = 0; i < 36; ++i) {
    position(cameraRot * yVec1);
    yVec1 = zRot1 * yVec1;
    position(cameraRot * yVec1);
  }

  end();

  if (mCurrentTranslateType == TranslateType_X) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Red", Ogre::RenderOperation::OT_LINE_LIST);
  }

  position(size * 0.1f, 0, 0);
  position(size, 0, 0);

  end();

  if (mCurrentTranslateType == TranslateType_Y) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Green", Ogre::RenderOperation::OT_LINE_LIST);
  }

  position(0, size * 0.1f, 0);
  position(0, size, 0);

  end();

  if (mCurrentTranslateType == TranslateType_Z) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Blue", Ogre::RenderOperation::OT_LINE_LIST);
  }

  position(0, 0, size * 0.1f);
  position(0, 0, size);

  end();

  if (mCurrentTranslateType == TranslateType_X) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  } else {
    begin("Gizmo/Red", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  }

  int vertex = 16;
  float radius = 0.05f * size;
  float height = 0.2f * size;

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

    position(size - height, 0, 0);
    position(size - height, p2x, p2y);
    position(size - height, p1x, p1y);
    position(size, 0, 0);
    position(size - height, p1x, p1y);
    position(size - height, p2x, p2y);
  }

  end();

  if (mCurrentTranslateType == TranslateType_Y) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  } else {
    begin("Gizmo/Green", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  }

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

    position(0, size - height, 0);
    position(p2y, size - height, p2x);
    position(p1y, size - height, p1x);
    position(0, size, 0);
    position(p1y, size - height, p1x);
    position(p2y, size - height, p2x);
  }

  end();

  if (mCurrentTranslateType == TranslateType_Z) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  } else {
    begin("Gizmo/Blue", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  }

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

    position(0, 0, size - height);
    position(p2x, p2y, size - height);
    position(p1x, p1y, size - height);
    position(0, 0, size);
    position(p1x, p1y, size - height);
    position(p2x, p2y, size - height);
  }

  end();
}

Ogre::String TranslateGizmoFactory::FACTORY_TYPE_NAME = "TranslateGizmo";

const Ogre::String &TranslateGizmoFactory::getType() const {
  return FACTORY_TYPE_NAME;
}

void TranslateGizmoFactory::destroyInstance(Ogre::MovableObject *obj) {
  delete obj;
}

Ogre::MovableObject *
TranslateGizmoFactory::createInstanceImpl(const Ogre::String &name,
                                          const Ogre::NameValuePairList *) {
  return new TranslateGizmo(name);
}
