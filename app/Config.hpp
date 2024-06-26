#pragma once

#include <nostr/NostrConfig.hpp>

class Config
{
public:
	Config() = default;
	NostrConfig nostr;
	bool connectAtStartup = true;

	void init();
	void load();
};