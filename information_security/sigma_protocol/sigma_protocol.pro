TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        aes/aes.cpp \
        aes/aes_func.cpp \
        aes/operations.cpp \
        dsa/bigint.cpp \
        dsa/dsa.cpp \
        dsa/rsa.cpp \
        eliptic_binary/BinaryPolynomial.cpp \
        eliptic_binary/diffie_hellman.cpp \
        eliptic_binary/point.cpp \
        main.cpp \
        sha/mac.cpp \
        sha/sha_224.cpp \
        sha/sha_3_512.cpp

HEADERS += \
    aes/aes.h \
    aes/aes_func.h \
    aes/operations.h \
    dsa/bigint.h \
    dsa/defines.h \
    dsa/dsa.h \
    dsa/rsa.h \
    eliptic_binary/BinaryPolynomial.h \
    eliptic_binary/diffie_hellman.h \
    eliptic_binary/point.h \
    sha/defines.h \
    sha/mac.h \
    sha/sha_224.h \
    sha/sha_3_512.h

SUBDIRS += \
    dsa/dsa.pro
