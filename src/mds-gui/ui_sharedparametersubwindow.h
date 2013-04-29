/********************************************************************************
** Form generated from reading UI file 'sharedparametersubwindow.ui'
**
** Created: Sun Feb 10 01:05:42 2013
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREDPARAMETERSUBWINDOW_H
#define UI_SHAREDPARAMETERSUBWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sharedParameterSubwindow
{
public:
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *sharedParameterSubwindow)
    {
        if (sharedParameterSubwindow->objectName().isEmpty())
            sharedParameterSubwindow->setObjectName(QString::fromUtf8("sharedParameterSubwindow"));
        sharedParameterSubwindow->resize(791, 681);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sharedParameterSubwindow->sizePolicy().hasHeightForWidth());
        sharedParameterSubwindow->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(sharedParameterSubwindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(sharedParameterSubwindow);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 771, 661));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        retranslateUi(sharedParameterSubwindow);

        QMetaObject::connectSlotsByName(sharedParameterSubwindow);
    } // setupUi

    void retranslateUi(QWidget *sharedParameterSubwindow)
    {
        sharedParameterSubwindow->setWindowTitle(QApplication::translate("sharedParameterSubwindow", "Parameter Frequency", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class sharedParameterSubwindow: public Ui_sharedParameterSubwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREDPARAMETERSUBWINDOW_H
