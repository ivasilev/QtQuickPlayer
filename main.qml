import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.1

import org.freedesktop.gstreamer.GLVideoItem 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    x: 30
    y: 30
    color: "black"

    Item {
        anchors.fill: parent

        GstGLVideoItem {
            id: videoItem
            objectName: "videoItem"
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
        }
    }

    Item {
        id: fullscreenControl
        objectName: "fullscreenControl"
        width: window.width / 12
        height: window.height / 12
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        signal fullScreenClicked()

        Image {
            id: fullscreenPicture
            visible: true
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "resources/icons/fullscreen.png"

            Component.onCompleted: {
                fullscreenHidetimer.start()
            }

            Behavior on opacity {
                NumberAnimation { properties:"opacity"; duration: 500 }
            }
        }

        MouseArea {
            id: fullscreenMouseArea
            anchors.fill: fullscreenPicture
            hoverEnabled: true

            onExited: {
                fullscreenHidetimer.start()
            }

            onEntered: {
                fullscreenHidetimer.stop()
                fullscreenPicture.opacity = 1
            }

            onClicked: {
                fullscreenControl.fullScreenClicked();
                fullscreenPicture.opacity = 0;
            }
        }

        Timer {
            id: fullscreenHidetimer
            interval: 5000
            onTriggered: {
                if (!fullscreenMouseArea.containsMouse) {
                    fullscreenPicture.opacity = 0
                }
                stop()
            }
        }


    }

    Item {
        id: playPauseControl
        objectName: "playPauseControl"
        anchors.centerIn: parent
        width: parent.width / 4
        height: parent.height / 4

        property bool playing: false
        signal playStateChanged()

        function play() {
            playPauseControl.playing = true
            playPausePicture.source = "resources/icons/pause.png"
            playStateChanged()
        }

        function pause() {
            playPauseControl.playing = false
            playPausePicture.source = "resources/icons/play.png"
            playStateChanged()
        }

        function toggle() {
            if (true == playPauseControl.playing) {
                pause()
            } else {
                play()
            }
        }

        Image {
            id: playPausePicture
            visible: true
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: "resources/icons/play.png"

            // the order of onComplete calls is undefined in QML, so call play() once we know that the image is present
            Component.onCompleted: {
                playHideTimer.start()
            }

            Behavior on opacity {
                NumberAnimation { properties:"opacity"; duration: 500 }
            }
        }

        MouseArea {
            id: playpauseMouseArea
            anchors.fill: parent
            hoverEnabled: true

            onExited: {
                playHideTimer.start()
            }

            onEntered: {
                playHideTimer.stop()
                playPausePicture.opacity = 1
            }

            onClicked: {
                playHideTimer.stop()
                playPauseControl.toggle()
                playPausePicture.opacity = 1
            }
        }

        Timer {
            id: playHideTimer
            interval: 5000
            onTriggered: {
                if (!playpauseMouseArea.containsMouse) {
                    playPausePicture.opacity = 0
                }
                stop()
            }
        }
    }
}
