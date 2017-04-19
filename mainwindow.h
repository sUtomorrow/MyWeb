#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include "mtcpserver.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MTcpServer* tcpserver;
private:
    Ui::MainWindow *ui;
private slots:
    void startServer();
    void closeServer();
    void selectRootDir();
};

#endif // MAINWINDOW_H
