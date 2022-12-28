#ifndef CUDPSOCKET_H
#define CUDPSOCKET_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>

class CUdpSocket : public QObject
{
    Q_OBJECT

public:
    explicit CUdpSocket(QObject *parent = nullptr);

    ~CUdpSocket();
    //通过该函数发送数据
    void sendData(QString data);

    //设置目标主机的ip和端口号
    void setTargetInfo(QString ip, quint16 port);
    Q_INVOKABLE QString getServerIP() { return mServerIP; }
signals:
    //通过该信号传递接收到的数据
    void recvDataSignal(QString data);

public slots:
    //读取数据的槽函数
    void readyReadData();

private:
    QUdpSocket      *mSock;        //UDP套接字指针
    QHostAddress    mHostAddr;     //保存目标的地址对象
    quint16         mPort;         //保存目标的端口号(类型一致)
    QString         mServerIP;
private:
    void parserServerIP(const QString &value);
};

#endif // CUDPSOCKET_H


