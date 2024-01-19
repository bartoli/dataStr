#pragma once

#include <nostr/NostrClient.hpp>

class Context
{
public:
  Context() = default;


private:
  QMap<QString, NostrRelayConnection> _relays;

  //NostrClient nostrClient;
};