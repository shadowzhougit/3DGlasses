#ifndef ENGINE_H
#define ENGINE_H
#include <QObject>
#include "UDPBroadcast.h"
#include "ScriptExecutor.h"
#include "CTcpServer.h"
class Engine : public QObject {
    Q_OBJECT
public:
    explicit Engine(QObject* parent = nullptr);
    ~Engine();
    void initObjects();
    Q_INVOKABLE void processExecute();
signals:

public slots:
    void tcpConnected();
    void tcpAppendData(const QString& data);

private:
    UDPBroadcast    *mUDPbroadcast;
    ScriptExecutor  *mExecutor;
    CTcpServer      *mTcpServer;
};

#endif  // ENGINE_H
