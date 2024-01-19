#pragma once

class Kernel;

#include <QMainWindow>


class MainWindow : public QMainWindow
{
  Q_DISABLE_COPY(MainWindow)

public:
  MainWindow(const Kernel&);

private:
  MainWindow();

  const Kernel& _kernel;

  QTabWidget* _tabWidget;
};
