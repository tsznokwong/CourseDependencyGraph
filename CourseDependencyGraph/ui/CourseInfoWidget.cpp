#include <QDebug>

#include "CourseInfoWidget.h"
#include "ui_CourseInfoWidget.h"

CourseInfoWidget::CourseInfoWidget(QWidget *parent,
                                   const AVLTree<QString, AVLTree<CourseCode, Course* >* >* courses,
                                   const Course* const course,
                                   const QTreeWidget* const treeWidget) :
    QWidget(parent),
    ui(new Ui::CourseInfoWidget),
    course(course),
    courses(courses),
    treeWidget(treeWidget){
    ui->setupUi(this);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

CourseInfoWidget::~CourseInfoWidget() {
    delete ui;
}

/**
 * @brief CourseInfoWidget::setCourseInfo.
 * Setting information of current selected course.
 */
void CourseInfoWidget::setCourseInfo() {
    if (this->course == nullptr) { return; }

    QString course = this->course->getCourseCode().description() + " (" + QString::number(this->course->getCredit()) + ")";
    course += "\n" + this->course->getTitle();

    QString attributeDescription = this->course->getVector() != "-" ? this->course->getVector() : "" ;
    if (attributeDescription.length() > 0) {
        if (this->course->getAttribute() != "-") {
            attributeDescription += " " + this->course->getAttribute();
        }
    } else {
        attributeDescription = this->course->getAttribute();
    }
    course += "\n" + attributeDescription;

    this->ui->courseLabel->setText(course);

    this->ui->descriptionLabel->setText(this->course->getDescription());

    this->ui->exclusionLabel->setText("Exclusion: " + this->course->getExclusion().getDescription());

    this->ui->prerequisiteLabel->setText("Prerequisites: " + this->course->getPrerequisite().getDescription());

    this->ui->corequisiteLabel->setText("Corequisites: " + this->course->getCorequisite().getDescription());

    this->ui->colistLabel->setText("Colists: " + this->course->getColist().getDescription());

    this->ui->availableAfterLabel->setText("Available After: " + this->course->getAvailableAfter().getDescription());

    this->ui->notAvailableAfterLabel->setText("Not Available After: " + this->course->getNotAvailableAfter().getDescription());

    this->ui->previousCodeLabel->setText("Previous Code: " + this->course->getPreviousCode());

    this->ui->offerInLabel->setText("Offer In: " + this->course->getOfferInDescription());
}

void CourseInfoWidget::treeWidgetItemSelected(CourseCode* courseCode) {
	AVLTree<CourseCode, Course* >* subjectTree = this->courses->find(courseCode->getSubject());
    if (subjectTree == nullptr) { return; }

	if (courseCode == nullptr) { return; }
    this->course = subjectTree->find(*courseCode);

    this->setCourseInfo();
}

