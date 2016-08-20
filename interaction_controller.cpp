//==============================================================================
// Project: QtQuickPlayer
// Author: Ivan Vasilev <ivan.v.vasilev at Gmail>
//
// A simple application demostrating QtQuick(QML) integration with gstreamer
//
// interaction_controller.cpp - a class to handle QML<-->C++ interaction -
//  mainly responding to user commands for now
//
//==============================================================================

//==============================================================================
// Includes
//==============================================================================
#include "interaction_controller.h"

//==============================================================================
// InteractionController Exported methods
//==============================================================================
InteractionController::InteractionController(QObject *window, GstElement* gstPlayer, bool autoplay) :
    QObject(window),
    m_GstPlayer(gstPlayer)
{
    Q_ASSERT(NULL != m_GstPlayer);
    gst_object_ref(m_GstPlayer);

    // set-up Signal->Slot connections
    m_PlayPauseControl = parent()->findChild<QQuickItem *>("playPauseControl");
    Q_ASSERT(NULL != m_PlayPauseControl);
    QObject::connect(m_PlayPauseControl, SIGNAL(playStateChanged()), this, SLOT(PlayPauseSlot()));

    QQuickItem * fullscreenControl = parent()->findChild<QQuickItem *>("fullscreenControl");
    Q_ASSERT(NULL != fullscreenControl);
    QObject::connect(fullscreenControl, SIGNAL(fullScreenClicked()), this, SLOT(FullScreenSlot()));

    if (true == autoplay)
    {
        QMetaObject::invokeMethod(m_PlayPauseControl, "play");
    }
}

InteractionController::~InteractionController() {
    // clean up when the window(our parent) is closed
    gst_element_set_state(m_GstPlayer, GST_STATE_NULL);
    gst_object_unref(m_GstPlayer);
}

void InteractionController::PlayPauseSlot() {

    if (true == m_PlayPauseControl->property("playing")) {
        qDebug("playing...");
        gst_element_set_state(m_GstPlayer, GST_STATE_PLAYING);
    } else {
        qDebug("pausing...");
        gst_element_set_state(m_GstPlayer, GST_STATE_PAUSED);
    }
}

void InteractionController::FullScreenSlot() {
    qDebug("entering fullscreen...");
    if (QQuickWindow::FullScreen != ((QQuickWindow *)parent())->visibility()) {
        ((QQuickWindow *)parent())->showFullScreen();
    } else {
        ((QQuickWindow *)parent())->showNormal();
    }
}
