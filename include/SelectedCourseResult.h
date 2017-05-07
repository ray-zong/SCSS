#ifndef SELECTEDCOURSERESULT_H
#define SELECTEDCOURSERESULT_H

#include <QDialog>

#include "DataTypeDefine.h"


class QLabel;
class QTableWidget;


class SelectedCourseResult : public QDialog
{
    Q_OBJECT

public:
    SelectedCourseResult(QWidget* parent = nullptr);
    ~SelectedCourseResult();

    void displaySelectedCourse(const QVector<DB_SpecialtyCourse> &);

private:
    void initWidget();

private:
    QLabel *m_pLabel;

    QTableWidget *m_pTableWidget;
}; 

#endif //SELECTEDCOURSERESULT_H
