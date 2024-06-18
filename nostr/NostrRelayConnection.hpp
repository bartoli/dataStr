#ifndef NOSTRRELAYCONNECTION_H
#define NOSTRRELAYCONNECTION_H

#include <QString>
#include <QWebSocket>

class NostrClient;

class Subscription
{
  using MessageStore = QMultiMap<time_t, QString>;
  using MessageHandler = std::function<void(const QString&)>;

  void registerMessage(time_t date, const QString& created_at);
  void clearResults();

  MessageStore& getMessages(); //not threadsafe!

  //run a lambda to work on the results whole holding the mutex for thread safety
  void run(std::function<void(const MessageStore&)> work);
  //pass a lambda to handle each new message received
  void setMessageHandler(MessageHandler*);

  enum Status
  {
    NEW,
    REGISTERED,
    STOPPED
  } _status;

private:
  //received and kept messages, ordered by event date
  MessageStore _messages;
  MessageHandler* _handler;
};

class NostrRelayConnection : public QObject
{
  Q_OBJECT
  Q_DISABLE_COPY(NostrRelayConnection)
public:
  NostrRelayConnection(/*const NostrClient& client, */const QString& address);
  bool startConnection(bool wait = false);
  bool isConnected() const;

private:
  //const NostrClient& _client;
  const QString _address;
  int failed_conn_attempts = 0;
  QWebSocket _socket;
  QMap<QString, Subscription> _subscriptions;
};

#endif // NOSTRRELAYCONNECTION_H
