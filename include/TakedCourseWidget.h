#ifndef TAKEDCOURSEWIDGET_H
#define TAKEDCOURSEWIDGET_H

#include <QWidget>
#include <QVector>

class QTableWidget;
class QPushButton;

class IPersonalData;
class AddCourseWidget;
class ModifyCourseWidget;

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

    //从数据库中通过课程索引删除课程
    bool deleteCourseByNumber(int number);

private slots:
    //删除按钮
    void deleteCourse();

    //修改课程
    void modifyCourse();

    //添加课程
    void addCourse();

signals:
    //修改已修课程
    void takedCourseChanged();

private:
    QTableWidget *m_pCourseTable;
    QPushButton *m_pPushButton_Add;
    QPushButton *m_pPushButton_Delete;
    QPushButton *m_pPushButton_Modify;

    AddCourseWidget *m_pAddCourseWidget;
    ModifyCourseWidget *m_pModifyCourseWidget;
};

#endif // TAKEDCOURSEWIDGET_H
