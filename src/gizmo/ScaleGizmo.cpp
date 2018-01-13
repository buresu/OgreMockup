#include "ScaleGizmo.hpp"
#include <OgreCamera.h>
#include <OgreNode.h>
#include <OgreRay.h>

#include <QDebug>

ScaleGizmo::ScaleGizmo(const Ogre::String &name) : AbstractGizmo(name) {
  render();
}

ScaleGizmo::~ScaleGizmo() {}

bool ScaleGizmo::mousePressed(const Ogre::Ray &ray) {

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

  // Coordinate Angles
  Ogre::Real angleX = Ogre::Math::Abs(
      cameraVec.angleBetween(Ogre::Vector3::UNIT_X).valueDegrees() - 90);
  Ogre::Real angleY = Ogre::Math::Abs(
      cameraVec.angleBetween(Ogre::Vector3::UNIT_Y).valueDegrees() - 90);
  Ogre::Real angleZ = Ogre::Math::Abs(
      cameraVec.angleBetween(Ogre::Vector3::UNIT_Z).valueDegrees() - 90);

  // X Coordinate
  if (angleY > angleZ) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Y, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - getParentNode()->getPosition();

      if (Ogre::Math::Abs(v.z) < size * Ogre::Real(0.1) && v.x > 0 &&
          v.x < size * Ogre::Real(1.2)) {
        mCurrentScaleType = ScaleType_X;
        mStartPosition = p;
      }
    }

  } else {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Z, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - getParentNode()->getPosition();

      if (Ogre::Math::Abs(v.y) < size * Ogre::Real(0.1) && v.x > 0 &&
          v.x < size * Ogre::Real(1.2)) {
        mCurrentScaleType = ScaleType_X;
        mStartPosition = p;
      }
    }
  }

  // Y Coordinate
  if (angleX > angleZ) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_X, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - getParentNode()->getPosition();

      if (Ogre::Math::Abs(v.z) < size * Ogre::Real(0.1) && v.y > 0 &&
          v.y < size * Ogre::Real(1.2)) {
        mCurrentScaleType = ScaleType_Y;
        mStartPosition = p;
      }
    }

  } else {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Z, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - getParentNode()->getPosition();

      if (Ogre::Math::Abs(v.x) < size * Ogre::Real(0.1) && v.y > 0 &&
          v.y < size * Ogre::Real(1.2)) {
        mCurrentScaleType = ScaleType_Y;
        mStartPosition = p;
      }
    }
  }

  // Z Coordinate
  if (angleX > angleY) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_X, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - getParentNode()->getPosition();

      if (Ogre::Math::Abs(v.y) < size * Ogre::Real(0.1) && v.z > 0 &&
          v.z < size * Ogre::Real(1.2)) {
        mCurrentScaleType = ScaleType_Z;
        mStartPosition = p;
      }
    }

  } else {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Y, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 v = p - getParentNode()->getPosition();

      if (Ogre::Math::Abs(v.x) < size * Ogre::Real(0.1) && v.z > 0 &&
          v.z < size * Ogre::Real(1.2)) {
        mCurrentScaleType = ScaleType_Z;
        mStartPosition = p;
      }
    }
  }

  if (mCurrentScaleType != ScaleType_None) {
    mStartScale = mTargetNode->getScale();
    render();
    return true;
  }

  return false;
}

