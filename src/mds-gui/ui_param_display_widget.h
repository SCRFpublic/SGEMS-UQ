/********************************************************************************
** Form generated from reading UI file 'param_display_widget.ui'
**
** Created: Tue Dec 4 18:01:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAM_DISPLAY_WIDGET_H
#define UI_PARAM_DISPLAY_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_param_display_widget
{
public:
    QListWidget *propertyListView;
    QScrollArea *parametersTreeScrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *param_display_widget)
    {
        if (param_display_widget->objectName().isEmpty())
            param_display_widget->setObjectName(QString::fromUtf8("param_display_widget"));
        param_display_widget->resize(375, 694);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(param_display_widget->sizePolicy().hasHeightForWidth());
        param_display_widget->setSizePolicy(sizePolicy);
        propertyListView = new QListWidget(param_display_widget);
        propertyListView->setObjectName(QString::fromUtf8("propertyListView"));
        propertyListView->setGeometry(QRect(10, 10, 349, 333));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(propertyListView->sizePolicy().hasHeightForWidth());
        propertyListView->setSizePolicy(sizePolicy1);
        parametersTreeScrollArea = new QScrollArea(param_display_widget);
        parametersTreeScrollArea->setObjectName(QString::fromUtf8("parametersTreeScrollArea"));
        parametersTreeScrollArea->setGeometry(QRect(10, 350, 349, 333));
        sizePolicy.setHeightForWidth(parametersTreeScrollArea->sizePolicy().hasHeightForWidth());
        parametersTreeScrollArea->setSizePolicy(sizePolicy);
        parametersTreeScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 347, 331));
        parametersTreeScrollArea->setWidget(scrollAreaWidgetContents);

        retranslateUi(param_display_widget);

        QMetaObject::connectSlotsByName(param_display_widget);
    } // setupUi

    void retranslateUi(QWidget *param_display_widget)
    {
        param_display_widget->setWindowTitle(QApplication::translate("param_display_widget", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class param_display_widget: public Ui_param_display_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAM_DISPLAY_WIDGET_H
