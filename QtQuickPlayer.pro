TEMPLATE = app

QT += qml quick opengl widgets
CONFIG += c++11

DEFINES += GST_USE_UNSTABLE_API

SOURCES += main.cpp \
    interaction_controller.cpp \
    gst_bus_poller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

# not tested (yet)
unix:!macx {
QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig
PKGCONFIG = \
    gstreamer-1.0
}

macx {
    INCLUDEPATH += /opt/local/include/gstreamer-1.0 /opt/local/lib/gstreamer-1.0/include
    INCLUDEPATH += /opt/local/include/glib-2.0 /opt/local/lib/glib-2.0/include /opt/local/include

    LIBS += \
        -framework AppKit \
        -F/Library/Frameworks \
        -L/opt/local/lib \
        -L/opt/local/lib/gstreamer-1.0 \
        -lgstreamer-1.0 -lgobject-2.0 -lglib-2.0 -lintl
}

HEADERS += \
    interaction_controller.h \
    gst_bus_poller.h


