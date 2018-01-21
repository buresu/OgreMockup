#pragma once

#include "window/OgreWindow.hpp"

class CameraController;
class TranslateGizmo;
class RotateGizmo;
class ScaleGizmo;

class MainView : public OgreWindow {
  Q_OBJECT
public:
  explicit MainView(QWindow *parent = nullptr);
  ~MainView();

protected:
  virtual void keyPressEvent(QKeyEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;

  virtual bool frameRenderingQueued(const Ogre::FrameEvent &event) override;

  void createScene();

  void setGizmoTarget(Ogre::SceneNode *node);

  Ogre::SceneManager *mSceneManager = nullptr;
  Ogre::Camera *mCamera = nullptr;
  CameraController *mCameraController = nullptr;

  // Gizmo
  Ogre::Node *mGizmoTargetNode = nullptr;
  TranslateGizmo *mTranslateGizmo = nullptr;
  RotateGizmo *mRotateGizmo = nullptr;
  ScaleGizmo *mScaleGizmo = nullptr;
};
