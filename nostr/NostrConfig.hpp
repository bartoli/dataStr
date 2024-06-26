#pragma once

#include <QMap>
#include <QString>
class QSettings;

class NostrRelayConfig
{
public:
	NostrRelayConfig() = default;
	bool enabled = false;;
	QString name;

	bool fromSettings(QSettings&);
	void toSettings(QSettings&) const;
};

class NostrConfig
{
	Q_DISABLE_COPY(NostrConfig)
public:
	NostrConfig() = default;
	//key is url
	using RelayConfigMap = QMap<QString, NostrRelayConfig>;
	RelayConfigMap relays;
	bool autoConnect = false;

	void init();
	bool fromSettings(QSettings&);
	void toSettings(QSettings&) const;
};