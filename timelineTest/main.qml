import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }



    Canvas {
        anchors.fill: parent;
      id:canvas
      onPaint:{
         var ctx = canvas.getContext('2d');
          ctx.lineWidth = 4
          ctx.strokeStyle = "blue"
          // setup the fill
          ctx.fillStyle = "steelblue"

          ctx.fillRect(0, 0, 1, 1);
          ctx.beginPath()
                  // top-left start point
                  ctx.moveTo(0,height)
                  // right line
                  ctx.lineTo(150,150)
                  // bottom line
                  ctx.lineTo(width,height)
                  // left line through path closing
                  ctx.closePath()
                  // fill using fill style
                  ctx.fill()
                  // stroke using line width and stroke style
                  ctx.stroke();
      }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }
}
