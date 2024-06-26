#pragma once

#include <QByteArray>

// SECP256K1 utils
//generates a seck256k1 key pair using OpenSSL, base64-encoded
QByteArray randomSecret();
//generates a secp25k1 private key from a secret, as a hexstring
QByteArray extractPrivateKey(const QByteArray& base64SecretKey); //out = 32byte ptiv key as hex string
//generate the main compressed public key hexstring for a private key hexstring
QByteArray compressedPublicKey(const QString &secretKey); //out = 32-byte+1bit pubkey as hex string
//signed a message (bytes) using a private key (bytes)
QByteArray sign(const QByteArray &data, const QByteArray &privateKey);
//Check that a signed message (bytes) corresponds to a clear message (bytes) signed by the owner of a given pubkey (bytes)
int check_sig(const QByteArray& message, const QByteArray& signedMessage, const QByteArray& pubkey);


// BECH32 utils
//generate a bech32 adress with a given hrp, and a key (binary bytes)
QByteArray toBech32(const QByteArray& hrp, const QByteArray& bytes);
//extract address hexstring from a bech32 adress
QByteArray fromBech32(const QByteArray& bech32);