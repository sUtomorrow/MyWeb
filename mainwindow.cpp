#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->CloseServiceBtn->setEnabled(false);
    connect(ui->StartServiceBtn,SIGNAL(clicked()),this,SLOT(startServer()));
    connect(ui->CloseServiceBtn,SIGNAL(clicked()),this,SLOT(closeServer()));
}

MainWindow::~MainWindow(){
    delete ui;
}
/**
 * @brief MainWindow::startServer 启动服务器按钮对应操作
 */
void MainWindow::startServer(){
    tcpserver = new MTcpServer(ui->ThreadNumEdit->text().toInt());
    QString Addr = QString("%1.%2.%3.%4").arg(ui->IPEdit1->text()).arg(ui->IPEdit2->text()).arg(ui->IPEdit3->text()).arg(ui->IPEdit4->text());
    this->tcpserver->listen(QHostAddress(Addr),ui->ListenPortEdit->text().toInt());
    ui->CloseServiceBtn->setEnabled(true);
    ui->StartServiceBtn->setEnabled(false);
    ui->IPEdit1->setEnabled(false);
    ui->IPEdit2->setEnabled(false);
    ui->IPEdit3->setEnabled(false);
    ui->IPEdit4->setEnabled(false);
    ui->ThreadNumEdit->setEnabled(false);
    ui->ListenPortEdit->setEnabled(false);
    ui->RootDirEdit->setEnabled(false);
}
/**
 * @brief MainWindow::closeServer 关闭服务器按钮对应操作
 */
void MainWindow::closeServer(){
    this->tcpserver->close();
    delete this->tcpserver;
    ui->StartServiceBtn->setEnabled(true);
    ui->CloseServiceBtn->setEnabled(false);
    ui->IPEdit1->setEnabled(true);
    ui->IPEdit2->setEnabled(true);
    ui->IPEdit3->setEnabled(true);
    ui->IPEdit4->setEnabled(true);
    ui->ThreadNumEdit->setEnabled(true);
    ui->ListenPortEdit->setEnabled(true);
    ui->RootDirEdit->setEnabled(true);
}


