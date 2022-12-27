// (C) DigiV Copyright 2022 - 2023
//
// All rights are reserved. Reproduction or transmission in whole or
// in part, in any form or by any means, electronic, mechanical or
// otherwise, is prohibited without the prior written permission of
// the copyright owner.
//
#ifndef DigiVideo_H
#define DigiVideo_H
#include <QQuickPaintedItem>
#include <QImage>
#include "DigiVideoThread.h"
class DigiVideo : public QQuickPaintedItem
{
    Q_OBJECT
public:
    Q_INVOKABLE void setSize(int width, int height);
    Q_PROPERTY(int videoWidth READ videoWidth WRITE setVideoWidth NOTIFY videoWidthChanged)
    Q_PROPERTY(int videoHeight READ videoHeight WRITE setVideoHeight NOTIFY videoHeightChanged)
    Q_PROPERTY(QString videoPath READ videoPath WRITE setVideoPath NOTIFY videoPathChanged)

    explicit DigiVideo();
    ~DigiVideo();
    int videoHeight() const { return mHeight; }
    void setVideoHeight(int value);

    int videoWidth() const { return mWidth; }
    void setVideoWidth(int value);

    QString videoPath() const { return mVideoPath; }
    void setVideoPath(const QString &value);

protected:
    virtual void paint(QPainter *pPainter);
private:
    QImage mFrame;
    DigiVideoThread *mVedioThread;
    int mHeight;
    int mWidth;
    QString mVideoPath;
public slots:
    void getOneFrame(const QImage& image);

signals:
    void videoWidthChanged(int mWidth);
    void videoHeightChanged(int mHeight);
    void videoPathChanged(QString strPath);

};

#endif
