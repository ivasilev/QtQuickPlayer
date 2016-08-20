#ifndef INTERACTIONCONTROLLER_H
#define INTERACTIONCONTROLLER_H

#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>

#include "gst/gst.h"

class InteractionController : public QObject
{
    Q_OBJECT
public:
    explicit InteractionController(QObject *window, GstElement* gstPlayer, bool autoplay);
    ~InteractionController();

signals:
    // none at that point

public slots:
    void PlayPauseSlot();
    void FullScreenSlot();

private:
    GstElement*     m_GstPlayer;
    QQuickItem*     m_PlayPauseControl;

};

#endif // INTERACTIONCONTROLLER_H
