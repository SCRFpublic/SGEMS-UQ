/********************************************************************************
** Form generated from reading UI file 'MeanData.ui'
**
** Created: Mon Apr 29 12:05:02 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEANDATA_H
#define UI_MEANDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "GsTLAppli/extra/qtplugins/selectors.h"

QT_BEGIN_NAMESPACE

class Ui_MeanData
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *Name;
    GridSelector *GridName;
    OrderedPropertySelector *PropertyNames;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *MeanData)
    {
        if (MeanData->objectName().isEmpty())
            MeanData->setObjectName(QString::fromUtf8("MeanData"));
        MeanData->resize(229, 484);
        verticalLayout_2 = new QVBoxLayout(MeanData);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(MeanData);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        Name = new QLineEdit(MeanData);
        Name->setObjectName(QString::fromUtf8("Name"));

        verticalLayout->addWidget(Name);


        verticalLayout_2->addLayout(verticalLayout);

        GridName = new GridSelector(MeanData);
        GridName->setObjectName(QString::fromUtf8("GridName"));

        verticalLayout_2->addWidget(GridName);

        PropertyNames = new OrderedPropertySelector(MeanData);
        PropertyNames->setObjectName(QString::fromUtf8("PropertyNames"));

        verticalLayout_2->addWidget(PropertyNames);

        verticalSpacer = new QSpacerItem(20, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(MeanData);
        QObject::connect(GridName, SIGNAL(activated(QString)), PropertyNames, SLOT(show_properties(QString)));

        QMetaObject::connectSlotsByName(MeanData);
    } // setupUi

    void retranslateUi(QWidget *MeanData)
    {
        MeanData->setWindowTitle(QApplication::translate("MeanData", "Compute Average", 0, QApplication::UnicodeUTF8));
        MeanData->setProperty("AlgorithmCategory", QVariant(QApplication::translate("MeanData", "Data", 0, QApplication::UnicodeUTF8)));
        MeanData->setProperty("AlgorithmDescription", QVariant(QApplication::translate("MeanData", "Compute the arithmetic average", 0, QApplication::UnicodeUTF8)));
        label->setText(QApplication::translate("MeanData", "Name of Data", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MeanData: public Ui_MeanData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEANDATA_H
