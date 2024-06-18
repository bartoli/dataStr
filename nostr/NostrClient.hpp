#ifndef NOSTRCLIENT_HPP
#define NOSTRCLIENT_HPP

#include <nostr/NostrConfig.hpp>
#include <nostr/NostrRelayConnection.hpp>
#include <nostr/NostrUtils.hpp>

#include <QMap>
#include <QString>
#include <QThread>

class NostrClient : public QThread
{
public:
    NostrClient(NostrConfig& config);
    ~NostrClient();

    NostrRelayConnection* registerRelay(const QString& address);
    bool connectAllRelays(bool wait = false);
    //const Identity& identity() const { return _identity; }

private:
    //const Identity _identity;
    using RelayMap = QMap<QString, NostrRelayConnection*>;
    RelayMap _relays;
    NostrConfig& _config;

    //Indicates if we already asked to connect to relays.
    //used to enable automatic connect to new added relays after we started connecting
    bool _autoConnect = false;
};

#endif // NOSTRCLIENT_HPP
