/********************************************************************************
** Form generated from reading UI file 'mdsmetricplotsubwindow.ui'
**
** Created: Tue Dec 4 18:01:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDSMETRICPLOTSUBWINDOW_H
#define UI_MDSMETRICPLOTSUBWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MDSMetricPlotSubWindow
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QScrollArea *vtkPlotArea;
    QWidget *scrollAreaWidgetContents;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QFrame *line;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QScrollArea *dataScrollArea;
    QWidget *scrollAreaWidgetContents_3;
    QFrame *line_2;
    QComboBox *comboBox;

    void setupUi(QWidget *MDSMetricPlotSubWindow)
    {
        if (MDSMetricPlotSubWindow->objectName().isEmpty())
            MDSMetricPlotSubWindow->setObjectName(QString::fromUtf8("MDSMetricPlotSubWindow"));
        MDSMetricPlotSubWindow->resize(1012, 722);
        gridLayoutWidget = new QWidget(MDSMetricPlotSubWindow);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(13, 29, 981, 431));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        vtkPlotArea = new QScrollArea(gridLayoutWidget);
        vtkPlotArea->setObjectName(QString::fromUtf8("vtkPlotArea"));
        vtkPlotArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 480, 427));
        vtkPlotArea->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(vtkPlotArea, 0, 0, 1, 1);

        scrollArea = new QScrollArea(gridLayoutWidget);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 480, 427));
        scrollArea->setWidget(scrollAreaWidgetContents_2);

        gridLayout->addWidget(scrollArea, 0, 2, 1, 1);

        line = new QFrame(gridLayoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 0, 1, 1, 1);

        tabWidget = new QTabWidget(MDSMetricPlotSubWindow);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 470, 971, 241));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        dataScrollArea = new QScrollArea(tab_2);
        dataScrollArea->setObjectName(QString::fromUtf8("dataScrollArea"));
        dataScrollArea->setGeometry(QRect(160, 10, 801, 191));
        dataScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 799, 189));
        dataScrollArea->setWidget(scrollAreaWidgetContents_3);
        line_2 = new QFrame(tab_2);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(140, 0, 20, 211));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        comboBox = new QComboBox(tab_2);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(10, 20, 131, 27));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(MDSMetricPlotSubWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MDSMetricPlotSubWindow);
    } // setupUi

    void retranslateUi(QWidget *MDSMetricPlotSubWindow)
    {
        MDSMetricPlotSubWindow->setWindowTitle(QApplication::translate("MDSMetricPlotSubWindow", "Form", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MDSMetricPlotSubWindow", "Display Properties", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MDSMetricPlotSubWindow", "Data", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MDSMetricPlotSubWindow: public Ui_MDSMetricPlotSubWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDSMETRICPLOTSUBWINDOW_H
