TEMPLATE = app
TARGET = DigiV
QT += qml quick network quickcontrols2

CONFIG += c++11
UI_DIR  = obj/Gui
MOC_DIR = obj/Moc
OBJECTS_DIR = obj/Obj

#DESTDIR=$$PWD/bin/
contains(QT_ARCH, i386) {
    message("32-bit")
    DESTDIR = bin/bin32
} else {
    message("64-bit")
    DESTDIR = bin/bin64
}
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
    LIBS += -L$$PWD/ffmpeg/windows/lib -lavcodec \
                                       -lavdevice \
                                       -lavfilter \
                                       -lavformat \
                                       -lavutil \
                                       -lpostproc \
                                       -lswresample \
                                       -lswscale
}


unix:!macx{
    CONFIG(debug, debug|release) {
        QMAKE_PRE_LINK += copy $$system_path($$PWD/ffmpeg/linux/lib/*.so) $$system_path($$OUT_PWD/debug) &
    }else{
        QMAKE_PRE_LINK += copy $$system_path($$PWD/ffmpeg/linux/lib/*.so) $$system_path($$OUT_PWD/release) &
    }

    INCLUDEPATH += $$PWD/ffmpeg/linux/include
    LIBS += -L$$PWD/ffmpeg/linux/lib -lavcodec \
                                      -lavdevice \
                                      -lavfilter \
                                      -lavformat \
                                      -lavutil \
                                      -lpostproc \
                                      -lswresample \
                                      -lswscale
}

