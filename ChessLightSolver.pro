TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    ImgUtils.cpp \
    chess.cpp
INCLUDEPATH += /usr/local/Cellar/opencv/2.4.6.1/include/

QMAKE_CXXFLAGS += -isystem /usr/local/Cellar/opencv/2.4.6.1/include/ -g -O2
QMAKE_LIBDIR += /usr/local/Cellar/opencv/2.4.6.1/lib/
QMAKE_LIBS += -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_features2d -lopencv_nonfree -lopencv_objdetect -lopencv_flann

HEADERS += \
    ImgUtils.h \
    chess.h

