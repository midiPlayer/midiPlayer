
    WorkerScript.onMessage = function(message) {
        var model = message.model;
        var device =  message.device;
        model.append({"devId":device.devId,"accepted" : message.acc_dev.indexOf(device.devId) !== -1});
        model.sync();
    }
