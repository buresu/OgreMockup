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

protected:
  virtual bool eventFilter(QObject *target, QEvent *event) override;

  virtual void initialize();

  void log(const Ogre::String &msg);
  void log(const QString &msg);

  bool mAnimating = false;
  Ogre::RenderWindow *mRenderWindow = nullptr;
};
