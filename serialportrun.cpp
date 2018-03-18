#include "serialportrun.h"

bool SerialPortRun::initFlag = false;

SerialPortRun::SerialPortRun()
{
}

SerialPortRun::~SerialPortRun()
{
    if (mSerialPort)
    {
        mSerialPort->close();
        delete mSerialPort;
        mSerialPort = NULL;
    }
}

void SerialPortRun::open(QString com, int baud, int dateBits, int stopBits, int parity)
{
#ifdef DEBUG
    qDebug()<<"thread id of open function = "<<QThread::currentThreadId();
#endif

    //单例模式
    if (!SerialPortRun::initFlag)
    {
#ifdef DEBUG
        qDebug()<<" serialport init ";
#endif
        SerialPortRun::initFlag = true;
        mSerialPort = new QSerialPort();
        connect(mSerialPort,SIGNAL(readyRead()),this,SLOT(read()));
    }

        //打开串口
        mSerialPort->setPortName(com);//设置串口
        if (mSerialPort->open(QIODevice::ReadWrite))//打开串口
        {
            mSerialPort->setBaudRate(baud);//设置波特率

            switch(dateBits)
            {
                case 8:mSerialPort->setDataBits(QSerialPort::Data8);break;
                case 7:mSerialPort->setDataBits(QSerialPort::Data7);break;
                case 6:mSerialPort->setDataBits(QSerialPort::Data6);break;
                case 5:mSerialPort->setDataBits(QSerialPort::Data5);break;
            }
            mSerialPort->setFlowControl(QSerialPort::NoFlowControl);

            switch(stopBits)
            {
                case 0:mSerialPort->setStopBits(QSerialPort::OneStop);break;
                case 1:mSerialPort->setStopBits(QSerialPort::OneAndHalfStop);break;
                case 2:mSerialPort->setStopBits(QSerialPort::TwoStop);break;

            }
            switch(parity)
            {
                case 0:mSerialPort->setParity(QSerialPort::NoParity);break;
                case 1:mSerialPort->setParity(QSerialPort::EvenParity);break;
                case 2:mSerialPort->setParity(QSerialPort::OddParity);break;
            }
            // 返回打开结果
            emit finished(true);
        }
        else
        {
            emit finished(false);
        }
}

void SerialPortRun::close()
{
    mSerialPort->close();
}

void SerialPortRun::send(QByteArray data)
{
#ifdef DEBUG
    qDebug()<<"thread id of send function = "<<QThread::currentThreadId();
#endif
    mSerialPort->write(data, qstrlen(data));//发送串口数据
}

void SerialPortRun::read()
{
#ifdef DEBUG
    qDebug()<<"thread id of read function = "<<QThread::currentThreadId();
#endif
    QByteArray data_received = mSerialPort->readAll();//读取串口上所有数据
    emit readBuf(data_received);
}
