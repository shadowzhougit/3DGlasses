#ifndef UDPBROADCAST_H
#define UDPBROADCAST_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QHostInfo>
#include <QHostAddress>
#include <QNetworkInterface>
#define TIMER_TIMEOUT 1000
const quint16 UDP_OPEN_PORT = 6677;

class UDPBroadcast : public QObject
{
    Q_OBJECT
public:
    explicit UDPBroadcast(QObject *parent = nullptr);
    ~UDPBroadcast();
    void stopUDP();
    QString getLocalIP();
    QHostAddress getLocalHostIP();
private:
    QUdpSocket *mUDPSocket;
    QTimer     *mTimer;
signals:

public slots:
    void startUDP();
    void sendMessage();
    void handleTimeout();
};

#endif // UDPBROADCAST_H
