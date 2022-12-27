// (C) DigiV Copyright 2022 - 2023
//
// All rights are reserved. Reproduction or transmission in whole or
// in part, in any form or by any means, electronic, mechanical or
// otherwise, is prohibited without the prior written permission of
// the copyright owner.
//
#ifndef DigiVideoTHREAD_H
#define DigiVideoTHREAD_H

#include <QThread>
#include "stdint.h"
extern "C"{

#ifdef __cplusplus
 #define __STDC_CONSTANT_MACROS
 #ifdef _STDINT_H
  #undef _STDINT_H
 #endif
 # include <stdint.h>
#endif

}
extern "C"
{

#include "libavformat/avformat.h"
#include "libavutil/mathematics.h"
#include "libavutil/time.h"
#include "libswscale/swscale.h"
}
class DigiVideoThread : public QThread
{
    Q_OBJECT
public:
    DigiVideoThread();
    ~DigiVideoThread();
    std::string strPath() const;
    void setStrPath(const std::string &strPath);

private:
    void init();
    void unInit();
    void run();
    uint8_t *mOutBuf;
    AVFrame *mFrame;
    AVFrame *mFrameRGB;
    AVCodecContext  *mCodecCtx;
    AVFormatContext *mInFmtCtx;
    AVCodec         *mCodec;
    AVPacket        *mPacket;
    std::string mPath;

signals:
    void sendOneFrame(const QImage& pImage);
};

#endif
