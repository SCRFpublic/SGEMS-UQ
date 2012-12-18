#include "mdsresponseitemdelegate.h"
#include <QModelIndex>

MDSResponseItemDelegate::MDSResponseItemDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}

QWidget* MDSResponseItemDelegate::createEditor(QWidget *parent,
                                               const QStyleOptionViewItem &option,
                                               const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        QCheckBox *editor = new QCheckBox(parent);
        return editor;
    }

    if (index.column() == 1)
    {
        QLineEdit *editor = new QLineEdit(parent);
        return editor;
    }

    if (index.column() == 2)
    {
        ColorListEditor *editor = new ColorListEditor(parent);
        return editor;
    }

}

void MDSResponseItemDelegate::setModelData(QWidget *editor,
                                           QAbstractItemModel *model,
                                           const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
        bool value = checkBox->isChecked();
        model->setData(index,value);
    }
    else if (index.column() == 1)
    {
        QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
        QString value = lineEdit->text();
        model->setData(index,value);
    }
    else if (index.column() == 2)
    {
        ColorListEditor *colorEdit = static_cast<ColorListEditor*>(editor);
        QString value = colorEdit->color().name();
        // model->setData(index,value);
        model->setData(index,QColor(value),Qt::BackgroundRole);
    }

}

void MDSResponseItemDelegate::setEditorData(QWidget *editor,
                                            const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        std::cout << "Setting Editor Data for Col 0" << std::endl;
        bool value = index.model()->data(index,Qt::EditRole).toBool();
        QCheckBox *box = static_cast<QCheckBox*>(editor);
        box->setChecked(value);
    }

    if (index.column() == 1)
    {
        QString value = index.model()->data(index,Qt::EditRole).toString();
        QLineEdit *line = static_cast<QLineEdit*>(editor);
        line->setText(value);
    }

    if (index.column() == 2)
    {
        QString value = index.model()->data(index,Qt::BackgroundRole).toString();
        ColorListEditor *colorEdit = static_cast<ColorListEditor*>(editor);
        //colorEdit->setColor(QColor(value));
    }
}

void MDSResponseItemDelegate::updateEditorGeometry(QWidget *editor,
                                                   const QStyleOptionViewItem &option,
                                                   const QModelIndex &index) const
{
    if (index.column() == 0)
    {
        editor->setGeometry(option.rect);
    }

    if (index.column() == 1)
    {
        editor->setGeometry(option.rect);
    }
    if (index.column() == 2)
    {
        editor->setGeometry(option.rect);
    }
}

MDSResponseItemTable::MDSResponseItemTable(QWidget *parent, int rows, int cols)
    : QTableWidget(rows,cols,parent)
{

}

void MDSResponseItemTable::fillTable(QStringList metricNames)
{
    setItemDelegate(new MDSResponseItemDelegate);

    QTableWidgetItem *item = 0;
    QStringList masterColorList = QColor::colorNames();

    for (unsigned int i = 0; i < metricNames.size(); ++i)
    {
        // Create a checkbox in the first column
        setItem(i, 0, item = new QTableWidgetItem);
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);

        // Set property name
        item = new QTableWidgetItem(metricNames.at(i));
        setItem(i, 1, item);
        item->setFlags(Qt::ItemIsEnabled);

        // Set colour
        item = new QTableWidgetItem();
        int colorIndex = (i+15)% masterColorList.size();
        QColor defaultColor = QColor(masterColorList.at(colorIndex));
        item->setData(Qt::BackgroundRole,defaultColor);
        setItem(i,2,item);
        item->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
    }

    // Set Headers
    QStringList headerLabels;
    headerLabels.append("Visible");
    headerLabels.append("Realization");
    headerLabels.append("Color");
    this->setHorizontalHeaderLabels(headerLabels);

    // Set Column Width

    // Connect Internal Slot
    connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),this,
            SLOT(tableModified(QTableWidgetItem*)));
}


void MDSResponseItemTable::tableModified(QTableWidgetItem *item)
{
    // Get the name of the property that we are altering

    QModelIndex index = this->model()->index(item->row(),1);
    QString propertyName = this->model()->data(index).toString();

    // This means the visibility has been modified
    if (item->column() == 0)
    {
        bool visible = item->checkState();
        this->visibilityModified(propertyName,visible);
    }

    // Else if color has been modified
    else if (item->column() == 2)
    {
        QColor newColor(item->data(Qt::BackgroundRole).toString());
        this->colorModified(propertyName,newColor);
    }
}
