TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    sha_224.cpp \
    sha_3_512.cpp

HEADERS += \
    defines.h
