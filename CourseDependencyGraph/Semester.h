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
    const QString& getYear();
    const QString& getSeason();

    QString description();

};

#endif // SEMESTER_H
