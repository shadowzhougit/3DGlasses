#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "DigiVideo.h"
#include "Engine.h"
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("engine"), new Engine(&app));
    qmlRegisterType<DigiVideo>("DigiVideo", 1, 0, "DigiVideo");
    engine.load(QUrl(QStringLiteral("qrc:/res/main.qml")));

    return app.exec();
}
