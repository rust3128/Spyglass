#include "appparams.h"
#include "dbsettingsdialog.h"
#include "LogginCategories/loggincategories.h"

#include <QSettings>
#include <QFile>

const QString AppParams::CONFIG_FILE_NAME = "Spyglass.ini";          // Файл настроек
const QString AppParams::LOG_FILE_NAME = "Spyglass.log";             // Лог файл

AppParams &AppParams::instance()
{
    static AppParams instance;
    return instance;
}

void AppParams::setParameter(const QString &paramName, const QString &paramValue)
{
    parameters[paramName] = paramValue;
}

QString AppParams::getParameter(const QString &paramName) const
{
    QString retValue;
    if(parameters.contains(paramName)){
        retValue = parameters.value(paramName, QString());
    } else {
        qCritical(logCritical()) << "Не можливо знайти параметр по ключу!" << paramName;
        retValue.clear();
    }
    return retValue;
}

void AppParams::readDatabaseParametersFromIniFile()
{
    QSettings settings(CONFIG_FILE_NAME, QSettings::IniFormat);
    QString dbHostName = settings.value("Database/HostName").toString();
    QString dbDatabasePort = settings.value("Database/Port").toString();
    QString dbDatabaseName = settings.value("Database/DatabaseName").toString();
    QString dbUserName = settings.value("Database/UserName").toString();
    QString dbPassword = settings.value("Database/Password").toString();

    // Встановити параметри підключення до бази даних
    setParameter("dbHostName", dbHostName);
    setParameter("dbDatabasePort", dbDatabasePort);
    setParameter("dbDatabaseName", dbDatabaseName);
    setParameter("dbUserName", dbUserName);
    setParameter("dbPassword", dbPassword);
}

void AppParams::setDefaultParameters()
{

}

AppParams::AppParams() {
    setDefaultParameters(); // Ініціалізація параметрів за замовчуванням
    if(QFile(CONFIG_FILE_NAME).exists()){
        readDatabaseParametersFromIniFile(); // Зчитування параметрів підключення до бази даних
    } else {
        DbSettingsDialog *dlgSetting = new DbSettingsDialog(true);
        dlgSetting->exec();
    }
}
