//==============================================================================
// Project: QtQuickPlayer
// Author: Ivan Vasilev <ivan.v.vasilev at Gmail>
//
// A simple application demostrating QtQuick(QML) integration with gstreamer
//
// gst_bus_poller.h - a class to periodically pop messages from a gstreamer
//  bus. nominal use case is
//  GstElement *pipeline = gst_pipeline_new ("playpipeline");
//  playPipelinePoller = new GstBusPoller(gst_pipeline_get_bus(GST_PIPELINE (pipeline)), "playPipelinePoller");
//
//==============================================================================

#ifndef GSTBUSPOLLER_H
#define GSTBUSPOLLER_H

//==============================================================================
// Includes
//==============================================================================

// QT Includes
#include <QObject>
#include <QRunnable>
#include <QBasicTimer>
#include <QTimerEvent>

// gstreamer includes
#include "gst/gst.h"

class GstBusPoller : public QObject
{
public:
    GstBusPoller(GstBus *bus, QString name);
    ~GstBusPoller();

private:
    QString         m_Name;
    GstBus *        m_Bus;
    QBasicTimer     m_Timer;

    virtual void timerEvent(QTimerEvent *event);
};

#endif // GSTBUSPOLLER_H
