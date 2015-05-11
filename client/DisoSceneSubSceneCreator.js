var placeholder;
var optionBox;
var beatScene;
var providerId;
var obj;
function createScene(requestType,ph,optionBoxP,beatSceneP,providerIdP,parent){
    beatScene = beatSceneP;
    placeholder = ph;
    optionBox = optionBoxP;
    providerId = providerIdP
    var filename = "";
    if(requestType === "beatScene1")
        filename = "BeatScene.qml";
    if(requestType === "falshScene")
        filename = "FlashScene.qml";
    if(filename != ""){
        obj = Qt.createComponent(filename,0,parent);
        if (obj.status === Component.Ready)
            finishCreation();
        else
            obj.statusChanged.connect(finishCreation);
     }
}


function finishCreation() {
    var incubator = obj.incubateObject(placeholder,{"visible":false,"requestId":providerId});
   if (incubator.status !== Component.Ready) {
       incubator.onStatusChanged = function(status) {
           if (status === Component.Ready) {
               optionBox.implicitHeight = incubator.object.height;
               beatScene = incubator.object;
           }
       }
   }
}
