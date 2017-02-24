#ifndef OPTIONTREEWIDGET_H
#define OPTIONTREEWIDGET_H

#include <QWidget>

class OptionTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OptionTreeWidget(QWidget *parent = 0);
    ~OptionTreeWidget();
};

#endif // OPTIONTREEWIDGET_H
