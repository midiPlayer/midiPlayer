var component;
function createNewKeyframe(requestId,graph,canvas) {
    component = Qt.createComponent("Keyframe.qml");
    if (component.status === Component.Ready)
        finishKeyframeCreation(requestId,graph,canvas);
    else
        component.statusChanged.connect(function(){
            finishKeyframeCreation(requestId,graph,canvas);
    });
}

function finishKeyframeCreation(requestId,graph,canvas) {
    var keyframe;
    if (component.status === Component.Ready) {
        keyframe= component.createObject(graph, {"requestId": requestId});
        graph.points.push(keyframe);
        graph.sortPoints();
        keyframe.onTimeChanged.connect(function(){
            graph.sortPoints();
            canvas.requestPaint();
        });

        keyframe.value.onColorChanged.connect(function(){
            canvas.requestPaint();
        });

        keyframe.deleteRequested.connect(function(){
            var index = graph.points.indexOf(keyframe);
            graph.points.splice(index, 1);
            canvas.requestPaint();
            keyframe.destroy();
        });

        canvas.requestPaint();
    } else if (component.status === Component.Error) {
        console.log("Error loading component:", component.errorString());
    }
}
