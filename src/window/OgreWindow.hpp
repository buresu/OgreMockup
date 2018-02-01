#pragma once

#include <OgreFrameListener.h>
#include <QWindow>

class OgreWindow : public QWindow, public Ogre::FrameListener {
  Q_OBJECT
public:
  explicit OgreWindow(QWindow *parent = nullptr);
  ~OgreWindow();

  virtual void render(QPainter *) {}
  virtual void render();

  virtual void renderLater();
  virtual void renderNow();

  Ogre::RenderWindow *renderWindow() const { return mRenderWindow; }

protected:
  virtual bool eventFilter(QObject *target, QEvent *event) override;

  virtual void initialize();

  bool mAnimating = false;
  Ogre::RenderWindow *mRenderWindow = nullptr;
};
