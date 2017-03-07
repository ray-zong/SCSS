#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QDockWidget;
class QStackedWidget;
class OptionTreeWidget;
class RetakeCourseWidget;
class SelectableCourseWidget;
class TakedCourseWidget;
class ChooseWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //菜单栏
    void createMenu();
    //左侧栏：显示课程状态
    void createDockWidget();
    //主界面：显示课程信息
    void createMainWidget();

private slots:
    //打开教学计划
    void openTeachingPlan();
    //打开培养方案
    void openTrainingProgram();
    //语言切换
    void changeLanguage();
    //显示状态改变
    void currentOptionChanged(const int, const int);
    //打开工具栏
    void openOptionDialog();
    //改变专业
    void currentSpecialtyChanged(int index);
    //改变学期
    void currentTermChanged(int index);

private:
    Ui::MainWindow *ui;

    QDockWidget *m_pDockWidget;
    QStackedWidget *m_pStackedWidget;
    OptionTreeWidget *m_pOptionTreeWidget;
    RetakeCourseWidget *m_pRetakeCourseWidget;
    SelectableCourseWidget *m_pSelectableCourseWidget;
    TakedCourseWidget *m_pTakedCourseWidget;
    ChooseWidget *m_pChooseWidget;
};

#endif // MAINWINDOW_H
