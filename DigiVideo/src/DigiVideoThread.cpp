#include "DigiVideoThread.h"
#include <QImage>
#include <QDebug>

DigiVideoThread::DigiVideoThread() {
    this->init();
}

DigiVideoThread::~DigiVideoThread() {
    requestInterruption();
    quit();
    wait();
    this->unInit();
}

void DigiVideoThread::init() {
    av_register_all();
    avformat_network_init();
}

void DigiVideoThread::unInit() {
    qDebug()<<"ffmpeg uninit";
    av_free(mOutBuf);
    av_free(mFrameRGB);
    av_free(mFrame);
    av_free(mCodec);
    avcodec_close(mCodecCtx);
    avformat_close_input(&mInFmtCtx);
}

void DigiVideoThread::run() {
    mInFmtCtx = avformat_alloc_context();
    if(avformat_open_input(&mInFmtCtx, mPath.c_str(), NULL, NULL)) {
        qDebug()<<"get rtsp failed";
        return;
    } else {
        qDebug()<<"get rtsp success";
    }

    if(avformat_find_stream_info(mInFmtCtx, NULL) < 0) {
        qDebug()<<"could not find stream information";
        return;
    }

    int nVideoIndex = -1;
    for(int i = 0; i < mInFmtCtx->nb_streams; i++) {
        if(mInFmtCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            nVideoIndex = i;
            break;
        }
    }

    if(nVideoIndex == -1) {
        qDebug()<<"could not find video stream";
        return;
    }
    //Output Info---输出一些信息
    qDebug("---------------- File Information ---------------");
    //av_dump_format(m_pInFmtCtx, 0, m_strPath.c_str(), 0);
    mCodecCtx = mInFmtCtx->streams[nVideoIndex]->codec;
    mCodec = avcodec_find_decoder(mCodecCtx->codec_id);
    if(!mCodec) {
        qDebug()<<"could not find codec";
        return;
    }

    if (avcodec_open2(mCodecCtx, mCodec, NULL) < 0) {
        qDebug("Could not open codec.\n");
        return;
    }
    mFrame     = av_frame_alloc();
    mFrameRGB  = av_frame_alloc();
    mOutBuf = (uint8_t*)av_malloc(avpicture_get_size(AV_PIX_FMT_RGB32, mCodecCtx->width, mCodecCtx->height));
    avpicture_fill((AVPicture*)mFrameRGB, mOutBuf, AV_PIX_FMT_RGB32, mCodecCtx->width, mCodecCtx->height);


    struct SwsContext *pImgCtx = sws_getContext(mCodecCtx->width, mCodecCtx->height, mCodecCtx->pix_fmt,
                                                mCodecCtx->width, mCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    int nSize = mCodecCtx->width * mCodecCtx->height;
    mPacket = (AVPacket *)av_malloc(sizeof(AVPacket));
    if(av_new_packet(mPacket, nSize) != 0) {
        qDebug()<<"new packet failed";
    }

    while (!isInterruptionRequested()) {
        if(av_read_frame(mInFmtCtx, mPacket) >= 0) {
            if(mPacket->stream_index == nVideoIndex) {
                int nGotPic = 0;
                if(avcodec_decode_video2(mCodecCtx, mFrame, &nGotPic, mPacket) < 0) {
                    qDebug()<<"decode failed";
                    return;
                }
                if(nGotPic) {
                    sws_scale(pImgCtx, (const uint8_t* const*)mFrame->data,
                              mFrame->linesize, 0, mCodecCtx->height, mFrameRGB->data,
                              mFrameRGB->linesize);
                    QImage image((uchar*)mOutBuf, mCodecCtx->width, mCodecCtx->height, QImage::Format_RGB32);
                    if (!image.isNull()) {
                        emit sendOneFrame(image);
                    }
                }
            }
        }
        av_free_packet(mPacket);
        msleep(25);
    }
    exec();
}

std::string DigiVideoThread::strPath() const {
    return mPath;
}

void DigiVideoThread::setStrPath(const std::string &strPath) {
    mPath = strPath;
}
