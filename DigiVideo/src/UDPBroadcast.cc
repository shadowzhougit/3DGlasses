#include "UDPBroadcast.h"
#include <QIODevice>
UDPBroadcast::UDPBroadcast(QObject *parent) : QObject(parent) {
    mUDPSocket = new QUdpSocket();
    QHostAddress hostAddress(getLocalIP());
    mUDPSocket->bind(hostAddress, UDP_OPEN_PORT);

    mTimer = new QTimer(this);
    startUDP();
    connect(mTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
}


UDPBroadcast::~UDPBroadcast() {
    // 释放UDP套接字内存空间
    delete mUDPSocket;
    delete mTimer;
}

void UDPBroadcast::stopUDP() {        
    mTimer->stop();
    mUDPSocket->close();
    qDebug() << "close timer, close udp.\n";
}

void UDPBroadcast::startUDP() {
    qDebug() << "udp connected success\n";
    mUDPSocket->open(QIODevice::OpenModeFlag::ReadWrite);
    mTimer->start(TIMER_TIMEOUT);
}

void UDPBroadcast::sendMessage() {
    //broadcast
    QString protocolStr = "digiv:serial number;ip:" + getLocalIP() + "\n";
    qint64 len = mUDPSocket->writeDatagram(protocolStr.toUtf8(), QHostAddress::Broadcast, UDP_OPEN_PORT);
    if (len == -1) {
        qDebug() << "udp sockect write datagram failed\n";
    } else {
        //qDebug() << "udp sockect launch success\n";
    }
}

void UDPBroadcast::handleTimeout() {
    //qDebug() << "Enter timeout process function\n";
    sendMessage();
}

QString UDPBroadcast::getLocalIP() {
    QString hostname = QHostInfo::localHostName();
    QHostInfo hostinfo = QHostInfo::fromName(hostname);
    QString localip = "";
    QList<QHostAddress> addList = hostinfo.addresses();
    if (addList.isEmpty()){
        return localip;
    }
    for (int i = 0;i<addList.size();i++) {
        QHostAddress aHost = addList.at(i);
        if (QAbstractSocket::IPv4Protocol == aHost.protocol()) {
            localip = aHost.toString();
            break;
        }
    }
    return localip;
}
