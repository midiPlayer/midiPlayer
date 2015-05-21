WorkerScript.onMessage = function(params) {
    var msg = params.msg;
    if(msg.effectList !== undefined){
        console.log("import started");
        for (var z = 0; z < msg.effectList.length; z++){
            var effect = msg.effectList[z];
            console.log(JSON.stringify(effect));
            var md = new Object();
            md.modelData = effect;
            params.listModel.append(md);//TODO add update function
        }
        params.listModel.sync();
    }
}


