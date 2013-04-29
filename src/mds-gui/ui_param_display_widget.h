/********************************************************************************
** Form generated from reading UI file 'param_display_widget.ui'
**
** Created: Fri Apr 26 17:30:34 2013
**      by: Qt User Interface Compiler version 4.8.1
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
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_param_display_widget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QScrollArea *realizationsScrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QLabel *label;
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
        verticalLayout = new QVBoxLayout(param_display_widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_2 = new QLabel(param_display_widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        verticalLayout->addWidget(label_2);

        realizationsScrollArea = new QScrollArea(param_display_widget);
        realizationsScrollArea->setObjectName(QString::fromUtf8("realizationsScrollArea"));
        realizationsScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 355, 310));
        realizationsScrollArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout->addWidget(realizationsScrollArea);

        label = new QLabel(param_display_widget);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout->addWidget(label);

        parametersTreeScrollArea = new QScrollArea(param_display_widget);
        parametersTreeScrollArea->setObjectName(QString::fromUtf8("parametersTreeScrollArea"));
        sizePolicy.setHeightForWidth(parametersTreeScrollArea->sizePolicy().hasHeightForWidth());
        parametersTreeScrollArea->setSizePolicy(sizePolicy);
        parametersTreeScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 355, 310));
        parametersTreeScrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(parametersTreeScrollArea);


        retranslateUi(param_display_widget);

        QMetaObject::connectSlotsByName(param_display_widget);
    } // setupUi

    void retranslateUi(QWidget *param_display_widget)
    {
        param_display_widget->setWindowTitle(QApplication::translate("param_display_widget", "Form", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("param_display_widget", "Realizations", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("param_display_widget", "Parameters", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class param_display_widget: public Ui_param_display_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAM_DISPLAY_WIDGET_H
