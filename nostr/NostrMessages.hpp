#ifndef NOSTRMESSAGES_HPP
#define NOSTRMESSAGES_HPP

#include <nostr/NostrUtils.hpp>

#include <QString>
class QByteArray;
class QJsonArray;
class QJsonObject;

//basic json encoding/decoding of messages.
//We don't handle them here, just extract info in a more easy to use way
class NostrMessages
{
public:
    NostrMessages();

    //Encode messages sent to a relay
    static QByteArray encode_req(const QString& subscriptionId, const QMap<QString,QStringList>& filters);
    static QByteArray encode_event(const Identity& author, int kind, QJsonArray& tags, const QString& contentJson);
    QByteArray encode_close();

    //Decode messages received from a relay
    void decode_event(const QByteArray&);
    void decode_ok(const QByteArray&);
    void decode_eose(const QByteArray&);
    void decode_notice(const QByteArray&);

    static QByteArray uniqueSubscriptionId();

};

#endif // NOSTRMESSAGES_HPP
