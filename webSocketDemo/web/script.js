var ws = null;
var programmId;

function startSession(){
  var url = $("#addressField").val();
  if(url=="")
    return;
  $("#connectBtn").addClass("connecting");
  ws = new WebSocket("ws://" + url + "/");
  ws.onopen = function() {
  $("#loginOverlay").fadeTo(500,0,function(){$("#loginOverlay").hide();});
  $("#addressField").removeClass("wrong",100);
  $("#connectBtn").removeClass("connecting");
};

ws.onmessage = function (evt) {
    $("#slider1").val(evt.data);
};

ws.onclose = function() {
    //alert("Die Verbindung wurde getrennt");
    $("#loginOverlay").show();
    $("#loginOverlay").fadeTo(500,0.9);
};

ws.onerror = function(err) {
    //alert("Error: " + err);
    $("#addressField").addClass("wrong",100);
    $("#connectBtn").removeClass("connecting");
};
}

function valueChanged(val){
 ws.send(val)
}

function trigger(){
  ws.send("trigger");
}