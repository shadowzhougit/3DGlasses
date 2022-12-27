#include "CUdpSocket.h"

CUdpSocket::CUdpSocket(QObject *parent) : QObject(parent)
{
    qDebug() << "CUdp socket init\n";
    //创建UDP套接字内存空间
    mSock = new QUdpSocket;
    mSock->bind(6677, QUdpSocket::ShareAddress);
    // 连接数据读取信号槽
    connect(mSock, &QUdpSocket::readyRead, this, &CUdpSocket::readyReadData);

}

CUdpSocket::~CUdpSocket()
{
    // 释放UDP套接字内存空间
    delete mSock;
}

bool CUdpSocket::bind(QString ip, ushort port)
{
    // 返回绑定函数返回值
    return mSock->bind(QHostAddress(ip), port);
}

void CUdpSocket::sendData(QString data)
{
    // 发送传入的数据到指定的信息的位置
    mSock->writeDatagram(data.toUtf8(), mHostAddr, mPort);
}

void CUdpSocket::setTargetInfo(QString ip, quint16 port)
{
    // 存储传入的IP和端口号
    mHostAddr = QHostAddress(ip);
    mPort = port;
}

void CUdpSocket::readyReadData()
{
    // 通过函数判断当前是否有等待读取的数据并循环获取
    while(mSock->hasPendingDatagrams()) {
        //创建数据存储容器，并设置长度为将接收的数据长度
        QByteArray data;
        data.resize(mSock->pendingDatagramSize());
        //读取数据并保存信息发送者的地址和ip(方便发送时指定发送位置)
        mSock->readDatagram(data.data(), data.size(), &mHostAddr, &mPort);
        //发送接收数据的信号
        emit recvDataSignal(data);
    }
}

