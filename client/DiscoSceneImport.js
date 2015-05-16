WorkerScript.onMessage = function(params) {
    var msg = params.msg;
    if(msg.effects !== undefined){
        console.log("import started");
        for (var z = 0; z < msg.effects.length; z++){
            var effect = msg.effects[z];
            console.log(JSON.stringify(effect));
            var fusionTypeModel = params.fusionTypeModel;
            for(var i  =0; i < fusionTypeModel.count;i++){
                if(fusionTypeModel.get(i).name === effect.fusionType){
                    effect.fusionTypeId = i;
                    break;
                }
            }
            var md = new Object();
            md.modelData = effect;
            params.listModel.append(md);//TODO add update function
        }
        params.listModel.sync();
    }
    if(msg.orderChanged !== undefined){
        for(var i = 0; i<msg.orderChanged.length;i++){
            var id = msg.orderChanged[i];
            if(params.listModel.get(i).modelData.sceneId !== id){
                for(var y = i;y < params.listModel.count; y++){
                    if(params.listModel.get(y).modelData.sceneId === id){
                        params.listModel.move(y,i,1);
                    }
                }
            }
        }
        params.listModel.sync();
    }

}


