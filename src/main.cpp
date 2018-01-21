#include "Application.hpp"
#include "window/OgreWindow.hpp"

int main(int argc, char *argv[]) {
  Application a(argc, argv);

  OgreWindow w;
  w.show();

  return a.exec();
}
