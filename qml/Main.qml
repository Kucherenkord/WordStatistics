import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("File Processor")

    property alias model: histogram.model
    property alias maxCount: histogram.maxCount
    property alias state: stateGroup.state

    property alias progressMax: progressBar.to
    property alias progressValue: progressBar.value

    signal open(string fileName)
    signal start()
    signal pause()
    signal resume()
    signal cancel()

    onStateChanged: {
        console.log("onStateChanged", state)
    }

    onProgressMaxChanged: {
        console.log("onProgressMaxChanged", progressMax)

    }

    onProgressValueChanged:  {
        console.log("onProgressValueChanged", progressValue)

    }

    StateGroup {
        id: stateGroup
        state: "default"
        states: [
            State {
                name: "default"

                PropertyChanges {
                    target: histogram
                    visible: false
                }

                PropertyChanges {
                    target: mainText
                    text: qsTr("No data, please load file for analysis data")
                }

                PropertyChanges {
                    target: progressBar
                    visible: false
                }

                PropertyChanges {
                    target: openButton
                    enabled: true
                }

                PropertyChanges {
                    target: startButton
                    enabled: false
                }

                PropertyChanges {
                    target: pauseButton
                    enabled: false
                }

                PropertyChanges {
                    target: cancelButton
                    enabled: false
                }
            },

            State {
                name: "loaded"

                PropertyChanges {
                    target: mainText
                    text: qsTr("File loaded, please start processing")
                }

                PropertyChanges {
                    target: progressBar
                    visible: false
                }

                PropertyChanges {
                    target: openButton
                    enabled: false
                }

                PropertyChanges {
                    target: startButton
                    enabled: true
                }

                PropertyChanges {
                    target: pauseButton
                    enabled: false
                }

                PropertyChanges {
                    target: cancelButton
                    enabled: false
                }
            },

            State {
                name: "processing"

                PropertyChanges {
                    target: histogram
                    visible: false
                }

                PropertyChanges {
                    target: progressBar
                    visible: true
                }

                PropertyChanges {
                    target: mainText
                    text: qsTr("File processing, please wait ...")
                }

                PropertyChanges {
                    target: openButton
                    enabled: false
                }

                PropertyChanges {
                    target: startButton
                    enabled: false
                }

                PropertyChanges {
                    target: pauseButton
                    enabled: true
                }

                PropertyChanges {
                    target: pauseButton
                    text: qsTr("Pause")
                }

                PropertyChanges {
                    target: cancelButton
                    enabled: true
                }
            },

            State {
                name: "processed"

                PropertyChanges {
                    target: histogram
                    visible: true
                }

                PropertyChanges {
                    target: progressBar
                    visible: false
                }

                PropertyChanges {
                    target: mainText
                    visible: false
                }

                PropertyChanges {
                    target: openButton
                    enabled: false
                }

                PropertyChanges {
                    target: startButton
                    enabled: false
                }

                PropertyChanges {
                    target: pauseButton
                    enabled: false
                }

                PropertyChanges {
                    target: cancelButton
                    enabled: true
                }
            },

            State {
                name: "paused"

                PropertyChanges {
                    target: histogram
                    visible: false
                }

                PropertyChanges {
                    target: progressBar
                    visible: true
                }

                PropertyChanges {
                    target: mainText
                    visible: true
                }

                PropertyChanges {
                    target: mainText
                    text: qsTr("Paused file processing")
                }

                PropertyChanges {
                    target: openButton
                    enabled: false
                }

                PropertyChanges {
                    target: startButton
                    enabled: false
                }

                PropertyChanges {
                    target: pauseButton
                    text: qsTr("Resume")
                }

                PropertyChanges {
                    target: cancelButton
                    enabled: true
                }
            }
        ]
    }

    Item {
        anchors.top: parent.top
        anchors.bottom: progressBar.top
        width: parent.width

        Text {
            id: mainText
            anchors.centerIn: parent
            color: "white"
        }
    }

    Histogram {
        id: histogram;
        anchors.top: parent.top
        anchors.bottom: progressBar.top
        width: parent.width
    }

    ProgressBar {
        id: progressBar
        anchors.bottom: footer.top
        width: parent.width
        height: 30
        from: 0
    }

    RowLayout {
        id: footer
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: 100

        Button {
            id: openButton
            text: qsTr("Open")
            onClicked: {
                fileDialog.open()
            }
        }

        Button {
            id: startButton
            text: qsTr("Start")
            onClicked: {
                root.start()
            }
        }

        Button {
            id: pauseButton
            text: qsTr("Pause")
            onClicked: {
                if (stateGroup.state == "paused"){
                    root.resume()
                }
                else{
                    root.pause()
                }
            }
        }

        Button {
            id: cancelButton
            text: qsTr("Cancel")
            onClicked: {
                root.cancel()
            }
        }
    }

    FileDialog {
        id: fileDialog
        currentFolder: StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
        onAccepted: {
            let fileName = fileDialog.currentFile.toString().replace("file://", "");
            console.log(fileDialog.currentFile.toString())

            root.open(fileName)
        }
    }
}
