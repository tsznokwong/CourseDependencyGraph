#ifndef CSVLOADER_H
#define CSVLOADER_H

#include <QString>
#include <QStringList>
#include <QList>

/**
 * @brief The CSVLoader class.
 * The class is used to loading csv data into QString Lists.
 */
class CSVLoader {
public:
    /**
     * @brief The Data struct.
     * Data in csv:
     * First line: fields.
     * Remaining lines: records
     */
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
