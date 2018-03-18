#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QDebug>
//#include <QSerialPort>
#include <QTextCodec>
#include "serialportrun.h"
#include <QThread>
#include <QEventLoop>

//#define DEBUG 1

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //QTextCodec *tc = QTextCodec::codecForName("GBK");//编码格式

public slots:
    void MyRead(QByteArray);//定义槽函数
    void MyOpened(bool);

private slots:
    void on_pushButton_open_clicked();
    void on_pushButton_close_clicked();
    void on_pushButton_send_clicked();
    void on_pushButton_clicked();
    void on_textBrowserRefesh();

signals:
    void MySend(QByteArray);
    void MyOpen(QString , int , int , int , int);
    void opened();
   void MyClose();

private:
    Ui::MainWindow *ui;
    SerialPortRun *mSerialPortRun;
    QThread *mThread ;
    bool openFlag;
};

#endif // MAINWINDOW_H
