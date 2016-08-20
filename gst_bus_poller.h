#ifndef GSTBUSPOLLER_H
#define GSTBUSPOLLER_H

#include <QObject>
#include <QRunnable>
#include <QBasicTimer>
#include <QTimerEvent>

#include "gst/gst.h"

// not used in the project right now, nominal use case is
// GstElement *pipeline = gst_pipeline_new ("playpipeline");
// playPipelinePoller = new GstBusPoller(gst_pipeline_get_bus(GST_PIPELINE (pipeline)), "playPipelinePoller");

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
