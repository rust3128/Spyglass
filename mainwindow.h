// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQmlContext>
#include <QGeoCoordinate>
#include "MapData/markermodel.h"
#include <QListWidgetItem>


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

private slots:
    void on_listWidgetClients_itemChanged(QListWidgetItem *item);

private:
    void addMarkersObject(int client_id);
    void createUI();
    void showMarkersForClient(int clientID);
    void hideMarkersForClient(int clientID);
private:
    Ui::MainWindow *ui;
    MarkerModel marker_model;
};

#endif // MAINWINDOW_H
