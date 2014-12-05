import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.1
import components 1.0

Rectangle {
    id: root
    width: 500
    height: 500
    anchors.fill: parent
    visible: true
    color: 'green'
    //    opacity: 0.7
    focus: true
    //    anchors.margins: 100

    Rectangle{
        anchors.fill: parent
        anchors.margins: 50
        color: 'red'
        focus: true

        QmlWebViewWidget{
            id:w
            anchors.fill: parent
            anchors.margins: 100
                        rotation: 45
            focus: true
            url: "http://173.201.253.186/Testing/ar/view-all-books"
//            onUrlChanged: yRotation.angle += 180
//            transform: Rotation {
//                id: yRotation;
//                origin.x: w.width/2; origin.y: w.height/2;
//                axis { x: 0; y: 1; z: 0 }
//                angle: 0
//                Behavior on angle{NumberAnimation{}}
//            }
        }
    }
}
