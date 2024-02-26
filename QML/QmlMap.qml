import QtQuick
import QtLocation
import QtPositioning

Rectangle {
  id: window

  property double latitude: 50.4546600
  property double longitude: 30.5238000

  Plugin {
    id: mapPlugin
    name: "osm"
    PluginParameter {
        name: "osm.mapping.providersrepository.disabled"
        value: "true"
    }
    PluginParameter {
        name: "osm.mapping.providersrepository.address"
        value: "http://maps-redirect.qt.io/osm/5.6/"
    }
  }

  Map {
    id: map
    anchors.fill: parent
    plugin: mapPlugin
    center: QtPositioning.coordinate(latitude, longitude)
    zoomLevel: 6
    property geoCoordinate startCentroid

    PinchHandler {
      id: pinch
      target: null
      onActiveChanged: if (active) {
        map.startCentroid = map.toCoordinate(pinch.centroid.position, false)
      }
      onScaleChanged: (delta) => {
        map.zoomLevel += Math.log2(delta)
        map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
      }
      onRotationChanged: (delta) => {
        map.bearing -= delta
        map.alignCoordinateToPoint(map.startCentroid, pinch.centroid.position)
      }
      grabPermissions: PointerHandler.TakeOverForbidden
    }

    WheelHandler {
      id: wheel
      acceptedDevices: Qt.platform.pluginName === "cocoa" || Qt.platform.pluginName === "wayland"
        ? PointerDevice.Mouse | PointerDevice.TouchPad
        : PointerDevice.Mouse
      rotationScale: 1/120
      property: "zoomLevel"
    }

    DragHandler {
      id: drag
      target: null
      onTranslationChanged: (delta) => map.pan(-delta.x, -delta.y)
    }

    Shortcut {
      enabled: map.zoomLevel < map.maximumZoomLevel
      sequence: StandardKey.ZoomIn
      onActivated: map.zoomLevel = Math.round(map.zoomLevel + 1)
    }

    Shortcut {
      enabled: map.zoomLevel > map.minimumZoomLevel
      sequence: StandardKey.ZoomOut
      onActivated: map.zoomLevel = Math.round(map.zoomLevel - 1)
    }

    // Додавання метки
    MapQuickItem {
      id: marker
      coordinate: QtPositioning.coordinate(latitude, longitude)
      anchorPoint.x: image.width / 2
      anchorPoint.y: image.height

      sourceItem: Item {
        width: 32
        height: 32

        Image {
          id: image
          source: "qrc:/Images/marker_UN.png" // Замініть це на вашу іконку мітки
        }
      }
    }

    // Обробка кліку на метку
    MouseArea {
      anchors.fill: marker
      onClicked: {
        console.log("Клік на метці! Координати:", marker.coordinate.latitude, marker.coordinate.longitude)
        // Додайте вашу логіку обробки кліку тут
      }
    }
    MouseArea {
        anchors.fill: parent
        onPressed: {
            var coordinate = map.toCoordinate(Qt.point(mouse.x, mouse.y));
              console.log("Широта:", coordinate.latitude.toString(), "°");
              console.log("Довгота:", coordinate.longitude.toString(), "°");
        }
    }

  }
}
