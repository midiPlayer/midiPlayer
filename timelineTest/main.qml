import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import RGBWColor 1.1

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

    Keyframe{
        id: keyF0
        time:0;
        value:RGBWColor{
            r:0
            g:0
            b:0
            w:0
        }
    }

    Keyframe{
        id: keyF1
        time:2;
        value:RGBWColor{
            r:0
            g:1
            b:0
            w:0
        }
    }
    Keyframe{
        id: keyF2
        time:6;
        value:RGBWColor{
            r:1
            g:0
            b:0
            w:0
        }
    }

    Canvas {
        anchors.fill: parent;
      id:canvas
      property int x_cord : 0
      property double zoom: 1;
      property double shift: 0;
      property double cursor : 1;
      property var points: [keyF0,keyF1,keyF2];
      property var zeroPoint : Keyframe{
      }

      function calcPosX(time){
          return time*zoom*100+shift;
      }
      function calcTime(posY){
          return (posY - shift)/100 / zoom;
      }

      function calcPosY(point){
          return height - height * ((point.value.r + point.value.g + point.value.b + point.value.w) / 4);
      }

      function sortPoints(){
          points = points.sort(function(a,b){return a.time - b.time});
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
          var gradient = ctx.createLinearGradient(0,0,width,0);

          ctx.lineWidth = 4
          ctx.strokeStyle = "blue"
          // setup the fill


          ctx.beginPath()
          ctx.moveTo(0,height)

          var isFirst = true;
          var isFirstUnprint = true;
          for(var i = 0; i <points.length; i++){
            var point = points[i];

            if(calcPosX(point.time) < 0 && i != points.length - 1)
                continue;


            if(isFirst){//vorherigen punkt zeichnen
              if(i > 0){
                  ctx.moveTo(calcPosX(points[i-1].time),calcPosY(points[i-1]));
                  //gradient.addColorStop(calcPosX(points[i-1].time)/width,Qt.rgba(point.value.r,point.value.g,point.value.b,1));
                  //not working: fix: create custom color mix function
                  //fix 2 expand gradient over size:
              }



              isFirst = false;
            }

            if(calcPosX(point.time <= width)){//visible points:
                ctx.lineTo(calcPosX(point.time),calcPosY(point));
                gradient.addColorStop(calcPosX(point.time)/width,point.value.preview);
            }
            else if(isFirstUnprint){
                ctx.lineTo(calcPosX(point.time),calcPosY(point));
                isFirstUnprint = false;

            }
          }
          if(isFirstUnprint && points.length > 0){
             ctx.lineTo(width,calcPosY(points[i-1]));
          }

          ctx.lineTo(width,height)
          ctx.closePath()
          ctx.fillStyle = gradient;
          ctx.fill()

          //draw points:
          for(var i = 0; i <points.length; i++){
            var point = points[i];

            if(calcPosX(point.time) < 0)
                continue;

              ctx.beginPath();
              var x = calcPosX(point.time);
              var y = calcPosY(point);
              ctx.fillStyle = "#fff";
              ctx.moveTo(x,y);
              ctx.arc(x,y,3,0,Math.PI*2,false);
              ctx.fill();
          }

          //draw cursor;
          ctx.beginPath();
          ctx.lineWidth = 1;
          ctx.strokeStyle = "#ff0000"
          ctx.moveTo(calcPosX(cursor),0);
          ctx.lineTo(calcPosX(cursor),height);
          ctx.stroke();

      }


      MouseArea{
          anchors.fill: parent;


          property bool rPressed: false;
          property bool gPressed: false;
          property bool bPressed: false;
          property bool wPressed: false;
          onWheel: {
              console.log("wheel");
              if(wheel.modifiers & Qt.ControlModifier){//zoom
                var delta = wheel.angleDelta.y / 5000;
                  parent.zoom = Math.min(Math.max(0.1, parent.zoom + delta),2);
                  console.log(parent.zoom);
                  parent.requestPaint();
              }
              else if(wheel.modifiers & Qt.ShiftModifier){//move cursor
                var deltaS = wheel.angleDelta.y / 5000 / parent.zoom;
                parent.cursor = Math.max(0, parent.cursor + deltaS);
                  console.log(parent.cursor);
                parent.requestPaint();
              }
              else if(rPressed){
                console.log("r scroll");
              }
              else{//shift
                var deltaS = wheel.angleDelta.y / 30 / parent.zoom;
                parent.shift = Math.min(0, parent.shift + deltaS);
                parent.requestPaint();
              }
          }

          function getClickedPointIndex(){
              for(var i = 0; i <parent.points.length; i++){
                  var point = parent.points[i];
                  var x = parent.calcPosX(point.time);
                  var y = parent.calcPosY(point);
                  if(Math.abs(mouseX - x) < 20 && Math.abs(mouseY - y) < 20){
                      return i;
                  }
              }
              return null;
          }

          property var activePoint: null;
          acceptedButtons : Qt.LeftButton | Qt.RightButton | Qt.MiddleButton;
          onPressed: {
            if(pressedButtons & Qt.LeftButton){
              console.log("pressed");
              activePoint = parent.points[getClickedPointIndex()];
            }
          }
          onReleased: {
             //activePoint = null;
          }

          onClicked: {
              if(mouse.button == Qt.MiddleButton){//delete poiunt
                console.log("delete: " + getClickedPointIndex());
                  parent.points.splice(getClickedPointIndex(),1);
                  parent.requestPaint();
              }
              else if(mouse.button == Qt.RightButton){
                  var t = parent.calcTime(mouseX);

                  var prev_id = 0;
                  var prev = parent.zeroPoint;
                  if(parent.points.length != 0){
                      for(var i = 0; i < parent.points.length; i++){
                          if(parent.calcTime(i) < t)
                             prev_id = [i];
                      }
                      prev = parent.points[prev_id];
                  }


                  var newPoint = {"time":t,"value":prev.value};
                  parent.points.splice(i,0,newPoint);
                  parent.sortPoints();
                  parent.requestPaint();
              }
          }

          onPositionChanged: {
              if(pressedButtons & Qt.LeftButton){
                if(activePoint != null){
                    activePoint.time = parent.calcTime(mouseX);
                    parent.sortPoints();
                      parent.requestPaint();
                  }
              }
          }

          Keys.onPressed: {
             /* if(event.key == Qt.Key_R)
                  rPressed = true;*/
            //  console.log("down");
              //event.accepted = true;

          }
          Keys.onReleased: {
              if(event.key == Qt.Key_R)
                  rPressed = false;
              console.log("up");
          }

          focus: true;
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
