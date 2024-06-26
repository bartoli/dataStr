#include <QByteArray>
#include <stdint.h>

int bech32_convert_bits(uint8_t* out, size_t* outlen, int outbits, const uint8_t* in, size_t inlen, int inbits, int pad) {
  uint32_t val = 0;
  int bits = 0;
  uint32_t maxv = (((uint32_t)1) << outbits) - 1;
  while (inlen--) {
    val = (val << inbits) | *(in++);
    bits += inbits;
    while (bits >= outbits) {
      bits -= outbits;
      out[(*outlen)++] = (val >> bits) & maxv;
    }
  }
  if (pad) {
    if (bits) {
      out[(*outlen)++] = (val << (outbits - bits)) & maxv;
    }
  }
  else if (((val << (outbits - bits)) & maxv) || bits >= inbits) {
    return 0;
  }
  return 1;
}

//bech32 code from https://github.com/ElementsProject/lightning/blob/master/common/bech32.c
//usage inspired from https://github.com/jeffthibault/python-nostr/blob/main/nostr/key.py

/** Supported encodings. */
typedef enum {
  BECH32_ENCODING_NONE,
  BECH32_ENCODING_BECH32,
  BECH32_ENCODING_BECH32M
} bech32_encoding;

static uint32_t bech32_polymod_step(uint32_t pre) {
  uint8_t b = pre >> 25;
  return ((pre & 0x1FFFFFF) << 5) ^
    (-((b >> 0) & 1) & 0x3b6a57b2UL) ^
    (-((b >> 1) & 1) & 0x26508e6dUL) ^
    (-((b >> 2) & 1) & 0x1ea119faUL) ^
    (-((b >> 3) & 1) & 0x3d4233ddUL) ^
    (-((b >> 4) & 1) & 0x2a1462b3UL);
}

const char bech32_charset[] = "qpzry9x8gf2tvdw0s3jn54khce6mua7l";

const int8_t bech32_charset_rev[128] = {
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
  15, -1, 10, 17, 21, 20, 26, 30,  7,  5, -1, -1, -1, -1, -1, -1,
  -1, 29, -1, 24, 13, 25,  9,  8, 23, -1, 18, 22, 31, 27, 19, -1,
  1,  0,  3, 16, 11, 28, 12, 14,  6,  4,  2, -1, -1, -1, -1, -1,
  -1, 29, -1, 24, 13, 25,  9,  8, 23, -1, 18, 22, 31, 27, 19, -1,
  1,  0,  3, 16, 11, 28, 12, 14,  6,  4,  2, -1, -1, -1, -1, -1
};

static uint32_t bech32_final_constant(bech32_encoding enc) {
  if (enc == BECH32_ENCODING_BECH32) return 1;
  if (enc == BECH32_ENCODING_BECH32M) return 0x2bc830a3;
  assert(0);
}

int bech32_encode(char *output, const char *hrp, const uint8_t *data, size_t data_len, size_t max_output_len, bech32_encoding enc) {
  uint32_t chk = 1;
  size_t i = 0;
  while (hrp[i] != 0) {
    int ch = hrp[i];
    if (ch < 33 || ch > 126) {
      return 0;
    }

    if (ch >= 'A' && ch <= 'Z') return 0;
    chk = bech32_polymod_step(chk) ^ (ch >> 5);
    ++i;
  }
  if (i + 7 + data_len > max_output_len) return 0;
  chk = bech32_polymod_step(chk);
  while (*hrp != 0) {
    chk = bech32_polymod_step(chk) ^ (*hrp & 0x1f);
    *(output++) = *(hrp++);
  }
  *(output++) = '1';
  for (i = 0; i < data_len; ++i) {
    if (*data >> 5) return 0;
    chk = bech32_polymod_step(chk) ^ (*data);
    *(output++) = bech32_charset[*(data++)];
  }
  for (i = 0; i < 6; ++i) {
    chk = bech32_polymod_step(chk);
  }
  chk ^= bech32_final_constant(enc);
  for (i = 0; i < 6; ++i) {
    *(output++) = bech32_charset[(chk >> ((5 - i) * 5)) & 0x1f];
  }
  *output = 0;
  return 1;
}

