import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "TextTypes"

Popup {
    id: root

    property string text
    property bool closeButtonVisible: true

    leftMargin: 25
    rightMargin: 25
    bottomMargin: 70

    width: parent.width - leftMargin - rightMargin

    anchors.centerIn: parent
    modal: root.closeButtonVisible
    closePolicy: Popup.CloseOnEscape

    Overlay.modal: Rectangle {
        visible: root.closeButtonVisible
        color: Qt.rgba(14/255, 14/255, 17/255, 0.8)
    }

    onOpened: {
        focusItem.forceActiveFocus()
    }

    onClosed: {
        PageController.forceStackActiveFocus()
    }

    background: Rectangle {
        anchors.fill: parent

        color: "white"
        radius: 4
    }

    contentItem: Item {
        implicitWidth: content.implicitWidth
        implicitHeight: content.implicitHeight

        anchors.fill: parent

        RowLayout {
            id: content

            anchors.fill: parent
            anchors.leftMargin: 16
            anchors.rightMargin: 16

            CaptionTextType {
                horizontalAlignment: Text.AlignLeft
                Layout.fillWidth: true

                onLinkActivated: function(link) {
                    Qt.openUrlExternally(link)
                }

                text: root.text

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.NoButton
                    cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                }
            }

            Item {
                id: focusItem
                KeyNavigation.tab: closeButton
            }

            BasicButtonType {
                id: closeButton
                visible: closeButtonVisible

                implicitHeight: 32

                defaultColor: "white"
                hoveredColor: "#C1C2C5"
                pressedColor: "#AEB0B7"
                disabledColor: "#494B50"

                textColor: "#0E0E11"
                borderWidth: 0

                text: qsTr("Close")
                KeyNavigation.tab: focusItem

                clickedFunc: function() {
                    root.close()
                }
            }
        }
    }
}
