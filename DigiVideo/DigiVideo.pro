TEMPLATE = app
TARGET = DigiV
QT += qml quick network

CONFIG += c++11

SOURCES += src/main.cpp \
    src/CTcpServer.cc \
    src/DigiVideo.cc \
    src/DigiVideoThread.cc \
    src/Engine.cpp \
    src/ScriptExecutor.cc \
    src/UDPBroadcast.cc

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/CTcpServer.h \
    src/DigiVideo.h \
    src/DigiVideoThread.h \
    src/Engine.h \
    src/ScriptExecutor.h \
    src/Types.h \
    src/UDPBroadcast.h

win32{
CONFIG(debug, debug|release) {
QMAKE_PRE_LINK += copy $$system_path($$PWD/ffmpeg/windows/bin/*.dll) $$system_path($$OUT_PWD/debug) &
}else{
QMAKE_PRE_LINK += copy $$system_path($$PWD/ffmpeg/windows/bin/*.dll) $$system_path($$OUT_PWD/release) &
}

INCLUDEPATH += $$PWD/ffmpeg/windows/include
LIBS += $$PWD/ffmpeg/windows/lib/avcodec.lib \
        $$PWD/ffmpeg/windows/lib/avdevice.lib \
        $$PWD/ffmpeg/windows/lib/avfilter.lib \
        $$PWD/ffmpeg/windows/lib/avformat.lib \
        $$PWD/ffmpeg/windows/lib/avutil.lib \
        #$$PWD/ffmpeg/windows/lib/postproc.lib \
        $$PWD/ffmpeg/windows/lib/swresample.lib \
        $$PWD/ffmpeg/windows/lib/swscale.lib
}


unix:!macx{

CONFIG(debug, debug|release) {
QMAKE_PRE_LINK += copy $$system_path($$PWD/ffmpeg/linux/lib/*.so) $$system_path($$OUT_PWD/debug) &
}else{
QMAKE_PRE_LINK += copy $$system_path($$PWD/ffmpeg/linux/lib/*.so) $$system_path($$OUT_PWD/release) &
}

INCLUDEPATH += $$PWD/ffmpeg/linux/include
LIBS += $$PWD/ffmpeg/linux/lib/libavcodec-57.so \
        $$PWD/ffmpeg/linux/lib/libavdevice-57.so \
        $$PWD/ffmpeg/linux/lib/libavfilter-6.so \
        $$PWD/ffmpeg/linux/lib/libavformat-57.so \
        $$PWD/ffmpeg/linux/lib/libavutil-55.so \
        #$$PWD/ffmpeg/linux/lib/libpostproc-54.so \
        $$PWD/ffmpeg/linux/lib/libswresample-2.so \
        $$PWD/ffmpeg/linux/lib/libswscale-4.so
}
