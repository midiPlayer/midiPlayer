WorkerScript.onMessage = function(params) {
    if(params.action=="add"){
        params.model.append({"color":params.color});
    }
    else if(params.action=="delete"){
        params.model.remove(params.myIndex);
    }
    /*else if(params.action=="deleteAll"){
        params.model.clear();
    }
    */

    params.model.sync();
}

