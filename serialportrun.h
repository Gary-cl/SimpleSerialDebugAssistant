#ifndef SERIALPORTRUN_H
#define SERIALPORTRUN_H

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QThread>
#include <QDebug>

//#define DEBUG 1
class SerialPortRun : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortRun();
    ~SerialPortRun();
    static bool initFlag;

private:
    QSerialPort *mSerialPort;//声明serial对象

public slots:
    void send(QByteArray data);
    void close();
    void open(QString com, int baud, int dateBits, int stopBits, int parity);

private slots:
    void read();

signals:
        void readBuf(QByteArray);
        void finished(bool);
};

#endif // SERIALPORTRUN_H
