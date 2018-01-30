#pragma once

#include <OgreCamera.h>
#include <OgreFrameListener.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <QKeyEvent>
#include <QMouseEvent>

class CameraController {
public:
  enum CameraStyle { CS_FREELOOK, CS_ORBIT, CS_MANUAL };

  explicit CameraController(Ogre::SceneNode *node);
  virtual ~CameraController();

  void setCameraNode(Ogre::SceneNode *node) { mCameraNode = node; }
  Ogre::SceneNode *getCameraNode() const { return mCameraNode; }

  void setTargetNode(Ogre::SceneNode *node) { mTargetNode = node; }
  Ogre::SceneNode *getTarget() { return mTargetNode; }

  void setYawPitchDist(Ogre::Radian yaw, Ogre::Radian pitch, Ogre::Real dist);

  void setTopSpeed(Ogre::Real topSpeed) { mTopSpeed = topSpeed; }
  Ogre::Real getTopSpeed() const { return mTopSpeed; }

  void setCameraStyle(CameraStyle style);
  CameraStyle getCameraStyle() { return mCameraStyle; }

  void manualStop();

  void frameRendered(const Ogre::FrameEvent &event);

  void keyPressed(QKeyEvent *event);
  void keyReleased(QKeyEvent *event);

  void mouseWheelRolled(QWheelEvent *event);
  void mousePressed(QMouseEvent *event);
  void mouseMoved(QMouseEvent *event);
  void mouseReleased(QMouseEvent *event);

protected:
  Ogre::Real getDistToTarget() const;

  CameraStyle mCameraStyle = CS_FREELOOK;
  Ogre::SceneNode *mCameraNode = nullptr;
  Ogre::SceneNode *mTargetNode = nullptr;
  bool mOrbiting = false;
  bool mMoving = false;
  Ogre::Real mTopSpeed = 150;
  Ogre::Vector3 mVelocity = Ogre::Vector3::ZERO;
  bool mGoingForward = false;
  bool mGoingBack = false;
  bool mGoingLeft = false;
  bool mGoingRight = false;
  bool mGoingUp = false;
  bool mGoingDown = false;
  bool mFastMove = false;
  Ogre::Vector3 mOffset = Ogre::Vector3::ZERO;
  QPoint mLastPos;
};
