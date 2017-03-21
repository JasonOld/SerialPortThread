#ifndef SERIALPORTTHREAD_H
#define SERIALPORTTHREAD_H

#include<QThread>
#include<QMutex>
#include<QWaitCondition>
#include<QSerialPort>

class SerialPortThread:public QThread
{

    Q_OBJECT
public:
    SerialPortThread(QObject *parent = 0);
    ~SerialPortThread();

public:
    void run();
    //通过检测线程是否运行来判断串口打开情况
    bool isSerialAreadyOpened();
signals:
    //三个信号量，数据返回，出现错误，超时错误
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

public slots:
//槽函数
//打开串口
    void Open(const QString& portName,
              QSerialPort::BaudRate baudRate=QSerialPort::Baud115200,
              QSerialPort::Parity parity=QSerialPort::NoParity,
              QSerialPort::DataBits dataBit=QSerialPort::Data8,
              QSerialPort::StopBits stopBits=QSerialPort::OneStop);
    //写入数据
    void writeData(const QString& s);
    void writeData(const QByteArray& s);
    //串口停止
    void Stop();

private:
    //写入数据调用的私有方法
    void Request(const QString &request);

private:
    QString portName;
    QString request;
    int waitTimeout;
    QMutex mutex;
    QSerialPort serialPort;
    bool quit;
};

#endif // SERIALPORTTHREAD_H
