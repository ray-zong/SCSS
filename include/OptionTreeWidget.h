#ifndef OPTIONTREEWIDGET_H
#define OPTIONTREEWIDGET_H

#include <QWidget>

class QTreeWidget;
class QTreeWidgetItem;

class OptionTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionTreeWidget(QWidget *parent = 0);
    ~OptionTreeWidget();

private:
    //列表
    void createTreeWidget();

private slots:
    void currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*);

signals:
    void currentOptionChanged(const int, const int);

private:
    QTreeWidget *m_pTreeWidget;
};

#endif // OPTIONTREEWIDGET_H
