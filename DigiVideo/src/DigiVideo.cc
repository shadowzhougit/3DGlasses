#include "DigiVideo.h"
#include <QPainter>
#include <QDebug>
#include "DigiVideoThread.h"

void DigiVideo::setSize(int width, int height){
    mWidth = width;
    mHeight = height;
}

DigiVideo::DigiVideo(): mHeight(205), mWidth(360){
    qDebug()<<"create\n";
    mVedioThread = new DigiVideoThread();
    connect(mVedioThread, &DigiVideoThread::sendOneFrame, this, &DigiVideo::getOneFrame);
    mVedioThread->start();
}

DigiVideo::~DigiVideo() {
    qDebug()<<"distory\n";
    mVedioThread->deleteLater();
}

void DigiVideo::getOneFrame(const QImage& image) {
    if (!image.isNull()) {
        mFrame = image.copy();
        //qDebug()<<"get one frame";
        update();
    }
}

void DigiVideo::paint(QPainter *pPainter) {
    if (!mFrame.isNull()) {
        //qDebug()<<"painter";
        pPainter->drawImage(QRect(0, 0, mWidth, mHeight), mFrame);
    }
}

void DigiVideo::setVideoPath(const QString &value) {
    if (mVideoPath != value) {
        mVideoPath = value;
        if (mVedioThread) {
            mVedioThread->setStrPath(value.toStdString());
        }
        emit videoPathChanged(value);
    }
}


void DigiVideo::setVideoWidth(int value) {
    if (mWidth != value) {
        mWidth = value;
        emit videoWidthChanged(value);
    }
}

void DigiVideo::setVideoHeight(int value) {
    if (mHeight != value) {
        mHeight = value;
        emit videoHeightChanged(value);
    }
}
