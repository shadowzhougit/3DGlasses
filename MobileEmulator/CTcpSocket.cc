#include "CTcpSocket.h"
#include <QHostAddress>

CTcpSocket::CTcpSocket(QTcpSocket *parent) : QTcpSocket(parent){
    //连接相应槽函数
    connect(this, &CTcpSocket::readyRead, this, &CTcpSocket::readyReadData);
}

void CTcpSocket::connectToServer(const QString &ip) {
    //指定ip且端口号为6688, (QHostAddress中指定的ip需本机存在或能连接到才可使用)
    this->connectToHost(QHostAddress(ip), 6688);
}

void CTcpSocket::sendData(QString data) {
    this->write(data.toUtf8());
}

void CTcpSocket::readyReadData() {
    emit recvDataSignal(this->readAll());
}

