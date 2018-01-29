#include "Application.hpp"
#include "controller/CameraController.hpp"
#include "gizmo/RotateGizmo.hpp"
#include "gizmo/ScaleGizmo.hpp"
#include "gizmo/TranslateGizmo.hpp"
#include "object/ActivePointObject.hpp"
#include "object/AxisGridLineObject.hpp"
#include "object/CameraObject.hpp"
#include <OgreRoot.h>
#include <OgreTextureManager.h>
#include <QMessageBox>
#include <QStandardPaths>
#include <RenderSystems/GL/OgreGLPlugin.h>
#include <RenderSystems/GL3Plus/OgreGL3PlusPlugin.h>

#include <OgreConfigFile.h>
#include <QDebug>

Application::Application(int &argc, char **argv) : QApplication(argc, argv) {

  // High Dpi Scaling for windows
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  // Ogre Initialize
  Ogre::Root *root = new Ogre::Root(Ogre::BLANKSTRING, Ogre::BLANKSTRING, Ogre::BLANKSTRING);

#if defined(Q_OS_WIN)
  root->loadPlugin("RenderSystem_GL");
  root->loadPlugin("RenderSystem_GL3Plus");
#else
  root->installPlugin(new Ogre::GLPlugin);
  root->installPlugin(new Ogre::GL3PlusPlugin);
#endif

  root->addMovableObjectFactory(new AxisGridLineObjectFactory);
  root->addMovableObjectFactory(new ActivePointObjectFactory);
  root->addMovableObjectFactory(new CameraObjectFactory);
  root->addMovableObjectFactory(new TranslateGizmoFactory);
  root->addMovableObjectFactory(new RotateGizmoFactory);
  root->addMovableObjectFactory(new ScaleGizmoFactory);

  auto resLocation =
      QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation)
          .back();

  // Qt 5.9+ Bug...
  resLocation.replace(".app", ".app/");

  Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
      resLocation.toStdString(), "FileSystem",
      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);

  const Ogre::RenderSystemList &rsList = root->getAvailableRenderers();
  Ogre::RenderSystem *rs = rsList[0];

  Ogre::StringVector renderOrder;
  renderOrder.push_back("OpenGL");
  renderOrder.push_back("OpenGL 3+");

  for (auto itr = renderOrder.begin(); itr != renderOrder.end(); itr++) {
    for (auto it = rsList.begin(); it != rsList.end(); it++) {
      if ((*it)->getName().find(*itr) != Ogre::String::npos) {
        rs = *it;
        break;
      }
    }
    if (rs != nullptr)
      break;
  }

  if (rs == nullptr) {
    QMessageBox::critical(nullptr, "Error", "RenderSystem is not found");
    quit();
  }

  rs->setConfigOption("Full Screen", "No");
  root->setRenderSystem(rs);
  root->initialise(false);

  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(0);
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

Application::~Application() {}
