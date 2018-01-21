#include "Application.hpp"
#include "MainView.hpp"

int main(int argc, char *argv[]) {
  Application a(argc, argv);

  MainView mainView;
  mainView.show();

  return a.exec();
}
