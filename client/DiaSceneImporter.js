
WorkerScript.onMessage = function(params) {
    var msg = params.msg;
    if(msg.scenes !== undefined){
        params.listModel.clear();
        console.log("import started");
        for (var z = 0; z < msg.scenes.length; z++){
            var effect = msg.scenes[z];
            console.log(JSON.stringify(effect));
            params.listModel.append(effect);
        }
        params.listModel.sync();
    }

    if(msg.hasOwnProperty("orderChanged")){
        for(var i = 0; i<msg.orderChanged.length;i++){
            var id = msg.orderChanged[i];
            if(params.listModel.get(i).id !== id){
                for(var y = i;y < params.listModel.count; y++){
                    if(params.listModel.get(y).id === id){
                        params.listModel.move(y,i,1);
                    }
                }
            }
        }
        params.listModel.sync();
    }
    if(msg.deleteScene !== undefined){
           params.listModel.remove(msg.deleteScene);
        params.listModel.sync();
    }

}


