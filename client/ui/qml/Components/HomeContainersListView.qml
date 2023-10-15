import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import SortFilterProxyModel 0.2

import PageEnum 1.0
import ProtocolEnum 1.0

import "../Controls2"
import "../Controls2/TextTypes"


ListView {
    id: menuContent

    property var rootWidth

    width: rootWidth
    height: menuContent.contentItem.height

    clip: true
    interactive: false

    ButtonGroup {
        id: containersRadioButtonGroup
    }

    delegate: Item {
        implicitWidth: rootWidth
        implicitHeight: content.implicitHeight

        ColumnLayout {
            id: content

            anchors.fill: parent
            anchors.rightMargin: 16
            anchors.leftMargin: 16

            VerticalRadioButton {
                id: containerRadioButton

                Layout.fillWidth: true

                text: name
                descriptionText: description

                ButtonGroup.group: containersRadioButtonGroup

                imageSource: "qrc:/images/controls/download.svg"
                showImage: !isInstalled

                checkable: isInstalled
                checked: isDefault

                onPressed: function(mouse) {
                    if (!isSupported) {
                        PageController.showErrorMessage(qsTr("The selected protocol is not supported on the current platform"))
                    }
                }

                onClicked: {
                    if (checked) {
                        var needReconnected = false
                        if (!isDefault) {
                            needReconnected = true
                        }

                        isDefault = true

                        menuContent.currentIndex = index
                        containersDropDown.menuVisible = false


                        if (needReconnected && (ConnectionController.isConnected || ConnectionController.isConnectionInProgress)) {
                            PageController.showNotificationMessage(qsTr("Reconnect via VPN Procotol: ") + name)
                            PageController.goToPageHome()
                            ConnectionController.openConnection()
                        }
                    } else {
                        ContainersModel.setCurrentlyProcessedContainerIndex(proxyContainersModel.mapToSource(index))
                        InstallController.setShouldCreateServer(false)
                        PageController.goToPage(PageEnum.PageSetupWizardProtocolSettings)
                        containersDropDown.menuVisible = false
                    }
                }

                MouseArea {
                    anchors.fill: containerRadioButton
                    cursorShape: Qt.PointingHandCursor
                    enabled: false
                }
            }

            DividerType {
                Layout.fillWidth: true
            }
        }
    }
}