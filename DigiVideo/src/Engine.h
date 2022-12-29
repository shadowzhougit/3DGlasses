#ifndef ENGINE_H
#define ENGINE_H
#include <QObject>
#include "UDPBroadcast.h"
#include "ScriptExecutor.h"
#include "CTcpServer.h"
#include "Types.h"
class Engine : public QObject {
    Q_OBJECT
public:
    explicit Engine(QObject* parent = nullptr);
    ~Engine();
    void initObjects();
    void processExecute();

signals:
    void startPlay();
    void pushVideoSingal();

public slots:
    void tcpConnected();
    void tcpAppendData(const QString& data);
    void executeVideoSlot();
private:
    UDPBroadcast    *mUDPbroadcast;
    ScriptExecutor  *mExecutor;
    CTcpServer      *mTcpServer;
    DigiVType::DeviceType mDeviceType;

private:
    void parserCmd(const QString& cmd);
};

#endif  // ENGINE_H
