SUBDIRS = roamlib
TEMPLATE = app
INCLUDEPATH += .
DEFINES += QT_NO_ASCII_CAST QT_NO_COMPAT
CONFIG += opengl 
LIBS += roamlib/libroamlib.a

# Input
HEADERS += glwidget.h
SOURCES += glwidget.cpp \
           main.cpp
