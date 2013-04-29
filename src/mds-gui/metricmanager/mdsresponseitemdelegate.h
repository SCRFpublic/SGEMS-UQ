#ifndef MDSRESPONSEITEMDELEGATE_H
#define MDSRESPONSEITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <common.h>
#include <QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QSpinBox>
#include <QtGui>

#include <QtGui>

#include <iostream>
#include <QStringList>
#include "metricmanager/colorlisteditor.h"

class METRICSGUI_DECL MDSResponseItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MDSResponseItemDelegate(QObject *parent = 0);

    virtual QWidget* createEditor(QWidget *parent,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const;

    virtual void setModelData(QWidget *editor, QAbstractItemModel *model,
                              const QModelIndex &index) const;

    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;

    virtual void updateEditorGeometry(QWidget *editor,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const;

signals:

public slots:

private:

};

class MDSResponseItemTable : public QTableWidget
{
    Q_OBJECT
public:
    MDSResponseItemTable(QWidget *parent = 0, int rows = 1, int cols = 3);

    void fillTable(QStringList metricNames);

    QAbstractItemModel* returnModel()
    {
        return this->model();
    }

private slots:
    // This is a slot that reads in any changes to the table and emits another
    // signal to the VTK plotter telling it of any changes
    void tableModified(QTableWidgetItem *item);
signals:
    void visibilityModified(QString property, bool visibile);
    void colorModified(QString property, QColor newColor);
};

#endif // MDSRESPONSEITEMDELEGATE_H
