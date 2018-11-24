#ifndef CSVLOADER_H
#define CSVLOADER_H

#include <QString>
#include <QStringList>
#include <QList>

class CSVLoader {
public:
    struct Data {
        QStringList fields;
        QList<QStringList> records;
    };
private:
    static const QString csvPath;
public:
    CSVLoader() = delete;
    static Data* load();
};

#endif // CSVLOADER_H
