import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item{
    ColumnLayout{
        anchors.fill: parent
        Item{
            Layout.preferredHeight: 100
            Layout.fillWidth: true
        }

            StackView{
                Layout.fillHeight: true
                Layout.fillWidth: true
                id: colorPickerStackView
                Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true
                }
              initialItem: initial
              }
    }

    property ColorPickerView initial: ColorPickerView{
        lmodel: ListModel{ 
            ListElement{
                color: "blue"
                subcolors: '["#2020c4","#1883de","#2c58d5"]'
            }

            ListElement{
                color: "#03D1D1"
            }

            ListElement{
                color: "#16E635"
            }

            ListElement{
                color: "green"
            }

            ListElement{
                color: "yellow"
            }

            ListElement{
                color: "orange"
            }

            ListElement{
                color: "red"
            }

            ListElement{
                color: "purple"
            }

            ListElement{
                color: "grey"
            }

        }
    }
}
