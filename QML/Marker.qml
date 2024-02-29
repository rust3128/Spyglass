import QtQuick 2.12
import QtLocation 5.12
import QtPositioning 5.12

MapQuickItem {
    id: marker
    property string objectID: ""
    property int clientID: 0

    // Властивість для зображення маркера
    property string imageSource: {
        switch (clientID) {
            case 1: return "qrc:/Images/marker_UN.png";
            case 2: return "qrc:/Images/marker_Marshal.png";
            // і так далі...
            default: return "qrc:/Images/marker_default.png";
        }
    }

    anchorPoint.x: marker.width / 4
    anchorPoint.y: marker.height

    sourceItem: Item {
        width: 32
        height: 32

        Image {
            width: parent.width
            height: parent.height
            // Встановлення зображення маркера зовнішньої властивості
            source: marker.imageSource
        }

        // Text {
        //     anchors.bottom: parent.top
        //     anchors.horizontalCenter: parent.horizontalCenter
        //     text: marker.objectID
        // }
    }
}
