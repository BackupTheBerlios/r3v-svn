TEMPLATE = app
INCLUDEPATH += .
CONFIG += opengl
DEFINES += QT_NO_ASCII_CAST QT_NO_COMPAT

# Input
HEADERS += diamond.h diamondlist.h trianglelist.h geopoint.h glwidget.h map.h node.h observer.h parser.h triangle.h
SOURCES += diamond.cpp \
           diamondlist.cpp \
           trianglelist.cpp \
           geopoint.cpp \
           glwidget.cpp \
           main.cpp \
           map.cpp \
           node.cpp \
           observer.cpp \
           parser.cpp \
           roam.cpp \
           triangle.cpp
