#pragma once

#include <QStringList>
#include <QByteArray>

class NProfile
{
public:
  QByteArray pubkey_hex;
  QStringList relays;
  static NProfile fromBech32(const QByteArray& nprofile);
  QByteArray toBech32() const;
};
