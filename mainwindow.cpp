#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->CloseServiceBtn->setEnabled(false);
    ui->RootDirEdit->setEnabled(false);
    this->setFixedSize(511,649);
    connect(ui->StartServiceBtn,SIGNAL(clicked()),this,SLOT(startServer()));
    connect(ui->CloseServiceBtn,SIGNAL(clicked()),this,SLOT(closeServer()));
    connect(ui->SelectRootDirBtn,SIGNAL(clicked()),this,SLOT(selectRootDir()));
}

MainWindow::~MainWindow(){
    delete ui;
}
/**
 * @brief MainWindow::startServer 启动服务器按钮对应操作
 */
void MainWindow::startServer(){
    tcpserver = new MTcpServer(ui->ThreadNumEdit->text().toInt(),ui->RootDirEdit->text());
    connect(tcpserver,SIGNAL(mPassDetailToWindow(QString,int,int,QString,QString)),this,SLOT(showRunningInfo(QString,int,int,QString,QString)));
    QString Addr = QString("%1.%2.%3.%4").arg(ui->IPEdit1->text()).arg(ui->IPEdit2->text()).arg(ui->IPEdit3->text()).arg(ui->IPEdit4->text());
    this->tcpserver->listen(QHostAddress(Addr),ui->ListenPortEdit->text().toInt());
    ui->CloseServiceBtn->setEnabled(true);
    ui->StartServiceBtn->setEnabled(false);
    ui->SelectRootDirBtn->setEnabled(false);
    ui->IPEdit1->setEnabled(false);
    ui->IPEdit2->setEnabled(false);
    ui->IPEdit3->setEnabled(false);
    ui->IPEdit4->setEnabled(false);
    ui->ThreadNumEdit->setEnabled(false);
    ui->ListenPortEdit->setEnabled(false);
//    connect();
}
/**
 * @brief MainWindow::closeServer 关闭服务器按钮对应操作
 */
void MainWindow::closeServer(){
    this->tcpserver->close();
    delete this->tcpserver;
    ui->StartServiceBtn->setEnabled(true);
    ui->CloseServiceBtn->setEnabled(false);
    ui->SelectRootDirBtn->setEnabled(true);
    ui->IPEdit1->setEnabled(true);
    ui->IPEdit2->setEnabled(true);
    ui->IPEdit3->setEnabled(true);
    ui->IPEdit4->setEnabled(true);
    ui->ThreadNumEdit->setEnabled(true);
    ui->ListenPortEdit->setEnabled(true);
}

void MainWindow::selectRootDir(){
    QDir curRootDir;
    if(!curRootDir.exists(ui->RootDirEdit->text())){
        curRootDir.mkdir(ui->RootDirEdit->text());
    }
    QString rootPath = QFileDialog::getExistingDirectory(this,QString("选择网站根目录"),ui->RootDirEdit->text());
    if(!rootPath.isEmpty())
        ui->RootDirEdit->setText(rootPath);
}

void MainWindow::showRunningInfo(QString reqIp, int reqPost, int threadId, QString requestHttp, QString responseHttp){
    QString text = QString("-----------------------------\n"
                           "请求ip:%1\n"
                           "请求源端口:%2\n"
                           "处理请求的线程号:%3\n"
                           "请求Http报文:\n%4"
                           "响应Http报文:\n%5\n").arg(reqIp).arg(reqPost).arg(threadId).arg(requestHttp).arg(responseHttp);
    ui->LogEdit->setText(text+ui->LogEdit->toPlainText());
}
