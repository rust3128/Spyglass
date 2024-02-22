#ifndef APPPARAMS_H
#define APPPARAMS_H

#include <QString>
#include <QMap>

class AppParams
{
public:
    static AppParams& instance();
    void setParameter(const QString& paramName, const QString& paramValue);
    QString getParameter(const QString& paramName) const;
    //Константи
    static const QString CONFIG_FILE_NAME;
    static const QString LOG_FILE_NAME;

    // Додати метод для зчитування параметрів підключення до бази даних з INI-файлу
    void readDatabaseParametersFromIniFile();
    // Додати метод для встановлення значень за замовчуванням
    void setDefaultParameters();
private:
    QMap<QString, QString> parameters;
    AppParams();
};

#endif // APPPARAMS_H