bool ScaleGizmo::mouseMoved(const Ogre::Ray &ray) {

  if (mCurrentScaleType == ScaleType_None || !isVisible()) {
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

  // Coordinate Angles
  Ogre::Real angleX = Ogre::Math::Abs(
      cameraVec.angleBetween(Ogre::Vector3::UNIT_X).valueDegrees() - 90);
  Ogre::Real angleY = Ogre::Math::Abs(
      cameraVec.angleBetween(Ogre::Vector3::UNIT_Y).valueDegrees() - 90);
  Ogre::Real angleZ = Ogre::Math::Abs(
      cameraVec.angleBetween(Ogre::Vector3::UNIT_Z).valueDegrees() - 90);

  if (mCurrentScaleType == ScaleType_X && angleY > angleZ) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Y, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 sv = mStartPosition - getParentNode()->getPosition();
      Ogre::Vector3 cv = p - getParentNode()->getPosition();

      mTargetNode->setScale(mStartScale);
      mTargetNode->scale(cv.length() / sv.length(), 1, 1);
    }

  } else if (mCurrentScaleType == ScaleType_X && angleZ > angleY) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Z, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 sv = mStartPosition - getParentNode()->getPosition();
      Ogre::Vector3 cv = p - getParentNode()->getPosition();

      mTargetNode->setScale(mStartScale);
      mTargetNode->scale(cv.length() / sv.length(), 1, 1);
    }

  } else if (mCurrentScaleType == ScaleType_Y && angleX > angleZ) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_X, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 sv = mStartPosition - getParentNode()->getPosition();
      Ogre::Vector3 cv = p - getParentNode()->getPosition();

      mTargetNode->setScale(mStartScale);
      mTargetNode->scale(1, cv.length() / sv.length(), 1);
    }

  } else if (mCurrentScaleType == ScaleType_Y && angleZ > angleX) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Z, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 sv = mStartPosition - getParentNode()->getPosition();
      Ogre::Vector3 cv = p - getParentNode()->getPosition();

      mTargetNode->setScale(mStartScale);
      mTargetNode->scale(1, cv.length() / sv.length(), 1);
    }

  } else if (mCurrentScaleType == ScaleType_Z && angleX > angleY) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_X, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 sv = mStartPosition - getParentNode()->getPosition();
      Ogre::Vector3 cv = p - getParentNode()->getPosition();

      mTargetNode->setScale(mStartScale);
      mTargetNode->scale(1, 1, cv.length() / sv.length());
    }

  } else if (mCurrentScaleType == ScaleType_Z && angleY > angleX) {

    auto r = ray.intersects(
        Ogre::Plane(Ogre::Vector3::UNIT_Y, getParentNode()->getPosition()));

    if (r.first) {

      Ogre::Vector3 p = ray.getPoint(r.second);
      Ogre::Vector3 sv = mStartPosition - getParentNode()->getPosition();
      Ogre::Vector3 cv = p - getParentNode()->getPosition();

      mTargetNode->setScale(mStartScale);
      mTargetNode->scale(1, 1, cv.length() / sv.length());
    }
  }

  return true;
}

bool ScaleGizmo::mouseReleased(const Ogre::Ray &) {

  if (mCurrentScaleType == ScaleType_None || !isVisible()) {
    return false;
  }

  mCurrentScaleType = ScaleType_None;
  mStartScale = Ogre::Vector3::UNIT_SCALE;

  render();

  return true;
}

