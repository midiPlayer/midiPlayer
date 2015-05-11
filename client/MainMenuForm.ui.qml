import QtQuick 2.4

Item {
    width: 400
    height: 400


    GridView {
        id: gridView1
        interactive: false
        contentHeight: 70
        anchors.fill: parent
        model: ListModel {
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
            ListElement {
                name: "Green"
                colorCode: "yellow"
            }
            ListElement {
                name: "Grey"
                colorCode: "grey"
            }

            ListElement {
                name: "Red"
                colorCode: "red"
            }

            ListElement {
                name: "Blue"
                colorCode: "blue"
            }

            ListElement {
                name: "Green"
                colorCode: "green"
            }
        }
        cellWidth:200
        cellHeight: 200
        delegate: Item {
            //height: 50
            Column {
                Rectangle {
                    width: 30
                    height: 30
                    color: colorCode
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Text {
                    x: 5
                    text: name
                    font.bold: true
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                spacing: 5
            }
        }
    }
}

