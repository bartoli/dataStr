#include <nostr/NostrClient.hpp>
#include <nostr/NostrRelayConnection.hpp>
#include <nostr/NostrUtils.hpp>

NostrClient::NostrClient(const Identity& id)
    :_identity(id)
{}

NostrClient::~NostrClient()
{
    qDeleteAll(_relays);
}

void NostrClient::registerRelay(const QString& address)
{
    //Fix input
    //Remove extra spaces
    QString fixed_address = address.trimmed();
    //We will add wss:// later
    if(fixed_address.startsWith("wss://"))
        fixed_address = fixed_address.mid(6);

    if(_relays.contains(fixed_address))
        return;

    //Create an instance to communicate with the relay
    NostrRelayConnection* relay = new NostrRelayConnection(*this, fixed_address);
    auto it = _relays.insert(fixed_address, relay);
    if(_connectionInitiated)
        it.value()->startConnection();
}

bool NostrClient::connectRelays(bool wait)
{
    if (_connectionInitiated)
    {
        //Just return whether we are already connected or not to some relays
        for (const auto & relay : _relays)
        {
            if(relay->isConnected())
                return true;
        }
        return false;
    }

    _connectionInitiated = true;
    for (auto & relay : _relays)
    {
        auto start_connection = [&relay, &wait]()
        {
            bool success = relay->startConnection(wait);
        };
        start_connection();
    }

    if(wait)
    {}

    return true;
}
