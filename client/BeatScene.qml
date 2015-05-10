import QtQuick 2.0
import QtQuick.Layouts 1.1

Item{
    width: parent.width;
    height:200
    id:main

    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 20

            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Foreground trigger:")
                    color:"#fff";
                }
                TriggerSourceBtn{  }
            }
            RowLayout{
                width: parent.width
                Text{
                    text:qsTr("Background trigger:")
                    color:"#fff";
                }
                TriggerSourceBtn{  }
            }
    }



    }
