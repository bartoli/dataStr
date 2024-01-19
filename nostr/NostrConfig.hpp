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

using NostrConfig = struct
{
  //key is url
  QMap<QString, NostrRelayConfig> relays;

  void init();
  bool fromSettings(QSettings&);
  void toSettings(QSettings&) const;
};