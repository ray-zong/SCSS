#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "OptionTreeWidget.h"
#include "TakedCourseWidget.h"
#include "SelectableCourseWidget.h"
#include "RetakeCourseWidget.h"
#include "IFileAnalysis.h"
#include "ChooseWidget.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDockWidget>
#include <QDebug>
#include <QStackedWidget>
#include <QFileDialog>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pDockWidget(nullptr)
    , m_pStackedWidget(nullptr)
    , m_pOptionTreeWidget(nullptr)
    , m_pRetakeCourseWidget(nullptr)
    , m_pSelectableCourseWidget(nullptr)
    , m_pTakedCourseWidget(nullptr)
    , m_pChooseWidget(nullptr)

{
    ui->setupUi(this);

    setWindowTitle(tr("SCSS(Student Course Selection Guidance System)"));
    //菜单栏
    createMenu();
    //左侧栏
    createDockWidget();
    //主界面
    createMainWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenu()
{
    //文件
    QMenu *pFile = menuBar()->addMenu(tr("&File"));
    {
    //打开
    QMenu *pOpen = pFile->addMenu(tr("Open"));
    //教学计划
    QAction *pTeachingPlan = pOpen->addAction(tr("Teaching Plan"));
    connect(pTeachingPlan, &QAction::triggered, this, &MainWindow::openTeachingPlan);
    //培养方案
    QAction *pTrainingProgram = pOpen->addAction(tr("Training Program"));
    connect(pTrainingProgram, &QAction::triggered, this, &MainWindow::openTrainingProgram);
    }

    //工具
    QMenu *pTool = menuBar()->addMenu(tr("&Tool"));
    {
        //选项
        QAction *pOption = pTool->addAction(tr("Option") + "...");
        connect(pOption, &QAction::triggered, this, &MainWindow::openOptionDialog);
    }
    //帮助
    QMenu *pHelp = menuBar()->addMenu(tr("&Help"));
    {
    QAction *pLanguage = pHelp->addAction("Language");
    connect(pLanguage, &QAction::triggered, this, &MainWindow::changeLanguage);
    }
}

void MainWindow::createDockWidget()
{
    //左侧栏:显示课程状态(树状列表)
    m_pDockWidget = new QDockWidget(tr("Course"), this);
    m_pDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_pDockWidget->setFloating(false);

    //隐藏标题栏
    //QWidget * qw=new QWidget(this);
    //m_pDockWidget->setTitleBarWidget(qw);

    addDockWidget(Qt::LeftDockWidgetArea, m_pDockWidget);

    m_pOptionTreeWidget = new OptionTreeWidget(this);
    connect(m_pOptionTreeWidget, &OptionTreeWidget::currentOptionChanged, this, &MainWindow::currentOptionChanged);
    m_pDockWidget->setWidget(m_pOptionTreeWidget);
}

void MainWindow::createMainWidget()
{
    QWidget *pWidget = this->centralWidget();

    QVBoxLayout *pVLayout = new QVBoxLayout(pWidget);

    m_pChooseWidget = new ChooseWidget(this);
    connect(m_pChooseWidget, &ChooseWidget::currentSpecialtyChanged, this, &MainWindow::currentSpecialtyChanged);

    //已修课程
    m_pTakedCourseWidget = new TakedCourseWidget(this);
    //未修课程
    m_pSelectableCourseWidget = new SelectableCourseWidget(this);
    //重修课程
    //m_pRetakeCourseWidget = new RetakeCourseWidget(this);

    m_pStackedWidget = new QStackedWidget(this);
    m_pStackedWidget->addWidget(m_pTakedCourseWidget);
    m_pStackedWidget->addWidget(m_pSelectableCourseWidget);
    //m_pStackedWidget->addWidget(m_pRetakeCourseWidget);

    pVLayout->addWidget(m_pChooseWidget);
    pVLayout->addWidget(m_pStackedWidget);
}

void MainWindow::openTeachingPlan()
{
    QString strFileName = QFileDialog::getOpenFileName(this,
          tr("Open Teaching Plan"), qApp->applicationDirPath(), tr("Files (*.txt)"));
    if(strFileName.isEmpty())
        return;

    IFileAnalysis *pFileAnalysis = IFileAnalysis::createFileAnalysis(IFileAnalysis::TeachingPlanFile);

    if(!pFileAnalysis->analysis(strFileName))
    {
        qDebug() << __FILE__ << __LINE__ << "open teaching plan error:" << strFileName;
        return;
    }

    if(m_pTakedCourseWidget != NULL)
    {
        QVector<IPersonalData* > vecTeachingPlan = pFileAnalysis->getData();
        m_pTakedCourseWidget->displayTeachingPlan(vecTeachingPlan);
    }
}

void MainWindow::openTrainingProgram()
{
    QString strFileName = QFileDialog::getOpenFileName(this,
          tr("Open Training Program"), qApp->applicationDirPath(), tr("Files (*.txt)"));
    if(strFileName.isEmpty())
        return;

    IFileAnalysis *pFileAnalysis = IFileAnalysis::createFileAnalysis(IFileAnalysis::TrainingProgramFile);

    if(!pFileAnalysis->analysis(strFileName))
    {
        qDebug() << __FILE__ << __LINE__ << "open training program error:" << strFileName;
        return;
    }

    if(m_pTakedCourseWidget != NULL)
    {
        QVector<IPersonalData* > vecTeachingPlan = pFileAnalysis->getData();
        m_pTakedCourseWidget->displayTrainingProgram(vecTeachingPlan);
        //TODO:暂时将培养方案中的已修课程保存在可选列表中
        m_pSelectableCourseWidget->setTakedCourseData(vecTeachingPlan);
    }
}

void MainWindow::changeLanguage()
{
    qDebug() << __FILE__ << __LINE__ << "changeLanguage function";
}

void MainWindow::currentOptionChanged(const int currentId, const int)
{
    m_pStackedWidget->setCurrentIndex(currentId);
}

void MainWindow::openOptionDialog()
{

}

void MainWindow::currentSpecialtyChanged(int index)
{
    Q_ASSERT(m_pSelectableCourseWidget != NULL);

    m_pSelectableCourseWidget->setCurrentSpecialty(index);
}

void MainWindow::currentTermChanged(int index)
{
    Q_ASSERT(m_pSelectableCourseWidget != NULL);

    m_pSelectableCourseWidget->setCurrentTerm(index);
}
