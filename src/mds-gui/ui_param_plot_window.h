/********************************************************************************
** Form generated from reading UI file 'param_plot_window.ui'
**
** Created: Fri Apr 26 17:30:34 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAM_PLOT_WINDOW_H
#define UI_PARAM_PLOT_WINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_param_plot_window
{
public:
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *param_plot_window)
    {
        if (param_plot_window->objectName().isEmpty())
            param_plot_window->setObjectName(QString::fromUtf8("param_plot_window"));
        param_plot_window->resize(579, 337);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(param_plot_window->sizePolicy().hasHeightForWidth());
        param_plot_window->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(param_plot_window);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea = new QScrollArea(param_plot_window);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 559, 317));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(scrollArea, 0, 0, 1, 1);


        retranslateUi(param_plot_window);

        QMetaObject::connectSlotsByName(param_plot_window);
    } // setupUi

    void retranslateUi(QWidget *param_plot_window)
    {
        param_plot_window->setWindowTitle(QApplication::translate("param_plot_window", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class param_plot_window: public Ui_param_plot_window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAM_PLOT_WINDOW_H
