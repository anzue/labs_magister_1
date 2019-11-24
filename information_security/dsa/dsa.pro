TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        bigint.cpp \
        dsa.cpp \
        main.cpp \
        rsa.cpp \
        sha_224.cpp

HEADERS += \
    bigint.h \
    defines.h \
    defines.h \
    sha_224.h
