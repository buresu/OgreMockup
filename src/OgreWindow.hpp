#pragma once

#include <OgreFrameListener.h>
#include <QWindow>

class CameraController;
class TranslateGizmo;
class RotateGizmo;
class ScaleGizmo;

class OgreWindow : public QWindow, public Ogre::FrameListener {
  Q_OBJECT
public:
  explicit OgreWindow(QWindow *parent = nullptr);
  ~OgreWindow();

  virtual void render(QPainter *) {}
  virtual void render();
  virtual void initialize();
  virtual void createScene();

  virtual void renderLater();
  virtual void renderNow();

  virtual bool eventFilter(QObject *target, QEvent *event) override;

signals:
  void entitySelected(Ogre::Entity *entity);

protected:
  virtual void keyPressEvent(QKeyEvent *event) override;
  virtual void keyReleaseEvent(QKeyEvent *event) override;
  virtual void wheelEvent(QWheelEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void exposeEvent(QExposeEvent *event) override;

  virtual bool frameRenderingQueued(const Ogre::FrameEvent &event) override;

  void log(Ogre::String msg);
  void log(QString msg);

  bool mAnimating = false;

  Ogre::Root *mOgreRoot = nullptr;
  Ogre::RenderWindow *mOgreWindow = nullptr;
  Ogre::SceneManager *mOgreSceneManager = nullptr;
  Ogre::Camera *mOgreCamera = nullptr;
  CameraController *mCameraController = nullptr;

  // Gizmo
  Ogre::Node *mGizmoTargetNode = nullptr;
  TranslateGizmo *mTranslateGizmo = nullptr;
  RotateGizmo *mRotateGizmo = nullptr;
  ScaleGizmo *mScaleGizmo = nullptr;
};
