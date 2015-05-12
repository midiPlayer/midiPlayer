import QtQuick 2.0
import QtQuick.Layouts 1.1
Rectangle{
    property alias name : text.text
    property alias imageSrc : img.source
    property string link;

    Layout.alignment:Qt.AlignCenter
    Layout.minimumWidth: 100
    Layout.preferredWidth: 200
    height: 200
    color:"#00000000"
      ColumnLayout{
          anchors.fill: parent
          spacing: 20
         Image{
             id: img
             Layout.fillHeight: true
             anchors.horizontalCenter: parent.horizontalCenter
             fillMode: Image.PreserveAspectFit
         }
         Text{
             id: text
            color: "#369cb6"
            font.pointSize: 17
            anchors.horizontalCenter: parent.horizontalCenter
         }
      }

    MouseArea{
        anchors.fill: parent
        onPressed:{
            stackView.push(Qt.resolvedUrl(link),{});
        }
    }
}
