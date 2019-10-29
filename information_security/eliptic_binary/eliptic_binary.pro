TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        BinaryPolynomial.cpp \
        Point.cpp \
        diffie_hellman.cpp \
        main.cpp

HEADERS += \
    BinaryPolynomial.h \
    Point.h \
    diffie_hellman.h
