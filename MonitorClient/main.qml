import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Controls 1.2
import QtMultimedia 5.5
import MediaManager 1.1
import WebSocketConnector 1.1

ApplicationWindow {
    id:mainWindow
    objectName: "mainWindow"
    visible: true
    width: desktopWidth
    height: desktopHeight
    title: qsTr("Video Player")

    property int fadeDur: 1000

    //visibility: "FullScreen"


    MediaPlayer {
            id: player
            source: "file:///home/jakob/Videos/00190002.MOV"
            autoPlay: true
            onError: {
                console.log(errorString);
            }
            muted: true

            autoLoad: true;

            loops: MediaPlayer.Infinite


        }

    Rectangle{
        id:display;
        anchors.fill: parent;

        color:"red";

    SlideImage{
        id:behindVideoImage
        fadeDur: mainWindow.fadeDur
    }

    VideoOutput {
        id: videoOutput2
        source: player
        fillMode: VideoOutput.PreserveAspectCrop;
        anchors.fill: parent;
        opacity: 1

        function fadeOut(){
            videoFadeOut.running = true;
        }

        PropertyAnimation {
            id: videoFadeOut;
            target: videoOutput2;
            property: "opacity";
            to: 0;
            duration: fadeDur;

            onRunningChanged: {
                if(running == false){
                    player.stop();
                }
            }
        }
    }



    SlideImage{
        id:image1
        //source: "file:///home/jakob/workspace/qt/midiPlayer-git/build-MonitorClient-Qt_5_5_0_System-Debug/Res/alpha1.png"
        opacity: 0
        fadeDur: mainWindow.fadeDur
    }

    SlideImage{
        id:image2
        //source: "file:///home/jakob/workspace/qt/midiPlayer-git/build-MonitorClient-Qt_5_5_0_System-Debug/Res/wellen-ikea.JPG"
        opacity: 0
        fadeDur: mainWindow.fadeDur
    }


    }

   MediaManager{
       id:manager
           onSearchFailed:{
               console.log("search failed");
               console.log(error);
               image1.fadeOut();
               image2.fadeOut();
               videoOutput2.fadeOut();
               behindVideoImage.fadeOut();
           }

           onVideoFound: {
               console.log("videoFound:");
               console.log(videoUrl);
               console.log(imageUrl);


               //fade over to thumbnail;
               image2.source = "file://" + imageUrl;
               behindVideoImage.source = "file://" + imageUrl;
               image2.fadeIn(function(){
                   //run video
                   behindVideoImage.opacity = 1;
                   player.stop();
                   player.source = "file://" + videoUrl;
                   videoOutput2.opacity = 1;
                   player.play();

                  image1.opacity = 0;//in case it was visible
                  image2.opacity = 0;//hide self

               });
           }

           onImageFound: {
               console.log("imageFound:");
               console.log(imageUrl);
               image2.source = "file://" + imageUrl;
               image2.fadeIn(function(){
                   player.stop();
                   //swap images
                   image1.source = image2.source;
                   image1.opacity = 1;//in case it was visible
                   image2.opacity = 0;//hide self

               });
           }
       }

   WebSocketConnector{
       id:slideWsc
       registrationParams: {
           var msg = { "deviceId" : nameParam};
           return JSON.stringify(msg);
       }
       reopen: true
       onConnectionSucceded: {
           requestType="monitor";
           display.visible = true;
           backgroundWsc.requestType="asBeamer";
       }
       onConnectionFailed: {
           display.visible = false;
       }

       url:urlParam
       onMessage: {
           console.log(JSON.stringify(msg));
           if(msg.hasOwnProperty("fadeT")){
              fadeDur = msg.fadeT * 1000;
           }
           if(msg.hasOwnProperty("ressource")){
              manager.search = msg.ressource;
           }
       }
   }

   WebSocketConnector{
       id:backgroundWsc
       registrationParams: {
           var msg = { "deviceId" : "beamer1"};
           return JSON.stringify(msg);
       }

       onMessage: {
           console.log(JSON.stringify(msg));
           if(msg.color !== undefined){
               display.color = msg.color

           }
       }
   }
}
