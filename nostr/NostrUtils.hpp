#ifndef NOSTRUTILS_HPP
#define NOSTRUTILS_HPP

#include <QString>

class NostrUtils
{
public:
    NostrUtils();


    // Identity utils
    static void randomIdentity(QByteArray& pub, QByteArray& priv);
};

class Identity{
public:
  QByteArray pubkey;
  QByteArray privkey;
};

#endif // NOSTRUTILS_HPP
