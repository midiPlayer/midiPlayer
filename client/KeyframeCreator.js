var component;
function createNewKeyframe(time, value,graph) {
    component = Qt.createComponent("Keyframe.qml");
    if (component.status === Component.Ready)
        finishKeyframeCreation(time,value,graph);
    else
        component.statusChanged.connect(function(){
            finishKeyframeCreation(time,value,graph);
    });
}

function finishKeyframeCreation(time,value,graph) {
    var keyframe;
    if (component.status === Component.Ready) {
        keyframe= component.createObject(self, {"time": time, "value": value});
        graph.points.push(keyframe);
        graph.sortPoints();
        graph.viewer.canvas.requestPaint();
    } else if (component.status === Component.Error) {
        console.log("Error loading component:", component.errorString());
    }
}
