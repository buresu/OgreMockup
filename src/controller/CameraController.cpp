#include "CameraController.hpp"

#include <QDebug>

CameraController::CameraController(Ogre::SceneNode *node) {
  setCameraNode(node);
  setCameraStyle(mCameraStyle);
}

CameraController::~CameraController() {}

void CameraController::setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch,
                                       Ogre::Real dist) {

  OgreAssert(mTargetNode, "No target set");

  mOffset = Ogre::Vector3::ZERO;
  mCameraNode->setPosition(mTargetNode->_getDerivedPosition());
  mCameraNode->setOrientation(mTargetNode->_getDerivedOrientation());
  mCameraNode->yaw(yaw);
  mCameraNode->pitch(-pitch);
  mCameraNode->translate(Ogre::Vector3(0, 0, dist), Ogre::Node::TS_LOCAL);
}

void CameraController::setCameraStyle(CameraStyle style) {

  if (mCameraStyle != CS_ORBIT && style == CS_ORBIT) {

    setTargetNode(mTargetNode ? mTargetNode
                              : mCameraNode->getCreator()->getRootSceneNode());
    mCameraNode->setFixedYawAxis(true);
    manualStop();

    Ogre::Real dist = getDistToTarget();
    const Ogre::Quaternion &q = mCameraNode->getOrientation();
    setYawPitchDist(q.getYaw(), q.getPitch(), dist == 0 ? 150 : dist);

  } else if (mCameraStyle != CS_FREELOOK && style == CS_FREELOOK) {
    mCameraNode->setFixedYawAxis(true);
  } else if (mCameraStyle != CS_MANUAL && style == CS_MANUAL) {
    manualStop();
  }

  mCameraStyle = style;
  mCameraNode->setAutoTracking(false);
}

void CameraController::manualStop() {

  if (mCameraStyle == CS_FREELOOK) {
    mGoingForward = false;
    mGoingBack = false;
    mGoingLeft = false;
    mGoingRight = false;
    mGoingUp = false;
    mGoingDown = false;
    mVelocity = Ogre::Vector3::ZERO;
  }
}

void CameraController::frameRendered(const Ogre::FrameEvent &event) {

  if (mCameraStyle == CS_FREELOOK) {

    Ogre::Vector3 accel = Ogre::Vector3::ZERO;
    Ogre::Matrix3 axes = mCameraNode->getLocalAxes();

    if (mGoingForward) {
      accel -= axes.GetColumn(2);
    }

    if (mGoingBack) {
      accel += axes.GetColumn(2);
    }

    if (mGoingRight) {
      accel += axes.GetColumn(0);
    }

    if (mGoingLeft) {
      accel -= axes.GetColumn(0);
    }

    if (mGoingUp) {
      accel += axes.GetColumn(1);
    }

    if (mGoingDown) {
      accel -= axes.GetColumn(1);
    }

    Ogre::Real topSpeed = mFastMove ? mTopSpeed * 20 : mTopSpeed;

    if (accel.squaredLength() != 0.f) {
      accel.normalise();
      mVelocity += accel * topSpeed * event.timeSinceLastFrame * 10;
    } else {
      mVelocity -= mVelocity * event.timeSinceLastFrame * 10;
    }

    Ogre::Real tooSmall = std::numeric_limits<Ogre::Real>::epsilon();

    if (mVelocity.squaredLength() > topSpeed * topSpeed) {
      mVelocity.normalise();
      mVelocity *= topSpeed;
    } else if (mVelocity.squaredLength() < tooSmall * tooSmall) {
      mVelocity = Ogre::Vector3::ZERO;
    }

    if (mVelocity != Ogre::Vector3::ZERO) {
      mCameraNode->translate(mVelocity * event.timeSinceLastFrame);
    }
  }
}

