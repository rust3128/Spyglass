#ifndef MARKERMODEL_H
#define MARKERMODEL_H

#include <QAbstractListModel>
#include <QGeoCoordinate>

class MarkerModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QGeoCoordinate current READ current NOTIFY currentChanged)
public:
    enum MarkerRoles{
        PositionRole = Qt::UserRole + 1000,
        ObjectIDRole,
        ClientIDRole
    };


    explicit MarkerModel(QObject *parent = nullptr);

    void moveMarker(const QGeoCoordinate &coordinate, int clientID);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    int maxMarkers() const;
    void setMaxMarkers(int maxMarkers=0);
    QGeoCoordinate current() const;
    void insert(int row, const QGeoCoordinate &coordinate, const QString &objectID, int clientID);
    void removeLastMarker();
    void removeByClientID(int clientID);

signals:
    void currentChanged();

private:
    QList<QGeoCoordinate> m_markers;
    QList<QString> m_objectIDs;
    QList<int> m_clientIDs;
    QGeoCoordinate m_current;
    int m_maxMarkers;
};

#endif // MARKERMODEL_H
