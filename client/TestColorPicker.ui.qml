import QtQuick 2.4

Item {
    id: item1
    width: 400
    height: 400

    Row {
        id: row1
        x: 0
        width: 640
        height: 60
        anchors.top: parent.top
        anchors.topMargin: 0

        TextInput {
            id: textInput1
            x: 15
            y: 18
            width: 268
            height: 42
            text: qsTr("Ausgewählte Farben:")
            horizontalAlignment: Text.AlignLeft
            font.bold: true
            font.pointSize: 16
            transformOrigin: Item.TopLeft
        }
    }

    Row {
        id: row5
        x: 14
        y: 75
        width: 610
        height: 156

        ColorButton {
            id: blau
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#8478F2" }
                GradientStop { position: 1.0; color: "#0F0287" }
            }
        }

        Column {
            id: column
            width: 20
            height: 156
        }

        ColorButton {
            id: tuerkis
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#AEFCFC" }
                GradientStop { position: 1.0; color: "#03D1D1" }
            }
        }

        Column {
            id: column2
            width: 20
            height: 156
        }

        ColorButton {
            id: hellgruen
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#B4FCBE" }
                GradientStop { position: 1.0; color: "#16E635" }
            }
        }
    }

    Row {
        id: row3
        x: 15
        y: 251
        width: 610
        height: 156

        ColorButton {
            id: gruen
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#5ADA2F" }
                GradientStop { position: 1.0; color: "#134C00" }
            }
        }

        Column {
            id: column3
            width: 20
            height: 156
        }

        ColorButton {
            id: gelb
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FEFF8B" }
                GradientStop { position: 1.0; color: "#FCFF00" }
            }
        }

        Column {
            id: column4
            width: 20
            height: 156
        }

        ColorButton {
            id: orange
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FFC670" }
                GradientStop { position: 1.0; color: "#FC9700" }
            }
        }
    }

    Row {
        id: row4
        x: 15
        y: 428
        width: 610
        height: 155

        ColorButton {
            id: rot
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FC706B" }
                GradientStop { position: 1.0; color: "#CF0902" }
            }
        }

        Column {
            id: column5
            width: 20
            height: 156
        }

        ColorButton {
            id: lila
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FE8DDA" }
                GradientStop { position: 1.0; color: "#D0028F" }
            }
        }

        Column {
            id: column6
            width: 20
            height: 156
        }

        ColorButton {
            id: schwarzweiss
            width: 190
            height: 156
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#FFFFFF" }
                GradientStop { position: 1.0; color: "#000000" }
            }
        }
    }
}
