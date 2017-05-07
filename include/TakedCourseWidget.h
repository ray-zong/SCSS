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

    //显示已修课程信息
    void displayTakedCourseInfo();

    //更新课程数据
    void updateCourseData(QVector<IPersonalData *> vecData);



private:
    void initWidget();

    //设置课程数据//
    void setData(const QVector<IPersonalData *> &);

private slots:

private:
    QTableWidget *m_pCourseTable;

};

#endif // TAKEDCOURSEWIDGET_H
