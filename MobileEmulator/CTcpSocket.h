#ifndef CTCPSOCKET_H
#define CTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>

class CTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CTcpSocket(QTcpSocket *parent = nullptr);
    //通过改函数发送数据
    Q_INVOKABLE void sendData(QString data);
    Q_INVOKABLE void connectToServer(const QString &ip);
signals:
    //通过该信号传递接收到的数据
    void recvDataSignal(QString data);
public slots:
    //读取数据的槽函数
    void readyReadData();
};
#endif // CTCPSOCKET_H

