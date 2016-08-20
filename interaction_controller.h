//==============================================================================
// Project: QtQuickPlayer
// Author: Ivan Vasilev <ivan.v.vasilev at Gmail>
//
// A simple application demostrating QtQuick(QML) integration with gstreamer
//
// interaction_controller.h - a class to handle QML<-->C++ interaction -
//  mainly responding to user commands for now
//
//==============================================================================
#ifndef INTERACTIONCONTROLLER_H
#define INTERACTIONCONTROLLER_H


//==============================================================================
// Includes
//==============================================================================
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
    void PlayPauseSlot();       // triggered when the user presses the play/pause control
    void FullScreenSlot();      // triggered when the user presses the fullscreen control

private:
    GstElement*     m_GstPlayer;
    QQuickItem*     m_PlayPauseControl;

};

#endif // INTERACTIONCONTROLLER_H
