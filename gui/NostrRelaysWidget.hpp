#pragma once

#include <QWidget>
class QTableWidget;

class NostrRelaysWidget : public QWidget
{
  Q_DISABLE_COPY(NostrRelaysWidget)

public:
  NostrRelaysWidget(QWidget* parent);
private:
  QTableWidget* _tableWidget;
};