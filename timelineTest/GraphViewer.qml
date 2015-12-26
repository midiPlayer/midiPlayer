import QtQuick 2.0

Rectangle{
    id:graphViewer

    property double zoom: 1;
    property double shift: 0;
    property double cursor : 1;
    property alias canvas: canvas


    Graph{
        id: graph1;
        anchors.top: parent.top;
        viewer: graphViewer
        height: 100;
    }
    Graph{
        id: graph2;
        anchors.top: graph1.bottom;
        viewer: graphViewer
        height: 100;
    }

    property var graphs: [graph1,graph2];

    color:"#000";

    anchors.fill: parent

    Rectangle
    {
       id:keyfEditMessage
       visible: false
       z:5
       anchors.centerIn: parent
       width: parent.width
       height: 50
       opacity: 0.7
       color:"#000"
       Text{
           text: qsTr("Change values (%1) by scrolling  <Esc> for exit").arg(parent.colors);
           color:"#fff"
           anchors.centerIn: parent
           font.pointSize: 13
       }

       property string colors: ""
    }

    function calcPosX(time){
        return time*zoom*100+shift;
    }
    function calcTime(posY){
        return (posY - shift)/100 / zoom;
    }

    Canvas {
        anchors.fill: parent;
      id:canvas


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


          //draw Graphs
          for(var i = 0; i < graphs.length; i++){
              var graph = graphs[i];
              graph.paint(ctx,parent.calcPosX);
          }


          //draw cursor;
          ctx.beginPath();
          ctx.lineWidth = 1;
          ctx.strokeStyle = "#ff0000"
          ctx.moveTo(calcPosX(cursor),0);
          ctx.lineTo(calcPosX(cursor),height);
          ctx.stroke();

      }

      property alias mouseA: mouseA

      MouseArea{
          id: mouseA
          anchors.fill: parent;

          focus:true;


          property bool rPressed: false;
          property bool gPressed: false;
          property bool bPressed: false;
          property bool wPressed: false;
          onWheel: {
              if(rPressed || gPressed || bPressed || wPressed){
                //ignore; event is handeled in Graph
                  wheel.accepted = false;
              }
              else if(wheel.modifiers & Qt.ControlModifier){//zoom
                var delta = wheel.angleDelta.y / 5000;
                  parent.parent.zoom = Math.min(Math.max(0.1, parent.parent.zoom + delta),2);
                  console.log(parent.parent.zoom);
                  parent.requestPaint();
              }
              else if(wheel.modifiers & Qt.ShiftModifier){//move cursor
                var deltaS = wheel.angleDelta.y / 5000 / parent.parent.zoom;
                parent.parent.cursor = Math.max(0, parent.parent.cursor + deltaS);
                  console.log(parent.parent.cursor);
                parent.requestPaint();
              }
              else{//shift
                var deltaS = wheel.angleDelta.y / 30 / parent.parent.zoom;
                parent.parent.shift = Math.min(0, parent.parent.shift + deltaS);
                parent.requestPaint();
              }
          }

          function getClickedPointIndex(){
              for(var i = 0; i <parent.points.length; i++){
                  var point = parent.points[i];
                  var x = parent.calcPosX(point.time);
                  var y = point.value.brightness;
                  if(Math.abs(mouseX - x) < 20 && Math.abs(mouseY - y) < 20){
                      return i;
                  }
              }
              return null;
          }


          acceptedButtons : Qt.LeftButton | Qt.RightButton | Qt.MiddleButton;
          onPressed: {
            mouse.accepted = false;
          }

          onClicked: {
              mouse.accepted = false;
          }

          onPositionChanged: {
              mouse.accepted = false;
          }

          Keys.onPressed: {
              event.accepted = true;
              console.log("keyEvent");

              if(event.key === Qt.Key_R)
                rPressed = !rPressed;
              if(event.key === Qt.Key_G)
                gPressed = !gPressed;
              if(event.key === Qt.Key_B)
                bPressed = !bPressed;
              if(event.key === Qt.Key_W)
                wPressed = !wPressed;
              if(event.key === Qt.Key_Escape)
                rPressed = gPressed = bPressed = wPressed = false;
              if(rPressed || gPressed || bPressed | wPressed){
                  keyfEditMessage.colors = (rPressed ? (qsTr("red") + " ") : "") + (gPressed ? (qsTr("green") + " ") : "") + (bPressed ? (qsTr("blue") + " ") : "") + (wPressed ? (qsTr("white")) : "");
                  keyfEditMessage.visible = true;
              }
              else{
                  keyfEditMessage.visible = false;
              }
          }

      }

    }


}

