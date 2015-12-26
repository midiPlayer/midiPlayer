import QtQuick 2.0
import RGBWColor 1.1
import "KeyframeCreator.js" as KeyframCreator

Item{
    id:graph
    property var self: graph;
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
            r:1
            g:0
            b:0
            w:1
        }
    }
    Keyframe{
        id: keyF2
        time:6;
        value:RGBWColor{
            r:1
            g:1
            b:0
            w:0
        }
    }

    width: parent.width

    property var points: [keyF0,keyF1,keyF2];
    property var zeroPoint : Keyframe{    }
    property Keyframe activePoint: null;
    property var viewer;

    function calcPosY(point){
        return height - height * point.value.brightness
    }

    function sortPoints(){
        points = points.sort(function(a,b){return a.time - b.time});
    }

    function paint(ctx,calcPosX){
        //draw graph
        var drawedPoints = [];

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
                ctx.lineTo(calcPosX(points[i-1].time),calcPosY(points[i-1]));
                drawedPoints.push(points[i-1]);
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

        ctx.lineTo(width,height)
        ctx.closePath();

        //generate gradient
        if(drawedPoints.length > 1){
          var start = calcPosX(drawedPoints[0].time);
          var end = calcPosX(drawedPoints[drawedPoints.length - 1].time);
          var gradient = ctx.createLinearGradient(start,0,end,0);
          for(var i = 0; i < drawedPoints.length;i++){
              var point = drawedPoints[i];
              var pos = (calcPosX(point.time)-start)/(end-start);
              if(point.value.brightness > 0){
                  gradient.addColorStop(pos,point.value.preview);
              }
              else{//workarround für schwarze punkte
                  console.log(i + "ist black point");
                  if(i > 0){
                      gradient.addColorStop(pos-0.0001,drawedPoints[i-1].value.preview);
                  }
                  if(i < drawedPoints.length-1){
                      gradient.addColorStop(pos+0.0001,drawedPoints[i+1].value.preview);
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
               if(parent.activePoint != null){
                  var deltaP = wheel.angleDelta.y / 5000;
                   if(viewer.canvas.mouseA.rPressed){
                       parent.activePoint.value.r = Math.max(Math.min(parent.activePoint.value.r + deltaP,1),0)
                   }
                   if(viewer.canvas.mouseA.gPressed){
                       parent.activePoint.value.g = Math.max(Math.min(parent.activePoint.value.g + deltaP,1),0)
                   }
                   if(viewer.canvas.mouseA.bPressed){
                       parent.activePoint.value.b = Math.max(Math.min(parent.activePoint.value.b + deltaP,1),0)
                   }
                   if(viewer.canvas.mouseA.wPressed){
                       parent.activePoint.value.w = Math.max(Math.min(parent.activePoint.value.w + deltaP,1),0)
                   }
                   viewer.canvas.requestPaint();
               }
            }
        }

        function getClickedPointIndex(){
            for(var i = 0; i <parent.points.length; i++){
                var point = parent.points[i];
                var x = viewer.calcPosX(point.time);
                var y = calcPosY(point);
                if(Math.abs(mouseX - x) < 20 && Math.abs(mouseY - y) < 20){
                    return i;
                }
            }
            return null;
        }


        acceptedButtons : Qt.LeftButton | Qt.RightButton | Qt.MiddleButton;
        onPressed: {
          if(pressedButtons & Qt.LeftButton){
            console.log("pressed 2");
            console.log("index:" + parent.points[getClickedPointIndex()]);
            parent.activePoint = parent.points[getClickedPointIndex()];
            viewer.canvas.requestPaint();
          }
        }

        onClicked: {
            if(mouse.button == Qt.MiddleButton){//delete poiunt
              console.log("delete: " + getClickedPointIndex());
                parent.points.splice(getClickedPointIndex(),1);
                viewer.canvas.requestPaint();
            }
            else if(mouse.button == Qt.RightButton){
                var t = viewer.calcTime(mouseX);

                var prev_id = 0;
                var prev = parent.zeroPoint;
                if(parent.points.length != 0){
                    for(var i = 0; i < parent.points.length; i++){
                        if(viewer.calcTime(i) < t)
                           prev_id = [i];
                    }
                    prev = parent.points[prev_id];
                }

                KeyframCreator.createNewKeyframe(t,prev.value.copy,parent);
            }
        }

        onPositionChanged: {
            if(pressedButtons & Qt.LeftButton){
              if(parent.activePoint != null){
                  parent.activePoint.time = viewer.calcTime(mouseX);
                  parent.sortPoints();
                    viewer.canvas.requestPaint();
                }
            }
        }

    }



}

