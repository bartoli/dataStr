#pragma once

#include <QMap>
#include <QString>
class QSettings;

using NostrRelayConfig = struct
{
  bool enabled;
  QString name;

  bool fromSettings(QSettings&);
  void toSettings(QSettings&) const;
};

class NostrConfig
{
  //key is url
  QMap<QString, NostrRelayConfig> relays;
  bool autoConnect = false;

  void init();
  bool fromSettings(QSettings&);
  void toSettings(QSettings&) const;
};