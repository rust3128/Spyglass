import QtQuick
import QtLocation
import QtPositioning

Rectangle {
    id: mainWindow

    property double latitude: 50.39700098315903
    property double longitude: 30.49388610412916

    Plugin {
        id: mapPlugin
        name: "osm"
        PluginParameter {
            name: "osm.mapping.providersrepository.disabled"
            value: "true"
        }
        PluginParameter {
            name: "osm.mapping.providersrepository.address"
            value: "http://maps-redirect.qt.io/osm/5.8/"
        }
    }

    Map {
        id: map
        anchors.fill: parent
        plugin: mapPlugin
        center: QtPositioning.coordinate(latitude, longitude)
        zoomLevel: 10
        property geoCoordinate startCentroid

        MapItemView {
            id: mapItemView
            model: marker_model
            delegate: Marker {
                coordinate: model.position
                objectID: model.objectIDRole
                clientID: model.clientIDRole // Додаємо передачу clientID в Marker

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("Marker clicked. ObjectID:", model.objectIDRole)
                        console.log("mainwindow:", mainwindow)
                        mainwindow.handleMarkerClick(model.objectIDRole);
                    }
                }
            }
        }




        PinchHandler {
            id: pinch
            target: map
            onActiveChanged: if (active) {
                                 map.startCentroid = map.toCoordinate(
                                             pinch.centroid.position, false)
                             }
            onScaleChanged: delta => {
                                map.zoomLevel += Math.log2(delta)
                                map.alignCoordinateToPoint(
                                    map.startCentroid, pinch.centroid.position)
                            }
            onRotationChanged: delta => {
                                   map.bearing -= delta
                                   map.alignCoordinateToPoint(
                                       map.startCentroid,
                                       pinch.centroid.position)
                               }
            grabPermissions: PointerHandler.TakeOverForbidden
        }

        WheelHandler {
            id: wheel
            acceptedDevices: Qt.platform.pluginName === "cocoa"
                             || Qt.platform.pluginName
                             === "wayland" ? PointerDevice.Mouse
                                             | PointerDevice.TouchPad : PointerDevice.Mouse
            rotationScale: 1 / 120
            property: "zoomLevel"
        }

        DragHandler {
            id: drag
            target: map
            onTranslationChanged: delta => map.pan(-delta.x, -delta.y)
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
              id: itbiMarker
              coordinate: QtPositioning.coordinate(latitude, longitude)
              anchorPoint.x: image.width / 2
              anchorPoint.y: image.height

              sourceItem: Item {
                width: 16
                height: 16

                Image {
                  id: image
                  source: "qrc:/Images/itb_logo.png" // Замініть це на вашу іконку мітки
                }
              }
            }
    }
}
