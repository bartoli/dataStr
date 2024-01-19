#pragma once

#include <nim/KeyStorageProviderInterface.hpp>

class PasswordProtectedKSP : public KeyStorageProviderInterface
{
  virtual ~PasswordProtectedKSP() = default;
};
