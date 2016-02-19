import QtQuick 2.0


Item{
    id:delegate

    property bool dragging : false;

    property ListView listView

    property Item scene;

    property alias content: dragRect.children;

    function startDrag(){
        dragRect.dragStart = delegate.y;
    }

    property alias target: dragRect

    signal orderChanged();


Item {

    id: dragRect

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter

    width:delegate.width;
    height: delegate.height;

    property int dragStart: 0
    property int jumpedItems: 0


    onYChanged: {
        if(!dragRect.Drag.active)
            return;

        var nextHeight = 100000;
        if(listView.itemAt(dragRect.width/2,y+height+listView.contentY) !== null)
            nextHeight = listView.itemAt(dragRect.width/2,y+height+listView.contentY).height;

        var prevHeight = 100000;
        if(listView.itemAt(dragRect.width/2,y+listView.contentY) !== null)
            prevHeight = listView.itemAt(dragRect.width/2,y+listView.contentY).height;

       if(dragRect.y + listView.contentY - dragStart > nextHeight * 0.75
               && index < listView.count-1
               && (listView.itemAt(dragRect.width/2,y+height+listView.contentY) !== delegate)){

            dragStart = dragStart + nextHeight;
            listView.model.move(index,index+1,1);
            orderChanged();
           jumpedItems++;
       }
       if(dragRect.y + listView.contentY - dragStart < -prevHeight * 0.75
               && index != 0
               && (listView.itemAt(dragRect.width/2,y+height+listView.contentY) !== delegate)){
            listView.model.move(index,index-1,1);
           orderChanged();
           dragStart = dragStart - prevHeight;
           if(index == 0){
               mouseArea.drag.minimumY = 0;
           }
       }
     }


    states: [
        State {
            when: dragging
            ParentChange {
                target: dragRect
                parent: scene
            }

            AnchorChanges {
                target: dragRect
                anchors.horizontalCenter: undefined
                anchors.verticalCenter: undefined
            }
        }
    ]

    Drag.active: dragging;
    Drag.hotSpot.x: dragRect.width / 2
    Drag.hotSpot.y: dragRect.height / 2
}
}
