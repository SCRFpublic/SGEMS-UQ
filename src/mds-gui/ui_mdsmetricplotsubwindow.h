/********************************************************************************
** Form generated from reading UI file 'mdsmetricplotsubwindow.ui'
**
** Created: Fri Apr 26 17:30:34 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDSMETRICPLOTSUBWINDOW_H
#define UI_MDSMETRICPLOTSUBWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MDSMetricPlotSubWindow
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QFrame *line;
    QScrollArea *vtkPlotArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *MDSMetricPlotSubWindow)
    {
        if (MDSMetricPlotSubWindow->objectName().isEmpty())
            MDSMetricPlotSubWindow->setObjectName(QString::fromUtf8("MDSMetricPlotSubWindow"));
        MDSMetricPlotSubWindow->resize(1012, 469);
        gridLayout_2 = new QGridLayout(MDSMetricPlotSubWindow);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        scrollArea = new QScrollArea(MDSMetricPlotSubWindow);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 486, 447));
        scrollArea->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(scrollArea, 0, 2, 1, 1);

        line = new QFrame(MDSMetricPlotSubWindow);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 0, 1, 1, 1);

        vtkPlotArea = new QScrollArea(MDSMetricPlotSubWindow);
        vtkPlotArea->setObjectName(QString::fromUtf8("vtkPlotArea"));
        vtkPlotArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 487, 447));
        vtkPlotArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(vtkPlotArea, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(MDSMetricPlotSubWindow);

        QMetaObject::connectSlotsByName(MDSMetricPlotSubWindow);
    } // setupUi

    void retranslateUi(QWidget *MDSMetricPlotSubWindow)
    {
        MDSMetricPlotSubWindow->setWindowTitle(QApplication::translate("MDSMetricPlotSubWindow", "Form", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MDSMetricPlotSubWindow: public Ui_MDSMetricPlotSubWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDSMETRICPLOTSUBWINDOW_H
