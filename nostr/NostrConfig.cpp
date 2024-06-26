#include <nostr/NostrConfig.hpp>

#include <QSettings>

void NostrConfig::init()
{
    relays["relay.damus.io"] = NostrRelayConfig{true, "Damus"};
}

bool NostrConfig::fromSettings(QSettings& settings)
{
  if (!settings.contains("nostr"))
    return true;

  settings.beginGroup("nostr");
  if(settings.contains("relays"))
  {
    settings.beginGroup("relays");
    for (const auto & key : settings.childKeys())
    {
      settings.beginGroup(key);
      NostrRelayConfig c;
      c.enabled = settings.value("enabled", QVariant(false)).toBool();
      c.name = settings.value("name", key).toString();
      relays.insert(key, c);
      settings.endGroup();
    }
    settings.endGroup();
    autoConnect = settings.value("autoConnect", QVariant(false)).toBool();
  }
  settings.endGroup();

  return true;
}
