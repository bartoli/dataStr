#include <crypto/crypto.hpp>

#include <app/Kernel.hpp>
#include <gui/MainWindow.hpp>

#include <QApplication>
#include <QTimer>

//#include "task.h"


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
