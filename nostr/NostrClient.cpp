#include <nostr/NostrClient.hpp>
#include <nostr/NostrRelayConnection.hpp>
#include <nostr/NostrUtils.hpp>

NostrClient::NostrClient(NostrConfig& config)
 :_config(config){

  for (auto& addr : config.relays.keys())
    registerRelay(addr);
}

NostrClient::~NostrClient()
{
    qDeleteAll(_relays);
}

NostrRelayConnection* NostrClient::registerRelay(const QString& address)
{
    //Fix input
    //Remove extra spaces
    QString fixed_address = address.trimmed();
    //We will add wss:// later
    if(fixed_address.startsWith("wss://"))
        fixed_address = fixed_address.mid(6);

    RelayMap::iterator it = _relays.find(fixed_address);
    if(it != _relays.end())
        return it.value();

    //Create an instance to communicate with the relay
    NostrRelayConnection* relay = new NostrRelayConnection(fixed_address);
    it = _relays.insert(fixed_address, relay);
    if(_autoConnect)
        it.value()->startConnection();
    return it.value();
}

bool NostrClient::connectAllRelays(bool wait)
{
  _autoConnect = true;

  //Just return whether we are already connected or not to some relays
  for (const auto & relay : _relays)
  {
      if(relay->isConnected())
          return true;
  }
  return false;

  if(wait)
    {/*TODO*/}
}
