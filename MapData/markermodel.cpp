#include "markermodel.h"

MarkerModel::MarkerModel(QObject *parent)
    : QAbstractListModel(parent),
    m_maxMarkers(0)
{
}

void MarkerModel::moveMarker(const QGeoCoordinate &coordinate)
{
    QGeoCoordinate last = m_current;
    m_current = coordinate;
    emit currentChanged();

    if (!last.isValid())
        return;

    if (m_maxMarkers == 0) {
        insert(0, last, "CentralMarker");
        return;
    }

    if (rowCount() >= m_maxMarkers) {
        while (rowCount() >= m_maxMarkers)
            removeLastMarker();
        removeLastMarker();
    }

    insert(0, last, "NewMarker");
}

int MarkerModel::maxMarkers() const
{
    return m_maxMarkers;
}

void MarkerModel::setMaxMarkers(int maxMarkers)
{
    m_maxMarkers = maxMarkers > 1 ? maxMarkers : 0;
}

QGeoCoordinate MarkerModel::current() const
{
    return m_current;
}

int MarkerModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_markers.count();
}

QVariant MarkerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == PositionRole)
        return QVariant::fromValue(m_markers[index.row()]);
    else if (role == ObjectIDRole)
        return m_objectIDs[index.row()];

    return QVariant();
}

QHash<int, QByteArray> MarkerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PositionRole] = "position";
    roles[ObjectIDRole] = "objectIDRole";  // Використовуйте правильну назву ролі тут
    return roles;
}



void MarkerModel::insert(int row, const QGeoCoordinate &coordinate, const QString &objectID)
{
    beginInsertRows(QModelIndex(), row, row);
    m_markers.insert(row, coordinate);
    m_objectIDs.insert(row, objectID);
    endInsertRows();

    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(rowCount() - 1, 0);
    emit dataChanged(topLeft, bottomRight, {PositionRole, ObjectIDRole});
}

void MarkerModel::removeLastMarker()
{
    if (m_markers.isEmpty() || m_objectIDs.isEmpty())
        return;

    beginRemoveRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
    m_markers.removeLast();
    m_objectIDs.removeLast();
    endRemoveRows();

    QModelIndex topLeft = createIndex(0, 0);
    QModelIndex bottomRight = createIndex(rowCount() - 1, 0);
    emit dataChanged(topLeft, bottomRight, {PositionRole, ObjectIDRole});
}
