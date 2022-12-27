// (C) DigiV Copyright 2022 - 2023
//
// All rights are reserved. Reproduction or transmission in whole or
// in part, in any form or by any means, electronic, mechanical or
// otherwise, is prohibited without the prior written permission of
// the copyright owner.
//
#ifndef SHELLEXECUTE_H
#define SHELLEXECUTE_H

#include <QObject>
#include <QProcess>

class ScriptExecutor : public QObject {
    Q_OBJECT
   public:
    explicit ScriptExecutor(QObject *parent = nullptr);
    ~ScriptExecutor();

   public:
    void startProcess(QProcess *process);
    void startReadOutput(QProcess *process);
    void finishProcess(const int &exitCode, QProcess *process);

   signals:
    void executeCompleted(const bool &success);

   public slots:
    void executeShellFile(const QString &filePath, const QString &param);
    void executeShellScript(const QString &script);
    void executeString(const QStringList & executeParams);
   private:
    void closeProcess(QProcess *process);
};

#endif  // SHELLEXECUTE_H
