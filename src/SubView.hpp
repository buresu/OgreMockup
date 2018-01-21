#pragma once

#include "window/OgreWindow.hpp"

class SubView : public OgreWindow {
  Q_OBJECT
public:
  explicit SubView(QWindow *parent = nullptr);
  ~SubView();
};
