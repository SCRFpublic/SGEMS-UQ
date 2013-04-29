#ifndef MDSPARAMETERTABLEMODEL_H
#define MDSPARAMETERTABLEMODEL_H

#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QDebug>
#include <common.h>

class METRICSGUI_DECL parameterNode: public QTreeWidgetItem
{
public:
    parameterNode(QString nodeName, QString nodeValue,
                  QTreeWidgetItem *parentItem = 0):
                  QTreeWidgetItem(parentItem,QTreeWidgetItem::UserType)
    {
        this->name_ = nodeName;
        this->value_ = nodeValue;
        this->setData(0,Qt::DisplayRole,this->name_);
        this->setData(1,Qt::DisplayRole,this->value_);
    }

    QHash<QString, parameterNode*> children;
    parameterNode* parent;

    QString name(){return name_;}
    QString value(){return value_;}

private:
    QString name_;
    QString value_;
};

class METRICSGUI_DECL parameterModel : public QTreeWidget
{
    Q_OBJECT

public:
    parameterModel(QWidget *parent = 0);
    ~parameterModel();
    void addNode(QString pathName, QString value, parameterNode *parentNode);
    parameterNode *root(){return root_;}

private:
    parameterNode *root_;
    void freeNodeMemory(parameterNode *node);
};
#endif // MDSPARAMETERTABLEMODEL_H