void ScaleGizmo::render() {

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

  Ogre::Vector3 yVec1 = Ogre::Vector3::UNIT_Y * size * 0.1f;
  Ogre::Quaternion zRot1(Ogre::Degree(10), Ogre::Vector3::UNIT_Z);

  for (int i = 0; i < 36; ++i) {
    position(cameraRot * yVec1);
    yVec1 = zRot1 * yVec1;
    position(cameraRot * yVec1);
  }

  end();

  if (mCurrentScaleType == ScaleType_X) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Red", Ogre::RenderOperation::OT_LINE_LIST);
  }

  position(size * 0.1f, 0, 0);
  position(size, 0, 0);

  end();

  if (mCurrentScaleType == ScaleType_Y) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Green", Ogre::RenderOperation::OT_LINE_LIST);
  }

  position(0, size * 0.1f, 0);
  position(0, size, 0);

  end();

  if (mCurrentScaleType == ScaleType_Z) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_LINE_LIST);
  } else {
    begin("Gizmo/Blue", Ogre::RenderOperation::OT_LINE_LIST);
  }

  position(0, 0, size * 0.1f);
  position(0, 0, size);

  end();

  if (mCurrentScaleType == ScaleType_X) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  } else {
    begin("Gizmo/Red", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  }

  Ogre::Vector3 xBox(size, 0, 0);
  float boxSize = 0.05f * size;

  position(xBox.x - boxSize, xBox.y + boxSize, xBox.z + boxSize);
  position(xBox.x + boxSize, xBox.y + boxSize, xBox.z + boxSize);
  position(xBox.x + boxSize, xBox.y - boxSize, xBox.z + boxSize);
  position(xBox.x - boxSize, xBox.y - boxSize, xBox.z + boxSize);
  position(xBox.x - boxSize, xBox.y + boxSize, xBox.z - boxSize);
  position(xBox.x + boxSize, xBox.y + boxSize, xBox.z - boxSize);
  position(xBox.x + boxSize, xBox.y - boxSize, xBox.z - boxSize);
  position(xBox.x - boxSize, xBox.y - boxSize, xBox.z - boxSize);

  quad(0, 3, 2, 1);
  quad(4, 5, 6, 7);
  quad(0, 1, 5, 4);
  quad(1, 2, 6, 5);
  quad(0, 4, 7, 3);
  quad(3, 7, 6, 2);

  end();

  if (mCurrentScaleType == ScaleType_Y) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  } else {
    begin("Gizmo/Green", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  }

  Ogre::Vector3 yBox(0, size, 0);

  position(yBox.x - boxSize, yBox.y + boxSize, yBox.z + boxSize);
  position(yBox.x + boxSize, yBox.y + boxSize, yBox.z + boxSize);
  position(yBox.x + boxSize, yBox.y - boxSize, yBox.z + boxSize);
  position(yBox.x - boxSize, yBox.y - boxSize, yBox.z + boxSize);
  position(yBox.x - boxSize, yBox.y + boxSize, yBox.z - boxSize);
  position(yBox.x + boxSize, yBox.y + boxSize, yBox.z - boxSize);
  position(yBox.x + boxSize, yBox.y - boxSize, yBox.z - boxSize);
  position(yBox.x - boxSize, yBox.y - boxSize, yBox.z - boxSize);

  quad(0, 3, 2, 1);
  quad(4, 5, 6, 7);
  quad(0, 1, 5, 4);
  quad(1, 2, 6, 5);
  quad(0, 4, 7, 3);
  quad(3, 7, 6, 2);

  end();

  if (mCurrentScaleType == ScaleType_Z) {
    begin("Gizmo/Yellow", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  } else {
    begin("Gizmo/Blue", Ogre::RenderOperation::OT_TRIANGLE_LIST);
  }

  Ogre::Vector3 zBox(0, 0, size);

  position(zBox.x - boxSize, zBox.y + boxSize, zBox.z + boxSize);
  position(zBox.x + boxSize, zBox.y + boxSize, zBox.z + boxSize);
  position(zBox.x + boxSize, zBox.y - boxSize, zBox.z + boxSize);
  position(zBox.x - boxSize, zBox.y - boxSize, zBox.z + boxSize);
  position(zBox.x - boxSize, zBox.y + boxSize, zBox.z - boxSize);
  position(zBox.x + boxSize, zBox.y + boxSize, zBox.z - boxSize);
  position(zBox.x + boxSize, zBox.y - boxSize, zBox.z - boxSize);
  position(zBox.x - boxSize, zBox.y - boxSize, zBox.z - boxSize);

  quad(0, 3, 2, 1);
  quad(4, 5, 6, 7);
  quad(0, 1, 5, 4);
  quad(1, 2, 6, 5);
  quad(0, 4, 7, 3);
  quad(3, 7, 6, 2);

  end();
}

Ogre::String ScaleGizmoFactory::FACTORY_TYPE_NAME = "ScaleGizmo";

const Ogre::String &ScaleGizmoFactory::getType() const {
  return FACTORY_TYPE_NAME;
}

void ScaleGizmoFactory::destroyInstance(Ogre::MovableObject *obj) {
  delete obj;
}

Ogre::MovableObject *
ScaleGizmoFactory::createInstanceImpl(const Ogre::String &name,
                                      const Ogre::NameValuePairList *) {
  return new ScaleGizmo(name);
}
