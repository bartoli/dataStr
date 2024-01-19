#include <app\Config.hpp>

#include <QSettings>

//init to defaults if there is no config to load
void Config::init()
{
  nostr.init();
}

//Try to load config from file
void Config::load()
{
  QSettings settings;
  settings.beginGroup("nostr");
  nostr.fromSettings(settings);
  settings.endGroup();
}