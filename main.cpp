#include <QQuickView>
#include <QtQml>
#include <QApplication>
#include "QmlWebViewWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


//    QQuickView* view(new QQuickView);
    qmlRegisterType<QmlWebViewWidget>("components", 1, 0, "QmlWebViewWidget" );


    QQmlApplicationEngine engine;

    engine.load(QUrl("qrc:/qml/main.qml"));

//    QSurfaceFormat surfaceFormat;
//    surfaceFormat.setAlphaBufferSize(8);
//    view->setFormat(surfaceFormat);
//    view->setClearBeforeRendering(true);
//    view->setColor(QColor(Qt::transparent));
//    view->setSource(QUrl("qrc:/qml/main.qml"));
//    view->showMaximized();

    return app.exec();
 }

