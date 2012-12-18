/********************************************************************************
** Form generated from reading UI file 'parameterplotsubwindow.ui'
**
** Created: Tue Dec 4 18:01:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAMETERPLOTSUBWINDOW_H
#define UI_PARAMETERPLOTSUBWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_parameterPlotSubWindow
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *parameterPlotSubWindow)
    {
        if (parameterPlotSubWindow->objectName().isEmpty())
            parameterPlotSubWindow->setObjectName(QString::fromUtf8("parameterPlotSubWindow"));
        parameterPlotSubWindow->resize(480, 320);
        verticalLayout = new QVBoxLayout(parameterPlotSubWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(parameterPlotSubWindow);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 460, 300));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(parameterPlotSubWindow);

        QMetaObject::connectSlotsByName(parameterPlotSubWindow);
    } // setupUi

    void retranslateUi(QWidget *parameterPlotSubWindow)
    {
        parameterPlotSubWindow->setWindowTitle(QApplication::translate("parameterPlotSubWindow", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class parameterPlotSubWindow: public Ui_parameterPlotSubWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAMETERPLOTSUBWINDOW_H
