#include <QTextStream>

#include "CSVLoader.h"

const QString CSVLoader::csvPath = ":/resources/csv/courses.csv";

CSVLoader::Data* CSVLoader::load() {
    Data *data = new Data;

    QFile csv { csvPath };
    if (!csv.exists()) { return nullptr; }
    csv.open(QIODevice::ReadOnly);

    QTextStream stream { &csv };

    // read field line
    QString fieldLine = stream.readLine();
    data->fields = fieldLine.split(",");

    // read record lines
    while (!stream.atEnd()) {
        QString recordLine = stream.readLine();
        QStringList record = recordLine.split("\",\"");
        for (QString &cell: record) {
            cell.remove("\"");
        }
        data->records.append(record);
    }

    csv.close();
    return data;
}
