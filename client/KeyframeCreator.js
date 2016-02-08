var component;
function createNewKeyframe(requestId,graph,canvas,whiteDeviceColor,channels) {
    component = Qt.createComponent("Keyframe.qml");
    if (component.status === Component.Ready)
        finishKeyframeCreation(requestId,graph,canvas,whiteDeviceColor,channels);
    else
        component.statusChanged.connect(function(){
            finishKeyframeCreation(requestId,graph,canvas,whiteDeviceColor,channels);
    });
}

function finishKeyframeCreation(requestId,graph,canvas,whiteDeviceColor,channels) {
    var keyframe;
    if (component.status === Component.Ready) {
        keyframe= component.createObject(graph, {"requestId": requestId});

        keyframe.value.deviceWhiteColor = whiteDeviceColor;
        keyframe.value.hasR = channels.hasOwnProperty("r");
        keyframe.value.hasG = channels.hasOwnProperty("g");
        keyframe.value.hasB = channels.hasOwnProperty("b");
        keyframe.value.hasW = channels.hasOwnProperty("w");

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
