WorkerScript.onMessage = function(params) {
    var msg = params.msg;
    var ret = new Object();

        if(msg.hasOwnProperty("songs")){
        params.model.clear();
        var noItem = new Object();
        noItem.text = "-- keine Musik --";
        noItem.path = "";
        params.model.append(noItem);

        for(var i = 0; i < msg.songs.length;i++){
            var song = new Object();
            song.text = msg.songs[i].name;
            song.path = msg.songs[i].path;
            params.model.append(song);
        }
        params.model.sync();
        }
}
