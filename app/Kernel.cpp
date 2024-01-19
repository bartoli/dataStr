#include <app/Kernel.hpp>

QScopedPointer<Kernel> Kernel::_instance;

Kernel::Kernel()
{
  _config.init();
  _config.load();
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