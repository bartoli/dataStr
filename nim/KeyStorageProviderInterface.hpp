#pragma once

#include <QString>

//
// Defines an interface to implement your own way of storing a keypair
//
// This is open source, so the way to store a private key can't be hidden.
// You have to rely on YOURSELF securing the folder where this is stored.
//
// This interface allows you to define, as a QT plugin, your own way to store your private keys
//
// built-in interfaces:
// - an interface that asks for a password and decrypts a file using that password
// - an command to launch your own executable/script
//

class KeyStorageProviderInterface
{
  //A key that identifies the interface implementation
  virtual QString key() const = 0;

  //Generates an ID to identify each stored keypair, in a format needed by the underlying system
  virtual QString newId() = 0;

  //configuration needed by the interface for where/how to fetch the private key
  //It should be a utf-8 encoded string containing 'something'
  virtual void setConfig(const QString& confString) = 0;

  //get a keypair for an ID, giving an optional 'thing' to authenticate.
  //Calling code should account for the fact that this can be blocking while user does 'some action' to authenticate
  virtual QByteArray getKeyPair(const QString& id, const QString& authString = QString()) = 0;

  // save a keypair with given id and name
  virtual bool saveKeyPair(const QString& id, const QString& name, const QByteArray& keyPair) = 0;
};