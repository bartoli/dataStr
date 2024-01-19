
#include <openssl/ec.h>
#include <openssl/ecdh.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/pem.h>

#include <secp256k1_extrakeys.h>
#include <secp256k1_schnorrsig.h>

#include <QDebug>
#include <QString>
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

QByteArray randomSecret()
{
  EC_KEY* keyPair = EC_KEY_new_by_curve_name(NID_secp256k1);
  if (!keyPair)
  {
    qDebug() << "Failed to generate SECP key pair.";
    return QByteArray();
  }
  if (EC_KEY_generate_key(keyPair) != 1)
  {
    qDebug() << "Failed to generate SECP key pair.";
    EC_KEY_free(keyPair);
    return QByteArray();
  }
  BIO* privateKeyBio = BIO_new(BIO_s_mem());
  if (!privateKeyBio)
  {
    qDebug() << "Failed to create BIO for private key.";
    return QByteArray();
  }
  if (!PEM_write_bio_ECPrivateKey(privateKeyBio, keyPair, nullptr, nullptr, 0, nullptr, nullptr))
  {
    qDebug() << "Failed to write private key to BIO.";
    BIO_free(privateKeyBio);
    return QByteArray();
  }
  BUF_MEM* privateKeyMem;
  BIO_get_mem_ptr(privateKeyBio, &privateKeyMem);
  std::string privateKeyStr(privateKeyMem->data, privateKeyMem->length);
  // Clean up
  BIO_free_all(privateKeyBio);
  EC_KEY_free(keyPair);
  auto list = QByteArray::fromStdString(privateKeyStr).split('\n');
  list.removeAll(QByteArray());
  return list.mid(1, list.size() - 2).join(QByteArray());
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

QByteArray extractPrivateKey(const QByteArray& base64SecretKey)
{
  // Decode the base64-encoded secret key
  auto decodedKey = QByteArray::fromBase64(base64SecretKey).toStdString();

  // Read the private key from the DER data
  const unsigned char* key_data = reinterpret_cast<const unsigned char*>(decodedKey.c_str());
  auto evp_key = d2i_PrivateKey(EVP_PKEY_EC, nullptr, &key_data, decodedKey.size());

  if (!evp_key)
  {
    qDebug() << "Error reading private key";
    return "";
  }

  // Extract the EC_KEY structure
  auto ec_key = EVP_PKEY_get1_EC_KEY(evp_key);
  if (!ec_key)
  {
    qDebug() << "Error getting EC_KEY from EVP_PKEY";
    EVP_PKEY_free(evp_key);
    return "";
  }

  // Extract the private key in its binary form (32 bytes)
  const BIGNUM* private_key_bn = EC_KEY_get0_private_key(ec_key);
  unsigned char private_key_bytes[32];
  int private_key_size = BN_bn2bin(private_key_bn, private_key_bytes);

  // Convert the private key to hex-encoded std::string
  std::string private_key_hex;
  for (int i = 0; i < private_key_size; i++)
  {
    char hex[3];
    snprintf(hex, sizeof(hex), "%02x", private_key_bytes[i]);
    private_key_hex += hex;
  }

  // Clean up
  EC_KEY_free(ec_key);
  EVP_PKEY_free(evp_key);

  return QByteArray::fromStdString(private_key_hex);
}

QByteArray compressedPublicKey(const QString &secretKey)
{
  using namespace std::string_literals;
  const std::string qnostr_privateKeyHeader = "-----BEGIN EC PRIVATE KEY-----\n"s;
  const std::string qnostr_privateKeyFooter = "\n-----END EC PRIVATE KEY-----\n"s;

  const std::string key = qnostr_privateKeyHeader + secretKey.toStdString() + qnostr_privateKeyFooter;
  auto privateKeyBio = BIO_new_mem_buf(key.c_str(), -1);
  if (!privateKeyBio)
  {
    qDebug() << "Failed to create BIO for private key.";
    return QByteArray();
  }

  auto ecKey = PEM_read_bio_ECPrivateKey(privateKeyBio, nullptr, nullptr, nullptr);
  if (!ecKey)
  {
    qDebug() << "Failed to read private key from BIO.";
    BIO_free(privateKeyBio);
    return QByteArray();
  }

  const EC_GROUP* group = EC_KEY_get0_group(ecKey);
  const EC_POINT* point = EC_KEY_get0_public_key(ecKey);

  size_t len = EC_POINT_point2oct(group, point, POINT_CONVERSION_COMPRESSED, nullptr, 0, nullptr);
  std::string compressedKey(len, '\0');

  if (EC_POINT_point2oct(group, point, POINT_CONVERSION_COMPRESSED, (unsigned char*)compressedKey.data(), len, nullptr) != len)
  {
    qDebug() << "Failed to compress public key.";
    return "";
  }

  // Clean up
  EC_KEY_free(ecKey);

  return QByteArray::fromStdString(compressedKey).toHex();
}

QByteArray sign(const QByteArray &data, const QByteArray &privateKey)
{
  auto ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  secp256k1_keypair keypair;
  if (secp256k1_keypair_create(ctx, &keypair, (const unsigned char *)privateKey.data()) != 1)
  {
    secp256k1_context_destroy(ctx);
    return QByteArray();
  }
  QByteArray signature;
  signature.resize(64);
  if (secp256k1_schnorrsig_sign32(ctx, (unsigned char*)signature.data(), (const unsigned char *)data.data(), &keypair, nullptr) != 1)
  {
    secp256k1_context_destroy(ctx);
    return QByteArray();
  }
  secp256k1_context_destroy(ctx);
  return QByteArray((const char*)signature.data(), signature.size());
}

int check_sig(const QByteArray& message, const QByteArray& signedMessage, const QByteArray& pubkey)
{
  auto ctx = secp256k1_context_create(SECP256K1_CONTEXT_NONE);
  secp256k1_xonly_pubkey pub_compressed;
  secp256k1_xonly_pubkey_parse(ctx, &pub_compressed, (const unsigned char*)pubkey.constData());
  int ret = secp256k1_schnorrsig_verify(ctx, (const unsigned char*)signedMessage.constData(), (const unsigned char*)message.constData(), message.size(), &pub_compressed);
  secp256k1_context_destroy(ctx);
  return ret;
}