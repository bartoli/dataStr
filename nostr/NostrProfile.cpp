#include <nostr/NostrProfile.hpp>
#include <crypto/crypto.hpp>

NProfile NProfile::fromBech32(const QByteArray& nprofile)
{
  NProfile ret;

  typedef enum
  {
    SPECIAL = 0,
    RELAY = 1,
    AUTHOR = 2,
    KIND = 3
  } TLVType;


  QByteArray hex = ::fromBech32(nprofile);
  QByteArray bin = QByteArray::fromHex(hex);

  auto extract_tlv = [&ret](const QByteArray& buf, int start)
  {
    uint8_t type = buf[start];
    uint8_t length = buf[start + 1];
    QByteArray value;
    value.resize(length + ((type != SPECIAL) ? 1 : 0));
    memcpy(value.data(), buf.constData() + start + 2, length);
    if (type == SPECIAL)
      ret.pubkey_hex = value.toHex();
    else if (type == RELAY)
    {
      value[length] = '\0';
      ret.relays.append(value);
    }
    else
      return -1;
    return start + 2 + length;
  };

  //extract tlvs one by one
  int pos = 0;
  do
  {
    pos = extract_tlv(bin, pos);
  } while (pos >= 0 && pos < bin.length());
  return ret;
}
QByteArray NProfile::toBech32() const
{
  //TODO (perf) : fill data as binary directly instead of hex that is then converted to binary

  QByteArray data;
  int out_size = 2 + 2 + 64;
  for (auto& relay : relays)
  {
    out_size += (2 + 2 + relay.length() * 2);
  }
  data.reserve(out_size);

  data.append("00");
  data.append("20");
  data.append(pubkey_hex);
  int next_pos = 2 + 2 + 64;
  for (auto& relay : relays)
  {
    data.append("01");
    data.append(QByteArray(1, char(relay.length())).toHex());
    data.append(QByteArray(relay.toUtf8()).toHex());
  }

  return ::toBech32(QByteArray("nprofile"), QByteArray::fromHex(data));
}
