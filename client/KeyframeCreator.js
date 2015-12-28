var component;
function createNewKeyframe(time, value,graph,canvas) {
    component = Qt.createComponent("Keyframe.qml");
    if (component.status === Component.Ready)
        finishKeyframeCreation(time,value,graph,canvas);
    else
        component.statusChanged.connect(function(){
            finishKeyframeCreation(time,value,graph,canvas);
    });
}

function finishKeyframeCreation(time,value,graph,canvas) {
    var keyframe;
    if (component.status === Component.Ready) {
        keyframe= component.createObject(self, {"time": time, "value": value});
        graph.points.push(keyframe);
        graph.sortPoints();
        canvas.requestPaint();
    } else if (component.status === Component.Error) {
        console.log("Error loading component:", component.errorString());
    }
}