bech32_encoding bech32_decode(char* hrp, uint8_t *data, size_t *data_len, const char *input, size_t max_input_len) {
  uint32_t chk = 1;
  size_t i;
  size_t input_len = strlen(input);
  size_t hrp_len;
  int have_lower = 0, have_upper = 0;
  if (input_len < 8 || input_len > max_input_len) {
    return BECH32_ENCODING_NONE;
  }
  *data_len = 0;
  while (*data_len < input_len && input[(input_len - 1) - *data_len] != '1') {
    ++(*data_len);
  }
  hrp_len = input_len - (1 + *data_len);
  if (1 + *data_len >= input_len || *data_len < 6) {
    return BECH32_ENCODING_NONE;
  }
  *(data_len) -= 6;
  for (i = 0; i < hrp_len; ++i) {
    int ch = input[i];
    if (ch < 33 || ch > 126) {
      return BECH32_ENCODING_NONE;
    }
    if (ch >= 'a' && ch <= 'z') {
      have_lower = 1;
    }
    else if (ch >= 'A' && ch <= 'Z') {
      have_upper = 1;
      ch = (ch - 'A') + 'a';
    }
    hrp[i] = ch;
    chk = bech32_polymod_step(chk) ^ (ch >> 5);
  }
  hrp[i] = 0;
  chk = bech32_polymod_step(chk);
  for (i = 0; i < hrp_len; ++i) {
    chk = bech32_polymod_step(chk) ^ (input[i] & 0x1f);
  }
  ++i;
  while (i < input_len) {
    int v = (input[i] & 0x80) ? -1 : bech32_charset_rev[(int)input[i]];
    if (input[i] >= 'a' && input[i] <= 'z') have_lower = 1;
    if (input[i] >= 'A' && input[i] <= 'Z') have_upper = 1;
    if (v == -1) {
      return BECH32_ENCODING_NONE;
    }
    chk = bech32_polymod_step(chk) ^ v;
    if (i + 6 < input_len) {
      data[i - (1 + hrp_len)] = v;
    }
    ++i;
  }
  if (have_lower && have_upper) {
    return BECH32_ENCODING_NONE;
  }
  if (chk == bech32_final_constant(BECH32_ENCODING_BECH32)) {
    return BECH32_ENCODING_BECH32;
  }
  else if (chk == bech32_final_constant(BECH32_ENCODING_BECH32M)) {
    return BECH32_ENCODING_BECH32M;
  }
  else {
    return BECH32_ENCODING_NONE;
  }
}

QByteArray toBech32(const QByteArray& hrp, const QByteArray& bytes)
{
  QByteArray out;
  out.resize(128);
  size_t out_size = 0;
  bech32_convert_bits((uint8_t*)out.data(), &out_size, 5, (const uint8_t*)bytes.data(), bytes.size(), 8, 1);
  out.resize(out_size);

  QByteArray out2;
  out2.resize(256);
  out2.fill('\0');
  bech32_encode(out2.data(), hrp.data(), (const uint8_t*)out.data(), out.size(), 256, bech32_encoding::BECH32_ENCODING_BECH32);
  out2.resize(strlen(out2.data()));

  return out2;
}

QByteArray fromBech32(const QByteArray& bech32)
{
  QByteArray hrp, data;
  hrp.resize(128);
  hrp.fill('\0');
  data.resize(128);
  size_t data_size = 0;
  bech32_decode((char*)hrp.data(), (uint8_t*)data.data(), &data_size, bech32.data(), bech32.size());
  data.resize(data_size);

  QByteArray data2;
  data2.resize(128);
  data_size = 0;
  bech32_convert_bits((uint8_t*)data2.data(), &data_size, 8, (const uint8_t*)data.data(), data.size(), 5, 1);
  data2.resize(data_size);
  data2 = data2.toHex();
  while (data2.endsWith("00"))
    data2.resize(data2.size() - 2);
  return data2;
}