#include "Application.hpp"
#include "MainView.hpp"
#include "SubView.hpp"

int main(int argc, char *argv[]) {
  Application a(argc, argv);

  MainView mainView;
  mainView.resize(1280, 960);
  mainView.show();

  SubView subView;
  subView.resize(1280 / 2, 960 / 2);
  subView.show();

  return a.exec();
}
