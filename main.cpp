#include "mainwindow.h"
#include "AppParams/appparams.h"
#include "LogginCategories/loggincategories.h"
#include "AppParams/dbsettingsdialog.h"

#include <QApplication>
#include <QFile>
#include <QDateTime>
#include <QTranslator>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


static QScopedPointer<QFile>   m_logFile;
bool openDatabaseConnection();

// Объявление обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Устанавливаем файл логирования
    m_logFile.reset(new QFile(AppParams::LOG_FILE_NAME));
    // Открываем файл логирования
    m_logFile.data()->open(QFile::Append | QFile::Text);
    // Устанавливаем обработчик
    qInstallMessageHandler(messageHandler);

    QTranslator qtTranslator;
    if (qtTranslator.load(":/Translations/qtbase_uk.qm")) {
        a.installTranslator(&qtTranslator);
    } else {
        // Обробка помилки завантаження перекладу Qt
        qDebug() << "Помилка завантаження перекладу для Qt.";
    }

    // Відкрити базу даних
    if (openDatabaseConnection()) {
        // З'єднання з базою даних вдало встановлено
        qInfo(logInfo()) << "База даних додатка успішно відкрита.";
        // // Прочитати параметри з бази даних
        // if(!readParametersFromDatabase()){
        //     qCritical(logCritical()) << QApplication::tr("Не удалось загрузить параметры приложения. Завершение работы");
        //     return 1;
        // }
    } else {
        qCritical(logCritical()) << QApplication::tr("База данных приложения не открыта. Завершение работы.");
        return 1;
    }


    MainWindow w;
    qInfo(logInfo()) << "Зауск головного вікна програми.";
    w.show();
    return a.exec();
}

// Функція для відкриття бази даних
bool openDatabaseConnection() {
    // Встановити з'єднання з базою даних, використовуючи параметри з AppParameters
    QSqlDatabase db = QSqlDatabase::addDatabase("QIBASE");
    db.setHostName(AppParams::instance().getParameter("dbHostName"));
    db.setPort(AppParams::instance().getParameter("dbDatabasePort").toInt());
    db.setDatabaseName(AppParams::instance().getParameter("dbDatabaseName"));
    db.setUserName(AppParams::instance().getParameter("dbUserName"));
    db.setPassword(AppParams::instance().getParameter("dbPassword"));

    if(db.open()){
        return true;
    } else {
        qCritical(logCritical()) << QApplication::tr("Ошибка открытия базы данных:") << db.lastError().text();
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(QApplication::tr("Ошибка подключения к базе данных!"));
        msgBox.setInformativeText(QApplication::tr("Проверить настройки подключения?"));
        msgBox.setStandardButtons(QMessageBox::Yes |  QMessageBox::No);
        msgBox.setDetailedText(db.lastError().text());
        int ret = msgBox.exec();
        if(ret == QMessageBox::Yes){
            DbSettingsDialog *dlgSetting = new DbSettingsDialog(false);
            dlgSetting->exec();
        }
        return false;
    }
}

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Открываем поток записи в файл
    QTextStream out(m_logFile.data());
    QTextStream console(stdout);
    // Записываем дату записи
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    // По типу определяем, к какому уровню относится сообщение
    switch (type)
    {
#ifdef QT_DEBUG
    case QtInfoMsg:
        out << "[INF] ";
        console << "Info:     " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtDebugMsg:
        out << "[DBG] " ;
        console << "Debug:    " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtWarningMsg:
        out << "[WRN] ";
        console << "Warning:  " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] "  << Qt::endl;
        break;
    case QtCriticalMsg:
        out << "[CRT] ";
        console << "Critical: " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
    case QtFatalMsg:
        out << "[FTL] ";
        console << "Fatality: " << msg << " ***[" << context.function << " File: " << context.file << " Line: " << context.line << "] " << Qt::endl;
        break;
#else
    case QtInfoMsg:     out << "[INF] "; break;
    case QtDebugMsg:    out << "[DBG] "; break;
    case QtWarningMsg:  out << "[WRN] "; break;
    case QtCriticalMsg: out << "[CRT] "; break;
    case QtFatalMsg:    out << "[FTL] "; break;
#endif

    }

    // Записываем в вывод категорию сообщения и само сообщение
    out << context.category << ": " << msg;
    out << "  ***[" << context.function << " File: " << context.file << " Line: " << context.line << "]***" << Qt::endl;
    // Очищаем буферизированные данные
    out.flush();
    console.flush();
}
