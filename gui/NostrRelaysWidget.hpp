#pragma once

#include <QWidget>
#include <QWebSocket>
class QTableWidget;

class NostrRelaysWidget : public QWidget
{
	Q_OBJECT
		Q_DISABLE_COPY(NostrRelaysWidget)

public:
	NostrRelaysWidget(QWidget* parent);

	using ConnexionState = QAbstractSocket::SocketState;
private:
	void _updateConnState(const QString& key, const QAbstractSocket::SocketState state);
	QTableWidget* _tableWidget;
};