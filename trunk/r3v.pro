TEMPLATE = app
INCLUDEPATH += .
CONFIG += opengl
DEFINES += QT_NO_ASCII_CAST QT_NO_COMPAT

# Input
HEADERS += diamond.h trianglelist.h geopoint.h glwidget.h map.h node.h observer.h parser.h triangle.h
SOURCES += diamond.cpp \
           trianglelist.cpp \
           geopoint.cpp \
           glwidget.cpp \
           main.cpp \
           map.cpp \
           node.cpp \
           observer.cpp \
           parser.cpp \
           triangle.cpp
