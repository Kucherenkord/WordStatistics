import QtQuick

Item {
    id: root
    property alias model: repeater.model
    property int maxCount: 1

    Row {
        id: row
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
        spacing: 5;

        Repeater {
            id: repeater
            delegate: Item {
                width: root.width / repeater.count - row.spacing;
                height: root.height

                Item {
                    anchors.top: parent.top
                    anchors.bottom: wordItem.top
                    anchors.bottomMargin: 10;
                    width: parent.width

                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width
                        height: root.maxCount != 0 ? parent.height * (model.count / root.maxCount) : 0
                        color: "red"

                        Text {
                            anchors.centerIn: parent;
                            text: model.count;
                            color: "white";
                            rotation: 270
                        }
                    }
                }

                Item {
                    id: wordItem
                    anchors.bottom: parent.bottom
                    width: parent.width
                    height: 50

                    Text {
                        id: word
                        anchors.centerIn: parent
                        text: model.word
                        color: "white"
                        elide: Text.ElideRight
                        rotation: word.width >= parent.width ? 270 : 0
                    }
                }
            }
        }
    }
}
