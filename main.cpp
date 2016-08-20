//==============================================================================
// QtQuickPlayer
// Author: Ivan Vasilev
//
// A simple application demostratint QtQuick(QML) integration with gstreamer
//
//==============================================================================

//==============================================================================
// Includes
//==============================================================================

// QT includes
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QRunnable>

// gstreamer includes
#include "gst/gst.h"

#include "gst_bus_poller.h"
#include "interaction_controller.h"


//==============================================================================
// Defines
//==============================================================================
#define VIDEO_URI       "http://distribution.bbb3d.renderfarming.net/video/mp4/bbb_sunflower_1080p_30fps_normal.mp4"
#define AUTOPLAY        false

int main(int argc, char *argv[])
{
    int ret;

    InteractionController * interactionController;

    QGuiApplication app(argc, argv);

    // Initialize gstreamer
    gst_init (&argc, &argv);


    // create source
    GstElement *src = gst_element_factory_make("playbin", "playbin");
    g_object_set(src, "uri", VIDEO_URI , NULL);

    // the plugin must be loaded before loading the qml file to register the
    // GstGLVideoItem qml item
    GstElement *sink = gst_element_factory_make ("qmlglsink", NULL);
    GstElement *sinkbin = gst_element_factory_make ("glsinkbin", NULL);

    g_assert (src && sink && sinkbin);

    // connect sinks together
    g_object_set (sinkbin, "sink", sink, NULL);
    g_object_unref (sink);

    g_object_set(src, "video-sink", sinkbin, NULL);
    g_object_unref(sinkbin);

    // instantiate Qml app engine
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    // setup qmlGlSink
    QQuickItem *videoItem;
    QQuickWindow *rootObject;
    
    // find and set the videoItem on the sink
    rootObject = static_cast<QQuickWindow *>(engine.rootObjects().first());
    videoItem = rootObject->findChild<QQuickItem *>("videoItem");
    g_assert (videoItem);
    g_object_set(sink, "widget", videoItem, NULL);

    interactionController = new InteractionController(rootObject, src, AUTOPLAY);

    ret = app.exec();

    delete interactionController;
    
    gst_deinit ();
    
    return ret;
}
