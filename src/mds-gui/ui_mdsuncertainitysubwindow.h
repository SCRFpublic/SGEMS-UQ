/********************************************************************************
** Form generated from reading UI file 'mdsuncertainitysubwindow.ui'
**
** Created: Fri Apr 26 17:30:34 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDSUNCERTAINITYSUBWINDOW_H
#define UI_MDSUNCERTAINITYSUBWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpinBox>
#include <QtGui/QSplitter>
#include <QtGui/QTableWidget>
#include <QtGui/QToolBox>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MDSUncertainitySubWindow
{
public:
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QScrollArea *vtkScrollArea;
    QWidget *scrollAreaWidgetContents;
    QToolBox *toolBox;
    QWidget *page_2;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *tableWidget;
    QWidget *page_3;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *clusterArea;
    QWidget *scrollAreaWidgetContents_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *clusterLayout;
    QLabel *label;
    QSpinBox *numClusterSpinBox;
    QPushButton *generateClusters;
    QTreeView *clusterTreeView;
    QPushButton *savePushButton;
    QLineEdit *filePathLineEdit;
    QPushButton *browsePushButton;
    QWidget *paramaterPage;

    void setupUi(QWidget *MDSUncertainitySubWindow)
    {
        if (MDSUncertainitySubWindow->objectName().isEmpty())
            MDSUncertainitySubWindow->setObjectName(QString::fromUtf8("MDSUncertainitySubWindow"));
        MDSUncertainitySubWindow->resize(965, 681);
        verticalLayout = new QVBoxLayout(MDSUncertainitySubWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        splitter = new QSplitter(MDSUncertainitySubWindow);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        vtkScrollArea = new QScrollArea(splitter);
        vtkScrollArea->setObjectName(QString::fromUtf8("vtkScrollArea"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(vtkScrollArea->sizePolicy().hasHeightForWidth());
        vtkScrollArea->setSizePolicy(sizePolicy);
        vtkScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 76, 661));
        vtkScrollArea->setWidget(scrollAreaWidgetContents);
        splitter->addWidget(vtkScrollArea);
        toolBox = new QToolBox(splitter);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(toolBox->sizePolicy().hasHeightForWidth());
        toolBox->setSizePolicy(sizePolicy1);
        toolBox->setMinimumSize(QSize(0, 0));
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        page_2->setEnabled(false);
        page_2->setGeometry(QRect(0, 0, 863, 570));
        verticalLayout_3 = new QVBoxLayout(page_2);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        tableWidget = new QTableWidget(page_2);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));

        verticalLayout_3->addWidget(tableWidget);

        toolBox->addItem(page_2, QString::fromUtf8("Page 1"));
        page_3 = new QWidget();
        page_3->setObjectName(QString::fromUtf8("page_3"));
        page_3->setEnabled(false);
        page_3->setGeometry(QRect(0, 0, 863, 570));
        verticalLayout_2 = new QVBoxLayout(page_3);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        clusterArea = new QScrollArea(page_3);
        clusterArea->setObjectName(QString::fromUtf8("clusterArea"));
        clusterArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 843, 550));
        gridLayoutWidget_2 = new QWidget(scrollAreaWidgetContents_2);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(0, 10, 371, 231));
        clusterLayout = new QGridLayout(gridLayoutWidget_2);
        clusterLayout->setObjectName(QString::fromUtf8("clusterLayout"));
        clusterLayout->setSizeConstraint(QLayout::SetNoConstraint);
        clusterLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayoutWidget_2);
        label->setObjectName(QString::fromUtf8("label"));

        clusterLayout->addWidget(label, 0, 0, 1, 3);

        numClusterSpinBox = new QSpinBox(gridLayoutWidget_2);
        numClusterSpinBox->setObjectName(QString::fromUtf8("numClusterSpinBox"));
        numClusterSpinBox->setMinimum(1);
        numClusterSpinBox->setMaximum(200);
        numClusterSpinBox->setValue(5);

        clusterLayout->addWidget(numClusterSpinBox, 3, 0, 1, 2);

        generateClusters = new QPushButton(gridLayoutWidget_2);
        generateClusters->setObjectName(QString::fromUtf8("generateClusters"));

        clusterLayout->addWidget(generateClusters, 3, 2, 1, 1);

        clusterTreeView = new QTreeView(gridLayoutWidget_2);
        clusterTreeView->setObjectName(QString::fromUtf8("clusterTreeView"));
        clusterTreeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        clusterLayout->addWidget(clusterTreeView, 1, 0, 1, 3);

        savePushButton = new QPushButton(gridLayoutWidget_2);
        savePushButton->setObjectName(QString::fromUtf8("savePushButton"));

        clusterLayout->addWidget(savePushButton, 2, 2, 1, 1);

        filePathLineEdit = new QLineEdit(gridLayoutWidget_2);
        filePathLineEdit->setObjectName(QString::fromUtf8("filePathLineEdit"));

        clusterLayout->addWidget(filePathLineEdit, 2, 0, 1, 1);

        browsePushButton = new QPushButton(gridLayoutWidget_2);
        browsePushButton->setObjectName(QString::fromUtf8("browsePushButton"));

        clusterLayout->addWidget(browsePushButton, 2, 1, 1, 1);

        clusterArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_2->addWidget(clusterArea);

        toolBox->addItem(page_3, QString::fromUtf8("Page 2"));
        paramaterPage = new QWidget();
        paramaterPage->setObjectName(QString::fromUtf8("paramaterPage"));
        paramaterPage->setGeometry(QRect(0, 0, 863, 570));
        toolBox->addItem(paramaterPage, QString::fromUtf8("Parameters"));
        splitter->addWidget(toolBox);

        verticalLayout->addWidget(splitter);


        retranslateUi(MDSUncertainitySubWindow);

        toolBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MDSUncertainitySubWindow);
    } // setupUi

    void retranslateUi(QWidget *MDSUncertainitySubWindow)
    {
        MDSUncertainitySubWindow->setWindowTitle(QApplication::translate("MDSUncertainitySubWindow", "Form", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("MDSUncertainitySubWindow", "Page 1", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MDSUncertainitySubWindow", "K-Means Clustering", 0, QApplication::UnicodeUTF8));
        generateClusters->setText(QApplication::translate("MDSUncertainitySubWindow", "Generate Kmeans", 0, QApplication::UnicodeUTF8));
        savePushButton->setText(QApplication::translate("MDSUncertainitySubWindow", "Save", 0, QApplication::UnicodeUTF8));
        browsePushButton->setText(QApplication::translate("MDSUncertainitySubWindow", "Browse", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page_3), QApplication::translate("MDSUncertainitySubWindow", "Page 2", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(paramaterPage), QApplication::translate("MDSUncertainitySubWindow", "Parameters", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MDSUncertainitySubWindow: public Ui_MDSUncertainitySubWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDSUNCERTAINITYSUBWINDOW_H
