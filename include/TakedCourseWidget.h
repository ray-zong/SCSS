#ifndef TAKEDCOURSEWIDGET_H
#define TAKEDCOURSEWIDGET_H

#include <QWidget>
#include <QVector>

class QTableWidget;
class IPersonalData;

class TakedCourseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TakedCourseWidget(QWidget *parent = 0);
    ~TakedCourseWidget();

    //显示教学计划
    void displayTeachingPlan(QVector<IPersonalData* >);
    //显示培训方案
    void displayTrainingProgram(const QVector<IPersonalData *> &);

private:
    void initWidget();

private slots:

private:
    QTableWidget *m_pTableWidget_teachingPlan;     //教学计划
    QTableWidget *m_pTableWidget_trainingProgram;  //培养方案
};

#endif // TAKEDCOURSEWIDGET_H
