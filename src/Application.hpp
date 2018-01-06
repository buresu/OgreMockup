#pragma once

#include <QApplication>

class Application final : public QApplication {
public:
  Application(int &argc, char **argv);
  ~Application();
};
