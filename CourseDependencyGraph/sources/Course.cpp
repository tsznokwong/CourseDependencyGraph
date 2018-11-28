
#include "Course.h"

Course::Course(CourseCode courseCode,
               QString title,
               int credit,
               QString description,
               QString attribute,
               QString exclusion,
               QString prerequisite,
               QString corequisite,
               QString colist,
               QString vector,
               QString previousCode,
               std::vector<Semester> offerIn):
    courseCode(courseCode),
    title(title),
    credit(credit),
    description(description),
    attribute(attribute),
    exclusion(Relationship{ Relationship::Type::EXCLUSION, exclusion }),
    prerequisite(Relationship{ Relationship::Type::PREREQUISITE, prerequisite }),
    corequisite(Relationship{ Relationship::Type::COREQUISITE, corequisite }),
    colist(Relationship{ Relationship::Type::COLIST, colist }),
    notAvailableAfter(Relationship{ Relationship::Type::NOT_AVAILABLE_AFTER, "" }),
    availableAfter(Relationship{ Relationship::Type::AVAILABLE_AFTER, "" }),
    vector(vector),
    previousCode(previousCode),
    offerIn(offerIn) {

}

const CourseCode& Course::getCourseCode() const { return this->courseCode; }
const QString& Course::getTitle() const { return this->title; }
const int& Course::getCredit() const { return this->credit; }
const QString& Course::getDescription() const { return this->description; }
const QString& Course::getAttribute() const { return this->attribute; }

const Relationship& Course::getExclusion() const { return this->exclusion; }
Relationship& Course::getExclusion() { return this->exclusion; }
const Relationship& Course::getPrerequisite() const { return this->prerequisite; }
Relationship& Course::getPrerequisite() { return this->prerequisite; }
const Relationship& Course::getCorequisite() const { return this->corequisite; }
const Relationship& Course::getColist() const { return this->colist; }
const Relationship& Course::getNotAvailableAfter() const { return this->notAvailableAfter; }
const Relationship& Course::getAvaialbeAfter() const { return this->availableAfter; }

const QString& Course::getVector() const { return this->vector; }
const QString& Course::getPreviousCode() const { return this->previousCode; }
const std::vector<Semester>& Course::getOfferIn() const { return this->offerIn; }

QString Course::getOfferInDescription() const {
    QString description = "";
    for (unsigned int iterator = 0; iterator < this->offerIn.size(); ++iterator) {
        description += this->offerIn[iterator].description() +
                (iterator < this->offerIn.size() - 1 ? ", " : "");
    }
    return description;
}

std::ostream& operator<<(std::ostream &os, const Course& course) {
    return os << course.getCourseCode().description().toStdString();
}

void Course::addNotAvailableAfter(Course* const course) {
    this->notAvailableAfter.addEdge(course->courseCode, course);
}
void Course::addAvaiableAfter(Course* const course) {
    this->availableAfter.addEdge(course->courseCode, course);
}

void Course::linkCourses(const AVLTree<QString, AVLTree<CourseCode, Course* >* > &courses) {
    this->exclusion.linkCourses(courses);
    this->prerequisite.linkCourses(courses);
    this->corequisite.linkCourses(courses);
    this->colist.linkCourses(courses);
}

QString Course::dubugDescription() {
    QString description = this->courseCode.description() + " "
            + this->title + " "
            + "(" + QString::number(this->credit) + ") "
            + "\n" + this->description
            + "\n" + this->attribute
            + "\nExclusion: " + this->exclusion.getDescription()
            + "\nPrerequisite: " + this->prerequisite.getDescription()
            + "\nCorequisite: " + this->corequisite.getDescription()
            + "\nColist: " + this->colist.getDescription()
            + "\nNot Available After: " + this->notAvailableAfter.getDescription()
            + "\nAvailable After: " + this->availableAfter.getDescription()
            + "\nVector: " + this->vector
            + "\nPrevious Code: " + this->previousCode
            + "\nOffer In: " + this->getOfferInDescription();
    return description;
}
