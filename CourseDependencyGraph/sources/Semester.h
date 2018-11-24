#ifndef SEMESTER_H
#define SEMESTER_H

#include <QString>

class Semester {
private:
    QString year;
    QString season;

public:
    Semester(QString year, QString season);

    // getter
    const QString& getYear() const;
    const QString& getSeason() const;

    QString description() const;

};

#endif // SEMESTER_H
