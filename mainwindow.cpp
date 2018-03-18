#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    openFlag(false)
{
    ui->setupUi(this);

#ifdef DEBUG
    qDebug()<<"main thread id = "<<QThread::currentThreadId();
#endif

    //线程部分
    mThread = new QThread();
    mSerialPortRun = new SerialPortRun();
    mSerialPortRun->moveToThread(mThread);

    connect(this, SIGNAL(MyOpen(QString , int , int , int , int)), mSerialPortRun, SLOT(open(QString , int , int , int , int)), Qt::QueuedConnection);
    connect(this, SIGNAL(MyClose()), mSerialPortRun, SLOT(close()));
    connect(this, SIGNAL(MySend(QByteArray)), mSerialPortRun, SLOT(send(QByteArray)), Qt::QueuedConnection);
    connect(mSerialPortRun, SIGNAL(finished(bool)), this, SLOT(MyOpened(bool)), Qt::QueuedConnection);
    connect(mSerialPortRun, SIGNAL(readBuf(QByteArray)), this, SLOT(MyRead(QByteArray)), Qt::QueuedConnection);
    connect(ui->textBrowser,SIGNAL(textChanged()),this,SLOT(on_textBrowserRefesh()));

    mThread->start();//启动线程

    //  UI部分  初始化
    ui->pushButton_close->setDisabled(true);
    ui->pushButton_send->setDisabled(true);

    QList<QSerialPortInfo> comInfoList = QSerialPortInfo::availablePorts();//获取可用串口列表
    if(comInfoList.isEmpty())//若没有可用串口，则发送警告
    {
        QMessageBox::warning(this,"Waring!","There's no avalible COM to use, plese check your serialport!");
    }
    else//将可用串口显示到comboBox上以供选择
    {
        for(int i = 0; i < comInfoList.size(); i ++)
        {
            ui->comboBox_portName->addItem(comInfoList[i].portName());
        }
    }
    QStringList baudRateList = QStringList() << "1200" << "2400" << "4800"
                                             << "9600" << "14400" << "19200" << "38400" << "43000" << "57600"
                                             << "76800" << "115200" << "128000" << "230400" << "256000" <<"460800"
                                             << "921600" << "1382400";
    QStringList parityList = QStringList() << "无" << "奇校验" << "偶校验";
    QStringList stopBitsList = QStringList() << "1" << "1.5" << "2";
    QStringList dataBitsList = QStringList() << "8" << "7" << "6" << "5";

    ui->comboBox_baudRate->addItems(baudRateList);
    ui->comboBox_baudRate->setCurrentIndex(3);//设置9600为默认选项

    ui->comboBox_parity->addItems(parityList);
    ui->comboBox_stopBit->addItems(stopBitsList);
    ui->comboBox_dataBit->addItems(dataBitsList);
}

MainWindow::~MainWindow()
{
    if (mSerialPortRun)
    {
        delete mSerialPortRun;
        mSerialPortRun = NULL;
    }

   mThread->quit();
   mThread->wait();
   if (mThread)
   {
       delete mThread;
       mThread = NULL;
   }

    delete ui;
}

void MainWindow::MyRead(QByteArray data)
{
    QByteArray data_received = data;
    //QString str = tc->toUnicode(data_received);//将接受到的数据改变编码格式
    ui->textBrowser->insertPlainText((QString)data_received + "\n");//显示接收到的数据
}

void MainWindow::on_textBrowserRefesh()
{
    ui->textBrowser->moveCursor(QTextCursor::End);//设置每次接收到新数据时光标跟随移动
}

void MainWindow::MyOpened(bool flag)
{
    openFlag = flag;
    emit opened();
}

void MainWindow::on_pushButton_open_clicked()
{
    // 获取串口信息
    QString com = ui->comboBox_portName->currentText();
    int baudRate = ui->comboBox_baudRate->currentText().toInt();
    int dataBits = ui->comboBox_dataBit->currentText().toInt();
    int stopBits = ui->comboBox_stopBit->currentIndex();
    int parity = ui->comboBox_parity->currentIndex();

    //打开串口
    emit MyOpen(com, baudRate, dataBits, stopBits, parity);

    //事件循环 等待打开完成
    QEventLoop loop;
    QObject::connect(this, SIGNAL(opened()), &loop, SLOT(quit()));
    loop.exec(QEventLoop::ExcludeUserInputEvents);
    QObject::disconnect(this, SIGNAL(opened()), &loop, SLOT(quit()));

    if (openFlag)
    {
        ui->pushButton_close->setDisabled(false);
        ui->pushButton_send->setDisabled(false);
        ui->pushButton_open->setDisabled(true);
    }
    else
    {
     QMessageBox::warning(this,"Waring!","fail to open serial port");
    }
}

void MainWindow::on_pushButton_close_clicked()
{
    emit MyClose();
    ui->pushButton_close->setDisabled(true);
    ui->pushButton_open->setDisabled(false);
    ui->pushButton_send->setDisabled(true);
}

void MainWindow::on_pushButton_send_clicked()
{
    QByteArray data = ui->lineEdit_send->text().toLatin1();
    emit MySend(data);
}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();//清屏
}
