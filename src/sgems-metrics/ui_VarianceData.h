/********************************************************************************
** Form generated from reading UI file 'VarianceData.ui'
**
** Created: Mon Apr 29 12:05:02 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VARIANCEDATA_H
#define UI_VARIANCEDATA_H

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

class Ui_VarianceData
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *Name;
    GridSelector *GridName;
    OrderedPropertySelector *PropertyNames;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *VarianceData)
    {
        if (VarianceData->objectName().isEmpty())
            VarianceData->setObjectName(QString::fromUtf8("VarianceData"));
        VarianceData->resize(229, 484);
        verticalLayout_2 = new QVBoxLayout(VarianceData);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label = new QLabel(VarianceData);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        Name = new QLineEdit(VarianceData);
        Name->setObjectName(QString::fromUtf8("Name"));

        verticalLayout->addWidget(Name);


        verticalLayout_2->addLayout(verticalLayout);

        GridName = new GridSelector(VarianceData);
        GridName->setObjectName(QString::fromUtf8("GridName"));

        verticalLayout_2->addWidget(GridName);

        PropertyNames = new OrderedPropertySelector(VarianceData);
        PropertyNames->setObjectName(QString::fromUtf8("PropertyNames"));

        verticalLayout_2->addWidget(PropertyNames);

        verticalSpacer = new QSpacerItem(20, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        retranslateUi(VarianceData);
        QObject::connect(GridName, SIGNAL(activated(QString)), PropertyNames, SLOT(show_properties(QString)));

        QMetaObject::connectSlotsByName(VarianceData);
    } // setupUi

    void retranslateUi(QWidget *VarianceData)
    {
        VarianceData->setWindowTitle(QApplication::translate("VarianceData", "Compute Variance", 0, QApplication::UnicodeUTF8));
        VarianceData->setProperty("AlgorithmCategory", QVariant(QApplication::translate("VarianceData", "Data", 0, QApplication::UnicodeUTF8)));
        VarianceData->setProperty("AlgorithmDescription", QVariant(QApplication::translate("VarianceData", "Compute the variance", 0, QApplication::UnicodeUTF8)));
        label->setText(QApplication::translate("VarianceData", "Name of Data", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VarianceData: public Ui_VarianceData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VARIANCEDATA_H
