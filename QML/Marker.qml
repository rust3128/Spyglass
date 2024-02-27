import QtQuick 2.12
import QtLocation 5.12
import QtPositioning 5.12

MapQuickItem {
    id: marker
    property string objectID: ""

    anchorPoint.x: marker.width / 4
    anchorPoint.y: marker.height

    sourceItem: Item {
        width: 32
        height: 32

        Image {
            source: "qrc:/Images/marker_UN.png"
            width: parent.width
            height: parent.height
        }

        Text {
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: marker.objectID
        }
    }
}



