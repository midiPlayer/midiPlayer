WorkerScript.onMessage = function(message) {
    var model = message.model;
    var devId =  message.devId;
    if(message.action === "add")
        model.append({"devId" : devId});
    else if(message.action === "del"){
        for(var i = 0; i < model.count;i++) {
            if(model.get(i).devId === devId){
                model.remove(i);
                break;
            }
        }
    }

    model.sync();
}
