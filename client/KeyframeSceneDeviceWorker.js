WorkerScript.onMessage = function(message) {
    var model = message.model;
    var lamps =  message.lamps;
    model.clear();
    for(var i = 0; i < lamps.length;i++){
        model.append(lamps[i]);
    }
    model.sync();
}
