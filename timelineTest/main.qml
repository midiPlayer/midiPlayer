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

    color:"#000";

    Canvas {
        anchors.fill: parent;
      id:canvas
      property int x_cord : 0
      property double zoom: 1;
      property double shift: 0;
      property var points: [{"time":0,"value":{"r":0,"g":0,"b":0,"w":0}},{"time":5,"value":{"r":1,"g":0,"b":0,"w":0}}];

      function calcPosX(time){
          return time*zoom*100+shift;
      }

      onPaint:{
         var ctx = canvas.getContext('2d');
          ctx.reset();

          //draw time marks:
          ctx.lineWidth = 0.5;
          ctx.strokeStyle = "#222"

          ctx.beginPath();
          for(var i = 0;calcPosX(i) < width;i++){
              ctx.moveTo(calcPosX(i),0);
              ctx.lineTo(calcPosX(i),height);
              ctx.stroke();
          }

          //draw graph
          ctx.lineWidth = 4
          ctx.strokeStyle = "blue"
          // setup the fill
          ctx.fillStyle = "steelblue"

          ctx.beginPath()
          ctx.moveTo(0,height)

          var isFirst = true;
          var isFirstUnprint = true;
          for(var i = 0; i <points.length; i++){
            var point = points[i];

            if(calcPosX(point.time) < 0 && i != points.length - 1)
                continue;

            var per  = (point.value.r + point.value.g + point.value.b + point.value.w) / 4;


            if(isFirst){//vorherigen punkt zeichnen
              if(i > 0){
                  var per_prev  = (points[i-1].value.r + points[i-1].value.g + points[i-1].value.b + points[i-1].value.w) / 4;
                  ctx.moveTo(calcPosX(points[i-1].time),height - height * per_prev);
                  //ctx.moveTo(calcPosX(-1),height);

              }else{
               //ctx.moveTo(calcPosX(0),height - height * per);
              }

              isFirst = false;
            }

            if(calcPosX(point.time <= width)){
                ctx.lineTo(calcPosX(point.time),height-height*per);
            }
            else if(isFirstUnprint){
                ctx.lineTo(calcPosX(point.time),height-height*per);
                isFirstUnprint = false;

            }
          }
          if(isFirstUnprint && points.length > 0){
             var per_prev  = (points[i-1].value.r + points[i-1].value.g + points[i-1].value.b + points[i-1].value.w) / 4;
             ctx.lineTo(width,height-height*per);
          }

          ctx.lineTo(width,height)

                  // left line through path closing
                  ctx.closePath()
                  // fill using fill style
                  ctx.fill()
                  // stroke using line width and stroke style
                  //ctx.stroke();

      }
      MouseArea{
          anchors.fill: parent;
          onPositionChanged:updateD();
          function updateD(){
              console.log(mouseX);
              if(mouseX >= 0)
                  parent.x_cord = mouseX;
                parent.requestPaint();
          }

          onWheel: {
              if(wheel.modifiers & Qt.ControlModifier){//zoom
                var delta = wheel.angleDelta.y / 5000;
                  parent.zoom = Math.min(Math.max(0.1, parent.zoom + delta),2);
                  console.log(parent.zoom);
                  parent.requestPaint();
              }
              if(wheel.modifiers & Qt.ShiftModifier){
                var deltaS = wheel.angleDelta.y / 30 / parent.zoom;
                parent.shift = Math.min(0, parent.shift + deltaS);
                  parent.requestPaint();
              }
          }

          onClicked: {

          }

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
