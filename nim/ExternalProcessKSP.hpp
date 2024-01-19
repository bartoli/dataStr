#pragma once

#include <nim/KeyStorageProviderInterface.hpp>

class ExternalProcessKSP : public KeyStorageProviderInterface
{
  virtual ~ExternalProcessKSP() = default;
};
