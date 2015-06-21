import QtQuick 2.4
import QtWebSockets 1.0

WebSocket {
            property string address
            id: socket
            url: address
            active: true
            onTextMessageReceived: {
                console.log(message);
            }
            onStatusChanged: {
                console.log("statusChanged");
                if (socket.status == WebSocket.Error) {
                    console.log("Error: " + socket.errorString);
                } else if (socket.status == WebSocket.Open) {
                    socket.sendTextMessage("ping");
                    console.log("socket opened");
                    mainForm.button1.enabled = true;
                    alert("asd")
                } else if (socket.status == WebSocket.Closed) {
                    console.log("socket closed");
                }
            }
            Component.onCompleted: {
                console.log("completed");
            }
        }
