#ifndef NOSTRCLIENT_HPP
#define NOSTRCLIENT_HPP

#include <nostr/NostrRelayConnection.hpp>
#include <nostr/NostrUtils.hpp>

#include <QMap>
#include <QString>
#include <QThread>

class NostrClient : public QThread
{
public:
    NostrClient(const Identity& id);
    ~NostrClient();

    void registerRelay(const QString& address);
    bool connectRelays(bool wait = false);
    const Identity& identity() const { return _identity; }

private:
    NostrClient() = delete;

    const Identity _identity;
    QMap<QString, NostrRelayConnection*> _relays;

    //Indicates if we already asked to connect to relays.
    //used to enable automatic connect to new added relays after we started connecting
    bool _connectionInitiated = false;
};

#endif // NOSTRCLIENT_HPP
