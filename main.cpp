//==============================================================================
// Project: QtQuickPlayer
// Author: Ivan Vasilev <ivan.v.vasilev at Gmail>
//
// A simple application demostrating QtQuick(QML) integration with gstreamer
//
// main.cpp - main application entry point. Gstreamer setup happens here for
//  now, but should probably be moved to a separate location if it gets too big
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

// application includes
#include "gst_bus_poller.h"
#include "interaction_controller.h"

//==============================================================================
// Defines
//==============================================================================
#define VIDEO_URI       "http://distribution.bbb3d.renderfarming.net/video/mp4/bbb_sunflower_1080p_30fps_normal.mp4"
#define AUTOPLAY        true

#define GST_DEBUG_LEVEL "1,2,3" // print important gstreamer messages on stderr

//==============================================================================
// Main entry point
//==============================================================================
int main(int argc, char *argv[])
{
    int ret;
    InteractionController * interactionController;
    GstBusPoller * srcPipelinePoller;

#if defined(GST_DEBUG_LEVEL)
    setenv("GST_DEBUG", GST_DEBUG_LEVEL, 1);
#endif

    QGuiApplication app(argc, argv);

    // Initialize gstreamer
    gst_init (&argc, &argv);

    // create source
    GstElement *src = gst_element_factory_make("playbin", "playbin");
    g_object_set(src, "uri", VIDEO_URI , NULL);

    // we're not using gst_main_loop but it's a good idea to pop the messages
    // manually so that we have an idea of what's happening on the gstreamer
    // side of the fence. Attach a poller to src to get events from it's bus
    GstBus *srcBus = gst_element_get_bus(src);
    if (NULL != srcBus) {
        srcPipelinePoller = new GstBusPoller(srcBus, "srcPipelinePoller");
    }

    // the qmlglsink plugin must be loaded before loading the qml file to
    // register the GstGLVideoItem qml item - see
    // https://bugzilla.gnome.org/show_bug.cgi?id=756082
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

    // set in in motion
    interactionController = new InteractionController(rootObject, src, AUTOPLAY);
    g_assert(interactionController);

    ret = app.exec();

    // clean-up. interactionController will unref src which in turn is the only
    // one holding references to sink&sinkbin
    delete interactionController;
    if (NULL != srcPipelinePoller) delete srcPipelinePoller;
    gst_deinit ();
    
    return ret;
}
