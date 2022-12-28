#include "Engine.h"

Engine::Engine(QObject *parent) : QObject{parent} {
    initObjects();
}

Engine::~Engine() {
    if (nullptr != mUDPbroadcast) {
        delete mUDPbroadcast;
        mUDPbroadcast = nullptr;
    }

    if (nullptr != mExecutor) {
        delete mExecutor;
        mExecutor = nullptr;
    }

    if (nullptr != mTcpServer) {
        delete mTcpServer;
        mTcpServer = nullptr;
    }
}

void Engine::initObjects() {
    mUDPbroadcast = new UDPBroadcast(this);
    qDebug() << "inited udp broadcast success!\n";
    mExecutor = new ScriptExecutor(this);
    qDebug() << "init script executor success!\n";
    mTcpServer = new CTcpServer();
    qDebug() << "init tcp server success!\n";
    connect(mTcpServer, &CTcpServer::recvDataSignal, this, &Engine::tcpAppendData);
    connect(mTcpServer, &CTcpServer::connected, this, &Engine::tcpConnected);
}

void Engine::processExecute() {
    QStringList params;
    params<< " -f"<<" dshow " <<
             " -i "<<"video='PC Camera' "<<
             " -framerate"<<" 25 " <<
             " -preset:v "<< "ultrafast "<<
             " -tune:v "<< "zerolatency"<<
             " -acodec "<< "libfaac "<<
             " -f "<< " rtsp"<<
             " -rtsp_transport"<<" tcp rtsp://127.0.0.1/live/test ";
    mExecutor->executeString(params);
}

void Engine::tcpConnected() {
    // once the tcp connected need stop UDP
    qDebug() << "tcp connected\n";
    mUDPbroadcast->stopUDP();
}

void Engine::tcpAppendData(const QString& data) {
    qDebug() << "tcp socket received client:" << data << "\n";
}
