//==============================================================================
// Project: QtQuickPlayer
// Author: Ivan Vasilev <ivan.v.vasilev at Gmail>
//
// A simple application demostrating QtQuick(QML) integration with gstreamer
//
// gst_bus_poller.cpp - a class to periodically pop messages from a gstreamer bsu
//  bus. Doesn't do much right now except printing error messages
//==============================================================================

//==============================================================================
// Includes
//==============================================================================
#include "gst_bus_poller.h"

//==============================================================================
// GstBusPoller Exported methods
//==============================================================================
GstBusPoller::GstBusPoller(GstBus *bus, QString name) : QObject(), m_Name(name), m_Bus(bus)
{
    g_object_ref(m_Bus);
    m_Timer.start(10, this);
}

GstBusPoller::~GstBusPoller()
{
    g_object_unref(m_Bus);
    m_Timer.stop();
}

//==============================================================================
// GstBusPoller private methods
//==============================================================================
void GstBusPoller::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_Timer.timerId()) {
        GstMessage *message;
        gst_object_ref(m_Bus);

        while (NULL != (message = gst_bus_pop(m_Bus)))
        {
            switch (GST_MESSAGE_TYPE (message))
            {
                case GST_MESSAGE_ERROR:
                {
                    GError *err = NULL;
                    gchar *dbg_info = NULL;

                    gst_message_parse_error(message, &err, &dbg_info);
                    g_printerr("%s:ERROR from element %s: %s\r\n", m_Name.toLatin1().data(), GST_OBJECT_NAME (message->src), err->message);
                    g_printerr("%s:Debugging info: %s\r\n", m_Name.toLatin1().data(), (dbg_info) ? dbg_info : "none");
                    g_error_free(err);
                    g_free(dbg_info);
                    break;
                }
                default:
                    g_printerr("%s: Got Message with type: %s\r\n", m_Name.toLatin1().data(), gst_message_type_get_name(GST_MESSAGE_TYPE(message)));
                    break;
            }
        }
        gst_object_unref(m_Bus);
    }
    else
    {
        QObject::timerEvent(event);
    }
}
