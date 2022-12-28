#include "CTcpServer.h"
#include <QTcpServer>

CTcpServer::CTcpServer(QTcpServer *parent) : QTcpServer(parent) {
    //设置可以连接的ip和端口号（此处为任意ip且端口号为6666的可以连接）；若要指定ip，设置第一个参数即可
    this->listen(QHostAddress::Any, 6688);
    //连接相关的信号槽
    connect(this, &CTcpServer::newConnection, this, &CTcpServer::newClient);
}

void CTcpServer::sendData(QString data) {
    //遍历客户端列表，将数据发送到所有客户端中（类似广播）
    mClient->write(data.toLocal8Bit(), data.length());
}

void CTcpServer::newClient() {
    //循环获取客户端socket
    while (this->hasPendingConnections()) {
        mClient = this->nextPendingConnection();   //拿到当前的socket
        emit connected();
        connect(mClient, &QTcpSocket::readyRead, this, &CTcpServer::readyReadData);
        break;
    }
}

void CTcpServer::readyReadData() {
    //拿到发送信号的客户端指针，通过该指针读取数据
    QTcpSocket *curClient = dynamic_cast<QTcpSocket *>(sender());
    emit recvDataSignal(curClient->readAll());
}
