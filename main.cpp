#include <crypto/crypto.hpp>

#include <app/Kernel.hpp>
#include <gui/MainWindow.hpp>
#include <nostr/NostrProfile.hpp>

#include <QApplication>
#include <QTimer>

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

  //Mine a pubkey starting with bartoli
  //QString start;
  //qlonglong attempts = 0;
  //do
  //{
  //  QByteArray keypair = randomSecret();
  //  QByteArray privkey = extractPrivateKey(keypair); //256bit
  //  QByteArray pubkey_hex = compressedPublicKey(keypair);//256+1bits
  //  QByteArray npub = toBech32(QByteArray("npub"), QByteArray::fromHex(pubkey_hex));
  //  start = npub.mid(6, 4);
  //  ++attempts;
  //} while (start != "an0n");


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
