#ifndef NOSTREVENT_HPP
#define NOSTREVENT_HPP

#include <stdint.h>
#include <QMap>
#include <QString>

/*
{
  "id": <32-bytes lowercase hex-encoded sha256 of the serialized event data>,
  "pubkey": <32-bytes lowercase hex-encoded public key of the event creator>,
  "created_at": <unix timestamp in seconds>,
  "kind": <integer between 0 and 65535>,
  "tags": [
    [<arbitrary string>...],
    ...
  ],
  "content": <arbitrary string>,
  "sig": <64-bytes lowercase hex of the signature of the sha256 hash of the serialized event data, which is the same as the "id" field>
}
*/

class NostrEvent
{
public:
    NostrEvent();
    QString id;
    uint16_t kind;
    int64_t created_at;
    QMap<QString, QStringList> tags;
    QString sig;
};

#endif // NOSTREVENT_HPP
