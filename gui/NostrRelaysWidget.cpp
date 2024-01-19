#include <gui/NostrRelaysWidget.hpp>
#include <app/Kernel.hpp>

#include <QTableWidget>
#include <QVBoxLayout>

NostrRelaysWidget::NostrRelaysWidget(QWidget* parent)
  :QWidget(parent)
{
  auto kernel = Kernel::getInstance();
  auto& relays = kernel->getConfig().nostr.relays;

  _tableWidget = new QTableWidget(relays.size(), 2, this);

  QVBoxLayout * layout = new QVBoxLayout();
  _tableWidget->resizeColumnsToContents();
  layout->addWidget(_tableWidget);
  setLayout(layout);
}
