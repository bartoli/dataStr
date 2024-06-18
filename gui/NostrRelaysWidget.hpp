#pragma once

#include <QWidget>
#include <QWebSocket>
class QTableWidget;

class NostrRelaysWidget : public QWidget
{
  Q_DISABLE_COPY(NostrRelaysWidget)

public:
  NostrRelaysWidget(QWidget* parent);

  using ConnexionState = QAbstractSocket::SocketState;
private:
  QTableWidget* _tableWidget;
};