void CameraController::keyPressed(QKeyEvent *event) {

  if (mCameraStyle == CS_FREELOOK) {
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
      mGoingForward = true;
    } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
      mGoingBack = true;
    } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
      mGoingLeft = true;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
      mGoingRight = true;
    } else if (event->key() == Qt::Key_PageUp) {
      mGoingUp = true;
    } else if (event->key() == Qt::Key_PageDown) {
      mGoingDown = true;
    } else if (event->key() == Qt::Key_Shift) {
      mFastMove = true;
    }
  }
}

void CameraController::keyReleased(QKeyEvent *event) {

  if (mCameraStyle == CS_FREELOOK) {
    if (event->key() == Qt::Key_W || event->key() == Qt::Key_Up) {
      mGoingForward = false;
    } else if (event->key() == Qt::Key_S || event->key() == Qt::Key_Down) {
      mGoingBack = false;
    } else if (event->key() == Qt::Key_A || event->key() == Qt::Key_Left) {
      mGoingLeft = false;
    } else if (event->key() == Qt::Key_D || event->key() == Qt::Key_Right) {
      mGoingRight = false;
    } else if (event->key() == Qt::Key_PageUp) {
      mGoingUp = false;
    } else if (event->key() == Qt::Key_PageDown) {
      mGoingDown = false;
    } else if (event->key() == Qt::Key_Shift) {
      mFastMove = false;
    }
  }
}

void CameraController::mouseWheelRolled(QWheelEvent *event) {

  int relZ = event->delta();
  if (mCameraStyle == CS_ORBIT && relZ != 0) {
    Ogre::Real dist =
        (mCameraNode->getPosition() - mTargetNode->_getDerivedPosition())
            .length();
    mCameraNode->translate(Ogre::Vector3(0, 0, -relZ * 0.0008f * dist),
                           Ogre::Node::TS_LOCAL);
  }
}

void CameraController::mousePressed(QMouseEvent *event) {

  if (mCameraStyle == CS_ORBIT) {
    if (event->buttons() & Qt::LeftButton) {
      mOrbiting = true;
    } else if (event->buttons() & Qt::RightButton) {
      mMoving = true;
    }
  }
}

void CameraController::mouseMoved(QMouseEvent *event) {

  int relX = 0;
  int relY = 0;

  if (!mLastPos.isNull()) {
    relX = event->x() - mLastPos.x();
    relY = event->y() - mLastPos.y();
  }

  mLastPos = event->pos();

  if (mCameraStyle == CS_ORBIT) {

    Ogre::Real dist = getDistToTarget();

    if (mOrbiting) {

      mCameraNode->setPosition(mTargetNode->_getDerivedPosition() + mOffset);

      mCameraNode->yaw(Ogre::Degree(-relX * 0.25f), Ogre::Node::TS_PARENT);
      mCameraNode->pitch(Ogre::Degree(-relY * 0.25f));

      mCameraNode->translate(Ogre::Vector3(0, 0, dist), Ogre::Node::TS_LOCAL);

    } else if (mMoving) {

      Ogre::Vector3 delta =
          mCameraNode->getOrientation() * Ogre::Vector3(-relX, relY, 0);
      delta *= dist / 1000.0f;
      mOffset += delta;
      mCameraNode->translate(delta);
    }

  } else if (mCameraStyle == CS_FREELOOK) {
    mCameraNode->yaw(Ogre::Degree(-relX * 0.15f), Ogre::Node::TS_PARENT);
    mCameraNode->pitch(Ogre::Degree(-relY * 0.15f));
  }
}

void CameraController::mouseReleased(QMouseEvent *event) {

  mLastPos = QPoint();

  if (mCameraStyle == CS_ORBIT) {
    if (event->buttons() & Qt::LeftButton) {
      mOrbiting = false;
    } else if (event->buttons() & Qt::RightButton) {
      mMoving = false;
    }
  }
}

Ogre::Real CameraController::getDistToTarget() const {
  Ogre::Vector3 offset =
      mCameraNode->getPosition() - mTargetNode->_getDerivedPosition() - mOffset;
  return offset.length();
}
