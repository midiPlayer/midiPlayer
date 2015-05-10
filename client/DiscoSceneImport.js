WorkerScript.onMessage = function(params) {
    var msg = params.msg;
    if(msg.effects !== undefined){
        for (var z = 0; z < msg.effects.length; z++){
            var effect = msg.effects[z];
            console.log(JSON.stringify(effect));
            var md = new Object();
            md.modelData = effect;
            params.listModel.append(md);//TODO add update function
        }
        params.listModel.sync();
    }
}


