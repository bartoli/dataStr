#include <nostr/NostrUtils.hpp>
#include <crypto/crypto.hpp>

#include <QDebug>

#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/pem.h>

NostrUtils::NostrUtils()
{

}


void NostrUtils::randomIdentity(QByteArray& pub, QByteArray& priv)
{
    QString secret = randomSecret();
    pub = compressedPublicKey(secret);
    priv = extractPrivateKey(secret.toLatin1());
}
