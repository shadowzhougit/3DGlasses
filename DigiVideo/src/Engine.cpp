#include "Engine.h"
#include "Types.h"
Engine::Engine(QObject *parent) : QObject{parent},
    mUDPbroadcast(nullptr),
    mExecutor(nullptr),
    mTcpServer(nullptr),
    mDeviceType(DigiVType::DeviceType::None) {
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
    //processExecute();
    connect(this, &Engine::pushVideoSingal, this, &Engine::executeVideoSlot);
    connect(mTcpServer, &CTcpServer::connected, this, &Engine::tcpConnected);
    connect(mTcpServer, &CTcpServer::recvDataSignal, this, &Engine::tcpAppendData);
}

void Engine::processExecute() {
    QStringList params;
    //QString cmd = "ffmpeg -f dshow -i video=\"PC Camera\" -framerate 25 -preset:v ultrafast -tune:v zerolatency -acodec libfaac  -f rtsp -rtsp_transport tcp rtsp://127.0.0.1/live/test";

    params<< "ffmpeg -f dshow -i"<<"video=\"PC Camera\" "<<
             " -framerate  25 "<<
             " -preset:v ultrafast -tune:v"<< "zerolatency -acodec libfaac "<<
             " -f rtsp -rtsp_transport"<<" tcp rtsp://127.0.0.1/live/test ";
   // QString cmd = "ffmpeg -re -i  \"D:\\XR\\ZLMediaKit\\release\\windows\\Debug\\Debug\\NiceChild.mp4\" -framerate 25 -vf scale=360:720  -f rtsp -rtsp_transport tcp rtsp://47.102.115.26/live/test";

    mExecutor->executeString(params);
}

void Engine::tcpConnected() {
    // once the tcp connected need stop UDP
    qDebug() << "tcp connected\n";
    mUDPbroadcast->stopUDP();
}

void Engine::tcpAppendData(const QString& cmd) {
    if (cmd.isNull() || cmd.isEmpty()) {
        qDebug() << "none received tcp cmd.\n";
        return;
    }
    qDebug() << "tcp socket received client:" << cmd << "\n";
    parserCmd(cmd);
}

void Engine::parserCmd(const QString& cmdSrc) {
    if (!cmdSrc.contains("\n")) {
        qDebug() << "received invalid cmd.\n";
        return;
    }
    QString executableSrc = cmdSrc.trimmed();
    executableSrc = executableSrc.replace("\n", "");
    QStringList Cmds = executableSrc.split(",");
    if (Cmds.count() != 3) {
        qDebug() << "cmd invalid count!=2.\n";
        return;
    }
    DigiVType::CMDType cmd =static_cast<DigiVType::CMDType>(Cmds.at(0).toInt());
    QString command = Cmds.at(1);
    QString param = Cmds.at(2);
    qDebug() << cmd << command << param;
    switch (cmd) {
    case DigiVType::CMDType::NoneCmd:
        qDebug() << "NoneCmd";
        break;
    case DigiVType::CMDType::SwitchCmd:
        mDeviceType = static_cast<DigiVType::DeviceType>(param.toInt());
        qDebug() << "SwitchCmd";
        break;
    case DigiVType::CMDType::PushCmd:
        qDebug() << "PushCmd";
        emit pushVideoSingal();
        break;
    case DigiVType::CMDType::PullCmd:
        qDebug() << "PullCmd";
        emit startPlay();
        break;
    }
}

void Engine::executeVideoSlot() {
    processExecute();
}

