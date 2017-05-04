#include "SelectCourseDelegate.h"

#include <QCheckBox>
#include <QDebug>

SelectCourseDelegate::SelectCourseDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{

}

void SelectCourseDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter, option, index);
}

QSize SelectCourseDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option, index);
}

QWidget *SelectCourseDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QCheckBox *editor = new QCheckBox(parent);
    connect(editor, &QCheckBox::stateChanged,
            this, &SelectCourseDelegate::checkBoxStateChanged);
    return editor;
}

void SelectCourseDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    bool isChecked = qvariant_cast<bool>(index.data());
    QCheckBox *checkEditor = qobject_cast<QCheckBox *>(editor);
    checkEditor->setChecked(isChecked);
}

void SelectCourseDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
}

void SelectCourseDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
}

void SelectCourseDelegate::checkBoxStateChanged(int index)
{
    qDebug() << __FILE__ << __LINE__ << index;
}
