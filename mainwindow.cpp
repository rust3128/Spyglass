// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "LogginCategories/loggincategories.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QListWidgetItem>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QQmlContext *qmlContext = ui->quickWidgetMap->rootContext();
    qmlContext->setContextProperty("marker_model", &marker_model);
    qmlContext->setContextProperty("mainwindow", this); // Добавляем указатель на MainWindow в контекст QML
    ui->quickWidgetMap->setSource(QUrl("qrc:/QML/QmlMap.qml"));
    createUI();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleMarkerClick(const QString &objectID) {
    // ваш код обработки клика по метке
    qDebug() << "Data in MAINWINDOW. ObjectID:" << objectID;
    QSqlQuery q;
    q.prepare("select c.name,  o.terminal_id, o.name from objects o LEFT JOIN clients c ON c.client_id = o.client_id where o.object_id = :objectID");
    q.bindValue(0, objectID.toInt());
    if(!q.exec()) {
        qCritical(logCritical()) << "Не вдалося отримати інформацію про об'єкт!" << q.lastError().text();
        return;
    }
    q.next();
    QString strMess = q.value(0).toString()+"\nАЗС "+q.value(1).toString()+"\n"+q.value(2).toString();

    ui->labelIndo->setText(strMess);
}

void MainWindow::addMarkersObject(int client_id)
{
    QSqlQuery q;
    q.prepare("SELECT object_id, terminal_ID, LATITUDE, LONGITUDE, NAME FROM OBJECTS WHERE CLIENT_ID = :client_D");
    q.bindValue(0, client_id);
    if (!q.exec()) {
        qCritical() << "Не можливо отримати координати АЗС";
        return;
    }
    int count = 0;
    while (q.next()) {
        QString objectID = q.value(0).toString();
        QGeoCoordinate coordinate(q.value(2).toDouble(), q.value(3).toDouble());
        marker_model.insert(count, coordinate, objectID, client_id);

        count++;
    }
}

void MainWindow::createUI()
{
    ui->labelIndo->clear();
    // Завантажуємо клієнтів із бази даних
    QSqlQuery query("SELECT CLIENT_ID, NAME FROM CLIENTS");
    while (query.next()) {
        int clientID = query.value(0).toInt();
        QString clientName = query.value(1).toString();

        // Створюємо елемент списку для кожного клієнта
        QListWidgetItem *item = new QListWidgetItem(clientName);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);  // Додаємо прапорець
        item->setCheckState(Qt::Unchecked);  // Ставимо стан прапорця (без галочки)

        item->setData(Qt::UserRole, clientID); // Зберігаємо ID клієнта як дані користувача

        // Додаємо елемент до QListWidget
        ui->listWidgetClients->addItem(item);
    }
}


void MainWindow::on_listWidgetClients_itemChanged(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Checked) {
        // Викликати метод для відображення маркерів клієнта
        int clientID = item->data(Qt::UserRole).toInt();
        showMarkersForClient(clientID);
    } else {
        // Викликати метод для приховування маркерів клієнта
        int clientID = item->data(Qt::UserRole).toInt();
        hideMarkersForClient(clientID);
    }
}

void MainWindow::showMarkersForClient(int clientID)
{
    // Ваш код для відображення маркерів клієнта
    addMarkersObject(clientID);
}

void MainWindow::hideMarkersForClient(int clientID)
{
    // Ваш код для приховування маркерів клієнта
    marker_model.removeByClientID(clientID);
}
