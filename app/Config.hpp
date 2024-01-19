#pragma once

#include <nostr/NostrConfig.hpp>

using Config = struct
{
  NostrConfig nostr;
  bool connectAtStartup = true;

  void init();
  void load();
};