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
    //av_register_all();
    avformat_network_init();
}

void DigiVideoThread::unInit() {
    qDebug()<<"ffmpeg uninit\n";
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
        qDebug()<<"get rtsp failed\n";
        return;
    } else {
        qDebug()<<"get rtsp success\n";
    }

    if(avformat_find_stream_info(mInFmtCtx, NULL) < 0) {
        qDebug()<<"could not find stream information\n";
        return;
    }

    int nVideoIndex = -1;
    for(int i = 0; i < mInFmtCtx->nb_streams; i++) {
        if(mInFmtCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            nVideoIndex = i;
            break;
        }
    }

    if(nVideoIndex == -1) {
        qDebug()<<"could not find video stream\n";
        return;
    }
    qDebug()<<"---------------- File Information ---------------\n";
    AVCodecParameters *codecParameters = mInFmtCtx->streams[nVideoIndex]->codecpar;
    mCodec = (AVCodec*)avcodec_find_decoder(codecParameters->codec_id);
    if(!mCodec) {
        qDebug()<<"could not find codec\n";
        return;
    }
    mCodecCtx = avcodec_alloc_context3(mCodec);;
    if (avcodec_parameters_to_context(mCodecCtx, codecParameters) !=0) {
        qDebug() << "DecoderBase::InitFFDecoder avcodec_parameters_to_context fail.\n";
        return;
    }
    if (avcodec_open2(mCodecCtx, mCodec, NULL) < 0) {
        qDebug("Could not open codec.\n");
        return;
    }
    mFrame     = av_frame_alloc();
    mFrameRGB  = av_frame_alloc();
    int size = av_image_get_buffer_size(AV_PIX_FMT_RGB32, codecParameters->width, codecParameters->height, 1);
    mOutBuf = static_cast<uint8_t *>(av_malloc(static_cast<size_t>(size)));

    if (av_image_fill_arrays(mFrameRGB->data,
                             mFrameRGB->linesize,
                             mOutBuf,
                             AV_PIX_FMT_RGB32,
                             mCodecCtx->width,
                             mCodecCtx->height,
                             1) < 0) {
        qDebug() << "fill array failed\n";
        return;
    }


    struct SwsContext *pImgCtx = sws_getContext(mCodecCtx->width, mCodecCtx->height, mCodecCtx->pix_fmt,
                                                mCodecCtx->width, mCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    mPacket = av_packet_alloc();

    while (!isInterruptionRequested()) {
        if(av_read_frame(mInFmtCtx, mPacket) >= 0) {
            if(mPacket->stream_index == nVideoIndex) {
                if (avcodec_send_packet(mCodecCtx, mPacket) != 0) {
                    qDebug() << "send packet failed";
                    return;
                }
                while (avcodec_receive_frame(mCodecCtx, mFrame) == 0) {
                    sws_scale(pImgCtx, (const uint8_t* const*)mFrame->data,
                              mFrame->linesize, 0, codecParameters->height, mFrameRGB->data,
                              mFrameRGB->linesize);
                    QImage image((uchar*)mOutBuf, codecParameters->width, codecParameters->height, QImage::Format_RGB32);
                    if (!image.isNull()) {
                        emit sendOneFrame(image.copy());
                    }
                }
            }
        }
        msleep(25);
    }
    av_packet_unref(mPacket);
    if(mFrame != nullptr) {
        av_frame_free(&mFrame);
        mFrame = nullptr;
    }
    if(mPacket != nullptr) {
        av_packet_free(&mPacket);
        mPacket = nullptr;
    }

    if(mCodecCtx != nullptr) {
        avcodec_close(mCodecCtx);
        avcodec_free_context(&mCodecCtx);
        mCodecCtx = nullptr;
        mCodec = nullptr;
    }

    if(mInFmtCtx != nullptr) {
        avformat_close_input(&mInFmtCtx);
        avformat_free_context(mInFmtCtx);
        mInFmtCtx = nullptr;
    }

    exec();
}

std::string DigiVideoThread::strPath() const {
    return mPath;
}

void DigiVideoThread::setStrPath(const std::string &strPath) {
    mPath = strPath;
}
