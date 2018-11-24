
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
    exclusion(Relationship{Relationship::Type::EXCLUSION, exclusion}),
    prerequisite(Relationship{Relationship::Type::PREREQUISITE, prerequisite}),
    corequisite(Relationship{Relationship::Type::COREQUISITE, corequisite}),
    colist(Relationship{Relationship::Type::COLIST, colist}),
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
const Relationship& Course::getPrerequisite() const { return this->prerequisite; }
const Relationship& Course::getCorequisite() const { return this->corequisite; }
const Relationship& Course::getColist() const { return this->colist; }

const QString& Course::getVector() const { return this->vector; }
const QString& Course::getPreviousCode() const { return this->previousCode; }
const std::vector<Semester>& Course::getOfferIn() const { return this->offerIn; }
