#include <app/Kernel.hpp>

QScopedPointer<Kernel> Kernel::_instance;

Kernel::Kernel()
 :_client(_config.nostr){
  //default config values
  _config.init();
  //load config files
  _config.load();

  //register relays
  QMapIterator<QString, NostrRelayConfig> it(_config.nostr.relays);
  while (it.hasNext())
  {
      auto* relay_con = _client.registerRelay(it.next().key());
  }

}

Kernel* Kernel::getInstance()
{
  if (_instance.isNull())
    _instance.reset(new Kernel());
  return _instance.data();
}

void Kernel::run()
{

}