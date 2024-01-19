#include <nostr/NostrMessages.hpp>
#include <crypto/crypto.hpp>

#include <QByteArray>
#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include <QString>
#include <QList>

#include <time.h>

NostrMessages::NostrMessages()
{

}

//Encode messages sent to a relay
//Client request. Embed
QByteArray NostrMessages::encode_req(const QString& subscriptionId, const QMap<QString,QStringList>& filters)
{
    //detect if a filter will be an array
    auto filter_is_array = [](const QString& name)
    {
        static const QStringList array_filters({"ids", "authors", "kinds"});
        if(array_filters.contains(name))
            return true;
        if(name.startsWith(('#') && name.length()==2)) //repalce by a regex because only[a-zA-Z] is valid?
            return true;
        return false;
    };
    auto filter_is_alpha = [](const QString& name)
    {
        static const QStringList int_filters{"kinds", "limit"};
        return !int_filters.contains(name);
    };

    QJsonObject filters_obj;
    QMapIterator<QString, QStringList> it(filters);
    while(it.hasNext()){
        it.next();
        const QString& filter_name = it.key();
        const QStringList& filter_values = it.value();
        if(filter_is_array(filter_name))
        {
            QJsonArray filter;
            for(const auto& value : filter_values)
                if(!filter_is_alpha(filter_name))
                    filter.append(value.toInt());
                else
                    filter.append(value);
            filters_obj.insert(filter_name, filter);
        }
        else
        {
            if(filter_is_alpha(filter_name))
              filters_obj.insert(filter_name, filter_values.first());
            else
              filters_obj.insert(filter_name, filter_values.first().toInt());
        }
    }

    QJsonArray req_obj;
    req_obj.append("REQ");
    req_obj.append(subscriptionId);
    req_obj.append(filters_obj);

    QJsonDocument doc;
    doc.setArray(req_obj);
    return doc.toJson(QJsonDocument::JsonFormat::Compact);
}

QByteArray NostrMessages::encode_event(const Identity& author, int kind, QJsonArray& tags, const QString& contentJson)
{
  //Generate part to serialize/hash first
  /*auto _pubkey = bech32Decode("npub10elfcs4fr0l0r8af98jlmgdh9c8tcxjvz9qkw038js35mp4dma8qzvjptg");*/
  //QString pubkey = "npub10elfcs4fr0l0r8af98jlmgdh9c8tcxjvz9qkw038js35mp4dma8qzvjptg";//QByteArray::fromRawData((const char*)_pubkey.second.data(), _pubkey.second.size());
  QString pubkey = fromBech32(author.pubkey);
  time_t created_at = time(NULL);
  QJsonArray msg1;
  msg1.append(0);
  msg1.append(pubkey);
  msg1.append(created_at);
  msg1.append(kind);
  msg1.append(tags);
  msg1.append(contentJson);
  QJsonDocument doc;
  doc.setArray(msg1);
  QByteArray json1 = doc.toJson(QJsonDocument::Compact);
  QByteArray id = QCryptographicHash::hash(json1, QCryptographicHash::Sha256);
  QByteArray sig = sign(id, QByteArray::fromHex(fromBech32(author.privkey))).toHex().toLower();
  int sig_check = check_sig(id, QByteArray::fromHex(sig), QByteArray::fromHex(fromBech32(author.pubkey)));

  //Final json
  QJsonArray msg2;
  msg2.append("EVENT");
  QJsonObject msg3;
  msg3.insert("id", QString(id.toHex().toLower()));
  msg3.insert("kind", kind);
  msg3.insert("pubkey", pubkey);
  msg3.insert("created_at", created_at);
  msg3.insert("tags", tags);
  msg3.insert("content", contentJson);
  msg3.insert("sig" , QString(sig));
  msg2.append(msg3);
  QJsonDocument doc2;
  doc2.setArray(msg2);
  return doc2.toJson(QJsonDocument::Compact);
}

QByteArray NostrMessages::encode_close()
{
  return QByteArray();
}

//Decode messages received from a relay
void NostrMessages::decode_event(const QByteArray&)
{}

void NostrMessages::decode_ok(const QByteArray&)
{}

void NostrMessages::decode_eose(const QByteArray&)
{}

void NostrMessages::decode_notice(const QByteArray&)
{}

QByteArray NostrMessages::uniqueSubscriptionId()
{
    static int generated_count=0;
    QByteArray input = (QString::number(time(NULL))+QString::number(++generated_count)).toLocal8Bit();

    //Sha1 is 160 bits long, so 40 chars as hex
    return QCryptographicHash::hash(input, QCryptographicHash::Sha1).toHex();
}
