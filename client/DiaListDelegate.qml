import QtQuick 2.0
import QtQuick.Layouts 1.1
import WebSocketConnector 1.1
import QtQuick.Controls 1.2

    DragRect {
        id:listDelegate
        width:parent.width;
        height: contentRect.height;
        dragging: mouseArea.drag.active
        content:
            Item{
            width:listDelegate.width
            height: contentRect.height;

        MouseArea{
            id: mouseArea
            anchors.fill: parent;
            onPressed: {
                listDelegate.startDrag();
            }

            onClicked: {
                diaWSC.requestId = requestId;
                diaLayout.visible=true;
                diaLayout.id = id;
                diaLayout.index = index;
            }
            onDoubleClicked: {
                diaList.currentIndex = index;
                var msg = {"currentScene":id};
                wsc.send = JSON.stringify(msg);
                diaList.currentIndex = index;
            }

            drag.target: listDelegate.target
            drag.axis: Drag.YAxis
        }

        Rectangle{
            id: contentRect;
            color:"#00000000"//transparent
            anchors.fill: parent
            height:text.height + 20

            states: [
                State {
                    when: dragging
                    PropertyChanges {
                        target: contentRect
                        color:"#33000000"
                    }
                }]

        RowLayout{
            anchors.fill: parent;
                Text{
                    anchors.centerIn: parent
                    width: parent.parent.width;
                    Layout.maximumWidth: parent.width;
                    id:text
                    text:name;
                    color:"#369cb6"
                    font.pointSize: 15;
                    wrapMode: Text.WrapAnywhere;
                }
            }
        }
    }
}

