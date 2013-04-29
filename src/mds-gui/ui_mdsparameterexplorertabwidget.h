/********************************************************************************
** Form generated from reading UI file 'mdsparameterexplorertabwidget.ui'
**
** Created: Fri Apr 26 17:30:34 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDSPARAMETEREXPLORERTABWIDGET_H
#define UI_MDSPARAMETEREXPLORERTABWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTabWidget>

QT_BEGIN_NAMESPACE

class Ui_MDSParameterExplorerTabWidget
{
public:

    void setupUi(QTabWidget *MDSParameterExplorerTabWidget)
    {
        if (MDSParameterExplorerTabWidget->objectName().isEmpty())
            MDSParameterExplorerTabWidget->setObjectName(QString::fromUtf8("MDSParameterExplorerTabWidget"));
        MDSParameterExplorerTabWidget->resize(400, 694);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MDSParameterExplorerTabWidget->sizePolicy().hasHeightForWidth());
        MDSParameterExplorerTabWidget->setSizePolicy(sizePolicy);
        MDSParameterExplorerTabWidget->setTabPosition(QTabWidget::West);

        retranslateUi(MDSParameterExplorerTabWidget);

        MDSParameterExplorerTabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MDSParameterExplorerTabWidget);
    } // setupUi

    void retranslateUi(QTabWidget *MDSParameterExplorerTabWidget)
    {
        MDSParameterExplorerTabWidget->setWindowTitle(QApplication::translate("MDSParameterExplorerTabWidget", "TabWidget", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MDSParameterExplorerTabWidget: public Ui_MDSParameterExplorerTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDSPARAMETEREXPLORERTABWIDGET_H
