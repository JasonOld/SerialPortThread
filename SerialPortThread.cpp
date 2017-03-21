#include "serialportthread.h"
#include<QTime>


SerialPortThread::SerialPortThread(QObject *parent)
    : QThread(parent), waitTimeout(0), quit(false)
{
    //connect(this,SIGNAL(response(QString)),this,SLOT(Response(QString)));
}

SerialPortThread::~SerialPortThread()
{
    mutex.lock();
    quit = true;
    mutex.unlock();
    wait();
}

void SerialPortThread::run()
{
    QString currentRequset;
    int currentWaitTimeout;
    //程序不会有重复开关的动作，因此这里不需要锁资源
    bool result = serialPort.open(QSerialPort::ReadWrite);
    if(!result){
        emit error(tr("无法打开串口： %1, 错误代码： %2")
                   .arg(portName).arg(serialPort.error()));
        return;
    }
    while(!quit)
    {
        mutex.lock();
        currentRequset=request;
        currentWaitTimeout=waitTimeout;
        mutex.unlock();
        mutex.lock();
        if(!request.isEmpty()){
            // 写请求
            QByteArray requestData = currentRequset.toLocal8Bit();
            serialPort.write(requestData);
            if (serialPort.waitForBytesWritten(currentWaitTimeout)) ;
                else {
                //写请求超时
                emit timeout(tr("写入操作超时： %1")
                             .arg(QTime::currentTime().toString()));
            }
        }
        mutex.unlock();

        mutex.lock();
        // 读取返回
        if(serialPort.isReadable()){
            if (serialPort.waitForReadyRead(currentWaitTimeout)) {
                QByteArray responseData = serialPort.readAll();
                while (serialPort.waitForReadyRead(10))
                    responseData += serialPort.readAll();
                QString response(responseData);
                emit this->response(response);
            } else {
                emit timeout(tr("读取超时Q： %1")
                             .arg(QTime::currentTime().toString()));
            }
        }
        mutex.unlock();
    }
    serialPort.close();

}

bool SerialPortThread::isSerialAreadyOpened()
{
    return this->isRunning();
}

void SerialPortThread::Open(const QString &portName,
                            QSerialPort::BaudRate baudRate,
                            QSerialPort::Parity parity,
                            QSerialPort::DataBits dataBit,
                            QSerialPort::StopBits stopBits)
{
    if(!this->serialPort.isOpen()){
        this->serialPort.setPortName(portName);
        this->serialPort.setBaudRate(baudRate);
        this->serialPort.setParity(parity);
        this->serialPort.setDataBits(dataBit);
        this->serialPort.setStopBits(stopBits);
    }
    this->start();
}


void SerialPortThread::writeData(const QString &s)
{
    if(this->isRunning())
        Request(s);
}

void SerialPortThread::writeData(const QByteArray &s)
{
    if(this->isRunning())
        Request(s);
}

void SerialPortThread::Stop()
{
    mutex.lock();
    quit = true;
    mutex.unlock();
    wait();
}


void SerialPortThread::Request(const QString &request)
{
    QMutexLocker m(&mutex);
    if (isRunning())
        this->request=request;
    else
        emit error("串口已打开");
}
