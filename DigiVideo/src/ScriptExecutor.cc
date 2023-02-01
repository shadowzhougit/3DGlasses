#include "ScriptExecutor.h"
#include <QDebug>
#include <QFileInfo>
#include <QJSEngine>
#include <QJSValueList>
#include <QObject>
#include <iostream>
#include <thread>
#include <QCoreApplication>
#define TEN_SECOND 1000 * 13

ScriptExecutor::ScriptExecutor(QObject *parent) : QObject(parent) {}

ScriptExecutor::~ScriptExecutor() {
    qDebug("Process destructor closed disconnect the connection.");
}

void ScriptExecutor::executeShellFile(const QString &filePath, const QString &param) {
    qDebug("executeShellFile filePath:%s, param:", filePath.toStdString().c_str(), param.toStdString().c_str());
#ifndef Q_OS_LINUX  // windows desn't support shell script.
    emit executeCompleted(false);
#else
    auto execmainFunc = [this, filePath, param]() -> void {
        if (!QFile::exists(filePath)) {
            emit executeCompleted(false);
            qDebug("Process shell file doesn't exist");
            return;
        }

        QProcess *process = new QProcess();
        connect(process, &QProcess::started, this, [this, process]() { qDebug("Process started"); });

        connect(process, &QProcess::readyReadStandardOutput, this, [this, process] {
            // read standard out put
            startReadOutput(process);
        });

        connect(process, static_cast<void (QProcess::*)(int exitCode)>(&QProcess::finished), this,
                [this, process](int exitCode) {
            // finished
            finishProcess(exitCode, process);
            closeProcess(process);
        });

        QString cmdPwd = "meta";
        QString sticher = QString("echo '%1'|sudo -S %2 %3").arg(cmdPwd).arg(filePath).arg(param);

        process->start("bash", QStringList() << "-c" << sticher);
        bool isWaiting = process->waitForFinished(TEN_SECOND);
        if (!isWaiting) {
            qDebug("Process wait for finishing but it's timeout");
            emit executeCompleted(false);
            closeProcess(process);
        }
    };

    std::thread thread(execmainFunc);
    thread.detach();
#endif
}

void ScriptExecutor::executeShellScript(const QString &script) {
    qDebug("executeShellScript %s", script.toStdString().c_str());
#ifdef Q_OS_WIN
    emit executeCompleted(false);
#else
    auto execmainFunc = [this, script]() -> void {
        QProcess *process = new QProcess();
        connect(process, &QProcess::started, this, [this, process]() {
            // started
            qDebug("Process started");
            startProcess(process);
        });
        connect(process, &QProcess::readyReadStandardOutput, this, [this, process] {
            // read standard out pu
            startReadOutput(process);
        });
        connect(process, static_cast<void (QProcess::*)(int exitCode)>(&QProcess::finished), this,
                [this, process](int exitCode) {
            // finished
            qDebug("Process finised the exit code=%d", exitCode);
            finishProcess(exitCode, process);
            closeProcess(process);
        });

        QString cmdPwd = "meta";
        QString sticher = QString("echo '%1'|sudo -S %2").arg(cmdPwd).arg(script);

        process->start("bash", QStringList() << "-c" << sticher);
        bool waiting = process->waitForFinished(TEN_SECOND);
        if (!waiting) {
            qDebug("Process wait for finishing but it's timeout");
            emit executeCompleted(false);
        }
    };
    std::thread thread(execmainFunc);
    thread.detach();
#endif
}

void ScriptExecutor::executeString(const QStringList & executeParams) {

    auto execmainFunc = [this, executeParams]() -> void {
        if (executeParams.isEmpty()) {
            return;
        }

        QProcess *process = new QProcess();
        connect(process, &QProcess::started, this, [this, process]() { qDebug("Process started"); });

        connect(process, &QProcess::readyReadStandardOutput, this, [this, process] {
            // read standard out put
            startReadOutput(process);
        });

        connect(process, static_cast<void (QProcess::*)(int exitCode)>(&QProcess::finished), this,
                [this, process](int exitCode) {
            // finished
            finishProcess(exitCode, process);
            closeProcess(process);
        });
        connect(qApp, &QCoreApplication::aboutToQuit, this,[this, process] {
            closeProcess(process);
        });
        qDebug() << "Nagrywam!";

        process->setProcessChannelMode(QProcess::MergedChannels);
        //push mp4 file to sever
        //QString cmd = executeParams.join(" ");
         //"ffmpeg -re -i  \"D:\\XR\\ZLMediaKit\\release\\windows\\Debug\\Debug\\NiceChild.mp4\" -framerate 25 -vf scale=360:720  -f rtsp -rtsp_transport tcp rtsp://47.102.115.26/live/test";
        QString cmd = "ffmpeg -f dshow -i video=\"PC Camera\" -framerate 25 -preset:v ultrafast -tune:v zerolatency -acodec libfaac  -f rtsp -rtsp_transport tcp rtsp://47.102.115.26/live/test";
        process->start(cmd);
        process->waitForFinished();
    };

    std::thread thread(execmainFunc);
    thread.detach();
}

void ScriptExecutor::startProcess(QProcess *process) {
    qDebug("Process started state: %d", process->state());
}

void ScriptExecutor::startReadOutput(QProcess *process) {
    qDebug("Process startReadOutput state: %d", process->state());
    if (process != nullptr && process->isOpen()) {
        QByteArray stdOutput = process->readAll();
        qDebug("scriptExecutor output: %s", stdOutput.toStdString().c_str());
    }
}

void ScriptExecutor::finishProcess(const int &exitCode, QProcess *process) {
    // invoke callback, when success sh will exit 0 else will exit 1
    qDebug("Process finised the exit code=%d state: %d", exitCode, process->state());
    emit executeCompleted(exitCode == 0);
}

void ScriptExecutor::closeProcess(QProcess *process) {
    process->disconnect();
    if (process != nullptr && process->isOpen()) {
        process->kill();
        delete process;
        process = nullptr;
    }
}
