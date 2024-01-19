#pragma once

#include <app/Config.hpp>
#include <app/Context.hpp>
class Identity;

#include <QThread>

class Kernel : public QThread
{
public:
  static Kernel* getInstance();
  void run() override;

  void setNostrIdentity(const Identity&);

  Config& getConfig() { return _config; }
  Context& getContext() { return _context; }

private:
  Kernel();

  static QScopedPointer<Kernel> _instance;
  Config _config;
  Context _context;
};
