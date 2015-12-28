import QtQuick 2.0
import RGBWColor 1.1
import "KeyframeCreator.js" as KeyframCreator
import WebSocketConnector 1.1
import QtQuick.Layouts 1.1

Item{
    id:graph
    //color:"#000";
    property var self: graph;
    //property string deviceRequestId;
    property string devideID;
    property alias requestId : ws.requestId

    function repaint(){
        graphCanvas.requestPaint();
    }

    width: parent.width
    onHeightChanged: {
        console.log("height" + height);
    }

    onDevideIDChanged: {
        //points = [];
        var msg = new Object();
        msg.get_keyframes = devId;
        ws.send = JSON.stringify(msg);
    }

    //width: parent.width

    property var points: [];
    property var zeroPoint : Keyframe{    }
    property Keyframe activePoint: null;
    property var viewer;

    function calcPosY(point){
        return height - height * point.value.brightness
    }

    function getAbsHeight(height){
        return height;
    }


    function sortPoints(){
        points = points.sort(function(a,b){return a.time - b.time});
    }

    Canvas {
        id: graphCanvas
        anchors.fill: parent;
        onPaint: {
            var ctx = graphCanvas.getContext('2d');
             ctx.reset();
            var clacPosx = viewer.calcPosX;

            //draw graph
            var drawedPoints = [];

            ctx.lineWidth = 4
            ctx.strokeStyle = "blue"
            // setup the fill


            ctx.beginPath()
            ctx.moveTo(0,getAbsHeight(height))

            var isFirst = true;
            var isFirstUnprint = true;

            for(var i = 0; i <points.length; i++){
              var point = points[i];

              if(calcPosX(point.time) < 0 && i != points.length - 1)
                  continue;


              if(isFirst){//vorherigen punkt zeichnen
                if(i > 0){
                    ctx.lineTo(calcPosX(points[i-1].time),calcPosY(points[i-1]));
                    drawedPoints.push(points[i-1]);
                }
                else{
                    ctx.lineTo(0,calcPosY(points[i]));
                }



                isFirst = false;
              }

              if(calcPosX(point.time <= width)){//visible points:
                  ctx.lineTo(calcPosX(point.time),calcPosY(point));
                  drawedPoints.push(point);
              }
              else if(isFirstUnprint){
                  ctx.lineTo(calcPosX(point.time),calcPosY(point));
                  drawedPoints.push(point);
                  isFirstUnprint = false;

              }
            }
            if(isFirstUnprint && points.length > 0){
               ctx.lineTo(width,calcPosY(points[i-1]));
                drawedPoints.push(points[i-1]);
            }

            ctx.lineTo(width,getAbsHeight(height));
            ctx.closePath();

            //generate gradient
            if(drawedPoints.length > 1){
              var start = calcPosX(drawedPoints[0].time);
              var end = calcPosX(drawedPoints[drawedPoints.length - 1].time);
              var gradient = ctx.createLinearGradient(start,0,end,0);
              for(var i = 0; i < drawedPoints.length;i++){
                  var point = drawedPoints[i];
                  var pos = 0;
                  if(end-start != 0)
                    pos = (calcPosX(point.time)-start)/(end-start);
                  if(point.value.brightness > 0){
                      gradient.addColorStop(pos,point.value.preview);
                  }
                  else{//workarround für schwarze punkte
                      if(i > 0){
                          gradient.addColorStop(Math.max(Math.min(pos-0.0001,1),0),drawedPoints[i-1].value.preview);
                      }
                      if(i < drawedPoints.length-1){
                          gradient.addColorStop(Math.max(Math.min(pos+0.0001,1),0),drawedPoints[i+1].value.preview);
                      }
                  }

              }
            }

            ctx.fillStyle = gradient;
            ctx.fill()

            //draw points:
            for(var i = 0; i < points.length; i++){
              var point = points[i];

              if(calcPosX(point.time) < 0)
                  continue;

                ctx.beginPath();
                var x = calcPosX(point.time);
                var y = calcPosY(point);
                ctx.fillStyle = "#fff";
                ctx.moveTo(x,y);
                var radius = 3;
                if(point === activePoint)
                    radius = 5;
                ctx.arc(x,y,radius,0,Math.PI*2,false);
                ctx.fill();
            }

        }


    MouseArea{
        anchors.fill: parent;
        onWheel: {
            if(viewer.canvas.mouseA.rPressed || viewer.canvas.mouseA.gPressed || viewer.canvas.mouseA.bPressed | viewer.canvas.mouseA.wPressed){
               if(parent.parent.activePoint != null){
                  var deltaP = wheel.angleDelta.y / 5000;
                   if(viewer.canvas.mouseA.rPressed){
                       parent.parent.activePoint.value.r = Math.max(Math.min(parent.parent.activePoint.value.r + deltaP,1),0)
                   }
                   if(viewer.canvas.mouseA.gPressed){
                       parent.parent.activePoint.value.g = Math.max(Math.min(parent.parent.activePoint.value.g + deltaP,1),0)
                   }
                   if(viewer.canvas.mouseA.bPressed){
                       parent.parent.activePoint.value.b = Math.max(Math.min(parent.parent.activePoint.value.b + deltaP,1),0)
                   }
                   if(viewer.canvas.mouseA.wPressed){
                       parent.parent.activePoint.value.w = Math.max(Math.min(parent.parent.activePoint.value.w + deltaP,1),0)
                   }
                   parent.requestPaint();
               }
            }
        }

        function getClickedPointIndex(){
            for(var i = 0; i <parent.parent.points.length; i++){
                var point = parent.parent.points[i];
                var x = viewer.calcPosX(point.time);
                var y = calcPosY(point);
                if(Math.abs(mouseX - x) < 20 && Math.abs(mouseY - y) < 20){
                    return i;
                }
            }
            return -1;
        }


        acceptedButtons : Qt.LeftButton | Qt.RightButton | Qt.MiddleButton;
        onPressed: {
          if(pressedButtons & Qt.LeftButton){
            var index = getClickedPointIndex()
            if(index !== -1){
                parent.parent.activePoint = parent.parent.points[index];
                parent.requestPaint();
            }
          }
        }

        onClicked: {
            if(mouse.button == Qt.MiddleButton){//delete poiunt
                    var index = getClickedPointIndex();
                    if(index !== -1){
                    var keyframe = parent.parent.points[index]
                    parent.parent.points.splice(index,1);
                    keyframe.requestDelete();
                    keyframe.destroy();
                    parent.requestPaint();
                }
            }
            else if(mouse.button == Qt.RightButton){//add Point
                var t = viewer.calcTime(mouseX);

                var msg = new Object();
                msg.add_keyframe = new Object();
                msg.add_keyframe.devId = graph.devideID;
                msg.add_keyframe.time = t;
                ws.send = JSON.stringify(msg);

                //KeyframCreator.createNewKeyframe(t,prev.value.copy,graph,graphCanvas);
            }
        }

        propagateComposedEvents: true
        onPositionChanged: {
            if(pressedButtons & Qt.LeftButton){
              if(parent.parent.activePoint != null){
                  parent.parent.activePoint.setTime(Math.max(0,viewer.calcTime(mouseX)));
                }
            }
        }

    }

    }

     WebSocketConnector{
         id: ws
         onMessage: {
             if(msg.devId === devideID && msg.hasOwnProperty("keyframes")){
                for(var i = 0; i < msg.keyframes.length; i++){
                    KeyframCreator.createNewKeyframe(msg.keyframes[i],graph,graphCanvas);
                }
             }
             if(msg.devId === devideID && msg.hasOwnProperty("new_keyframe")){
                    KeyframCreator.createNewKeyframe(msg.new_keyframe,graph,graphCanvas);
             }
         }
     }

}

