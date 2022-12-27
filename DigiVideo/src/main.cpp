#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "DigiVideo.h"
#include "ScriptExecutor.h"
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<DigiVideo>("DigiVideo", 1, 0, "DigiVideo");
    engine.load(QUrl(QStringLiteral("qrc:/res/main.qml")));

    ScriptExecutor *executor = new ScriptExecutor();
    QStringList params;
    params<< " -f"<<" dshow " <<
             " -i "<<"video='PC Camera' "<<
             " -framerate"<<" 25 " <<
             " -preset:v "<< "ultrafast "<<
             " -tune:v "<< "zerolatency"<<
             " -acodec "<< "libfaac "<<
             " -f "<< " rtsp"<<
             " -rtsp_transport"<<" tcp rtsp://127.0.0.1/live/test ";

    executor->executeString(params);
    return app.exec();
}
