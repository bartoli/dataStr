#include <nostr/NostrRelayConnection.hpp>
#include <nostr/NostrMessages.hpp>
#include <nostr/NostrClient.hpp>

#include <QJsonArray>
#include <QJsonObject>

#define MAX_CONN_FAIL 3

NostrRelayConnection::NostrRelayConnection(const NostrClient&client, const QString& address)
    :_client(client), _address(address)
{
    connect(&_socket, &QWebSocket::connected, [this](){
        qDebug() << "Connected to "<<_address;

        QByteArray hello_event = NostrMessages::encode_event(_client.identity(), 1, QJsonArray(), "disaStr test");
        qDebug() << "Sending: " << hello_event;
        _socket.sendTextMessage(hello_event);

        //query for some message
        QByteArray req = NostrMessages::encode_req(NostrMessages::uniqueSubscriptionId(),
                                                   QMap<QString, QStringList>{
                                                       { "kinds", {"1"}},
                                                       { "limit", {"10"}}
                                                   });
        qDebug() << "Sending: " << req;
        _socket.sendTextMessage(req);
    });
    connect(&_socket, &QWebSocket::disconnected, [this](){
        qDebug() << "Disonnected from "<<_address;
    });
    connect(&_socket, &QWebSocket::stateChanged, [this](QAbstractSocket::SocketState state){
        qDebug() << state;
    });
    connect(&_socket, &QWebSocket::textMessageReceived, [this](const QString& message){
        qDebug()<<"Received Message: "<<message;
    });
    connect(&_socket, &QWebSocket::bytesWritten, [this](qint64 bytes){
        qDebug() << bytes << " bytes written";
    });
}

bool NostrRelayConnection::startConnection(bool wait)
{
    const QUrl url{"wss://"+_address};
    _socket.open(url);
    if(wait)
    {
        //wait for at least MAX_CONN_FAIL connection failures
    }
    return true;
}

bool NostrRelayConnection::isConnected() const
{
    const auto state =_socket.state();
    return state == QAbstractSocket::ConnectedState;
}
