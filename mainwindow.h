// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQmlContext>
#include <QGeoCoordinate>
#include "MapData/markermodel.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void handleMarkerClick(const QString &objectID);

private:
    void addMarkersObject(int client_id);



private:
    Ui::MainWindow *ui;
    MarkerModel marker_model;
};

#endif // MAINWINDOW_H
