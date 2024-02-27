// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlQuery>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QQmlContext *qmlContext = const_cast<QQmlContext*>(ui->quickWidgetMap->rootContext());
    qmlContext->setContextProperty("marker_model", &marker_model);

    ui->quickWidgetMap->setSource(QUrl("qrc:/QML/QmlMap.qml"));
    addMarkersObject(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addMarkersObject(int client_id)
{
    QSqlQuery q;
    q.prepare("SELECT terminal_ID, LATITUDE, LONGITUDE, NAME FROM OBJECTS WHERE CLIENT_ID = :client_D");
    q.bindValue(0, client_id);
    if (!q.exec()) {
        qCritical() << "Не можливо отримати координати АЗС";
        return;
    }

    int count = 0;
    while (q.next()) {
        QString objectID = q.value(0).toString();
        QGeoCoordinate coordinate(q.value(1).toDouble(), q.value(2).toDouble());
        marker_model.insert(count, coordinate, objectID);
        count++;
    }
}


