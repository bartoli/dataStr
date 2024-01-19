#include <gui/MainWindow.hpp>
#include <gui/NostrRelaysWidget.hpp>
#include <app/Kernel.hpp>

#include <QLayout>

MainWindow::MainWindow(const Kernel& kernel)
 : QMainWindow()
 , _kernel(kernel)
{
  //Build the tabs
  _tabWidget = new QTabWidget(this);
  _tabWidget->addTab(new NostrRelaysWidget(this), "Nostr relays");

  //Page layout
  QWidget *central_widget = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(_tabWidget);
  central_widget->setLayout(layout);
  setCentralWidget(central_widget);
}
