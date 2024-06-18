#include <gui/NostrRelaysWidget.hpp>
#include <app/Kernel.hpp>

#include <QTableWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QHeaderView>

NostrRelaysWidget::NostrRelaysWidget(QWidget* parent)
  :QWidget(parent)
{
  auto kernel = Kernel::getInstance();
  const auto& relays = kernel->getConfig().nostr.relays;

  _tableWidget = new QTableWidget(relays.size(), 4, this);
  QMapIterator<QString, NostrRelayConfig> it(relays);
  _tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem());
  _tableWidget->horizontalHeaderItem(0)->setText("Name");
  _tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem());
  _tableWidget->horizontalHeaderItem(1)->setText("URL");
  _tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem());
  _tableWidget->horizontalHeaderItem(2)->setText("Status");
  _tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem());
  _tableWidget->horizontalHeaderItem(3)->setText("");

  int row = 0;
  while(it.hasNext())
  {
    it.next();
    _tableWidget->insertRow(row);
    auto& relay(it.value());
    auto col0 = new QTableWidgetItem();
    col0->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    col0->setCheckState(relay.enabled? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    col0->setText(relay.name);
    _tableWidget->setItem(row, 0, col0);
    auto col1 = new QTableWidgetItem();
    col1->setFlags(Qt::ItemFlag::NoItemFlags);
    col1->setText(it.key());
    _tableWidget->setItem(row, 1, col1);
    QPushButton* connectBtn = new QPushButton("Connect", _tableWidget);
    _tableWidget->setCellWidget(row, 3, connectBtn);



    ++row;
  }

  QVBoxLayout * layout = new QVBoxLayout();
  _tableWidget->resizeColumnsToContents();
  layout->addWidget(_tableWidget);
  setLayout(layout);
}
