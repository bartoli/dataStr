#include <crypto/crypto.hpp>

#include <app/Kernel.hpp>
#include <gui/MainWindow.hpp>

#include <QApplication>
#include <QTimer>

//#include "task.h"

class NProfile
{
public:
  QByteArray pubkey_hex;
  QStringList relays;
  static NProfile fromBech32(const QByteArray& nprofile);
  QByteArray toBech32() const;
};

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
    value.resize(length + ((type != SPECIAL)? 1:0));
    memcpy(value.data(), buf.constData() + start + 2, length);
    if (type == SPECIAL)
      ret.pubkey_hex = value.toHex();
    else if (type == RELAY)
    {
      value[length] = '\0';
      ret.relays.append(value);
    }
    else
      return - 1;
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
  int next_pos = 2+2+64;
  for (auto& relay : relays)
  {
    data.append("01");
    data.append(QByteArray(1, char(relay.length())).toHex());
    data.append(QByteArray(relay.toUtf8()).toHex());
  }

  return ::toBech32(QByteArray("nprofile"), QByteArray::fromHex(data));
}


int main(int argc, char *argv[])
{
  QCoreApplication::setOrganizationName("bartoli");
  QCoreApplication::setOrganizationDomain("lnshortcut.ovh");
  QCoreApplication::setApplicationName("disaStr");
  QCoreApplication::addLibraryPath("c:/sdk/polaris8/winnt64/apps/qt-5.15.2/plugins");
  QApplication app(argc, argv);

  QByteArray keypair = randomSecret();
  QByteArray privkey = extractPrivateKey(keypair); //256bit
  QByteArray pubkey_hex = compressedPublicKey(keypair);//256+1bits
  QByteArray npub = toBech32(QByteArray("npub"), QByteArray::fromHex(pubkey_hex));// QByteArray::fromHex("3bf0c63fcb93463407af97a5e5ee64fa883d107ef9e558472c4eb9aaaefa459d"));
  QByteArray keyhex = fromBech32(npub);

  //get privkey from a known nsec, then pubkey, then npub and nprofile from it
  //npub10elfcs4fr0l0r8af98jlmgdh9c8tcxjvz9qkw038js35mp4dma8qzvjptg  should give the pubkey 7e7e9c42a91bfef19fa929e5fda1b72e0ebc1a4c1141673e2794234d86addf4e
  auto pk = fromBech32("npub10elfcs4fr0l0r8af98jlmgdh9c8tcxjvz9qkw038js35mp4dma8qzvjptg");
  //nsec1vl029mgpspedva04g90vltkh6fvh240zqtv9k0t9af8935ke9laqsnlfe5  should gitve the pribkey 67dea2ed018072d675f5415ecfaed7d2597555e202d85b3d65ea4e58d2d92ffa
  pk = fromBech32("nsec1vl029mgpspedva04g90vltkh6fvh240zqtv9k0t9af8935ke9laqsnlfe5");
  //nprofile1qqsrhuxx8l9ex335q7he0f09aej04zpazpl0ne2cgukyawd24mayt8gpp4mhxue69uhhytnc9e3k7mgpz4mhxue69uhkg6nzv9ejuumpv34kytnrdaksjlyr9p : pubkey=3bf0c63fcb93463407af97a5e5ee64fa883d107ef9e558472c4eb9aaaefa459d, relay=wss://r.x.com, relay=wss://djbas.sadkb.com
  NProfile prof = NProfile::fromBech32("nprofile1qqsrhuxx8l9ex335q7he0f09aej04zpazpl0ne2cgukyawd24mayt8gpp4mhxue69uhhytnc9e3k7mgpz4mhxue69uhkg6nzv9ejuumpv34kytnrdaksjlyr9p");
  pk = prof.toBech32();


  Kernel* kernel = Kernel::getInstance();
  kernel->run();

  MainWindow mw(*kernel);
  mw.show();

  // Task parented to the application so that it
  // will be deleted by the application.
  //Task *task = new Task(&a);
  // This will cause the application to exit when
  // the task signals finished.
  //QObject::connect(task, SIGNAL(finished()), &a, SLOT(quit()));
  // This will run the task from the application event loop.
  //QTimer::singleShot(0, task, SLOT(run()));

  return app.exec();
}
