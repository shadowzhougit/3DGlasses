#ifndef CTCPSERVER_H
#define CTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class CTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CTcpServer(QTcpServer *parent = nullptr);

    //发送信息到已连接的所有客户机
    void sendData(QString data);
signals:
    //通过该信号传递接收到的数据
    void recvDataSignal(QString data);
    void connected();
public slots:
    //当有新连接时的槽函数
    void newClient();
    //当有数据来时的槽函数
    void readyReadData();
private:
    QTcpSocket *mClient;   //只接受一个socket
};

#endif // CTCPSERVER_H

