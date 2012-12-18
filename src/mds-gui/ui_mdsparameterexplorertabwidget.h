/********************************************************************************
** Form generated from reading UI file 'mdsparameterexplorertabwidget.ui'
**
** Created: Thu Dec 6 00:22:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDSPARAMETEREXPLORERTABWIDGET_H
#define UI_MDSPARAMETEREXPLORERTABWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MDSParameterExplorerTabWidget
{
public:
    QWidget *tab;
    QVBoxLayout *verticalLayout;
    QListWidget *completePropertiesList1;
    QScrollArea *parametersTreeScrollArea;
    QWidget *scrollAreaWidgetContents;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_2;
    QListWidget *completePropertiesList2;
    QScrollArea *parametersTreeScrollArea2;
    QWidget *scrollAreaWidgetContents_3;
    QPushButton *findCommonModelButton;
    QListView *commonModelsResultsListView;
    QWidget *tab1;
    QVBoxLayout *verticalLayout_3;
    QListWidget *completePropertiesList;
    QPushButton *addButton;
    QPushButton *minusButton;
    QListWidget *selectedPropertiesList;
    QPushButton *commonParamSearchButton;
    QScrollArea *resultsScrollArea;
    QWidget *scrollAreaWidgetContents_2;
    QWidget *tab_4;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QListWidget *completePropertiesList4;
    QLabel *label_6;
    QScrollArea *sharedParamScrollArea;
    QWidget *scrollAreaWidgetContents_5;
    QPushButton *displaySharedParamButton;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_4;
    QComboBox *numClustersComboBox;
    QLabel *label;
    QListWidget *clusterListWidget;
    QLabel *label_2;
    QListWidget *propertiesListWidget;
    QLabel *label_3;
    QScrollArea *sharedParameterScrollArea;
    QWidget *scrollAreaWidgetContents_4;
    QPushButton *displayPushButton;

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
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        verticalLayout = new QVBoxLayout(tab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        completePropertiesList1 = new QListWidget(tab);
        completePropertiesList1->setObjectName(QString::fromUtf8("completePropertiesList1"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(completePropertiesList1->sizePolicy().hasHeightForWidth());
        completePropertiesList1->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(completePropertiesList1);

        parametersTreeScrollArea = new QScrollArea(tab);
        parametersTreeScrollArea->setObjectName(QString::fromUtf8("parametersTreeScrollArea"));
        sizePolicy1.setHeightForWidth(parametersTreeScrollArea->sizePolicy().hasHeightForWidth());
        parametersTreeScrollArea->setSizePolicy(sizePolicy1);
        parametersTreeScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 347, 331));
        parametersTreeScrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(parametersTreeScrollArea);

        MDSParameterExplorerTabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_2 = new QVBoxLayout(tab_2);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        completePropertiesList2 = new QListWidget(tab_2);
        completePropertiesList2->setObjectName(QString::fromUtf8("completePropertiesList2"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(completePropertiesList2->sizePolicy().hasHeightForWidth());
        completePropertiesList2->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(completePropertiesList2);

        parametersTreeScrollArea2 = new QScrollArea(tab_2);
        parametersTreeScrollArea2->setObjectName(QString::fromUtf8("parametersTreeScrollArea2"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(parametersTreeScrollArea2->sizePolicy().hasHeightForWidth());
        parametersTreeScrollArea2->setSizePolicy(sizePolicy3);
        parametersTreeScrollArea2->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_3"));
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 347, 207));
        parametersTreeScrollArea2->setWidget(scrollAreaWidgetContents_3);

        verticalLayout_2->addWidget(parametersTreeScrollArea2);

        findCommonModelButton = new QPushButton(tab_2);
        findCommonModelButton->setObjectName(QString::fromUtf8("findCommonModelButton"));

        verticalLayout_2->addWidget(findCommonModelButton);

        commonModelsResultsListView = new QListView(tab_2);
        commonModelsResultsListView->setObjectName(QString::fromUtf8("commonModelsResultsListView"));
        sizePolicy2.setHeightForWidth(commonModelsResultsListView->sizePolicy().hasHeightForWidth());
        commonModelsResultsListView->setSizePolicy(sizePolicy2);

        verticalLayout_2->addWidget(commonModelsResultsListView);

        MDSParameterExplorerTabWidget->addTab(tab_2, QString());
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        verticalLayout_3 = new QVBoxLayout(tab1);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        completePropertiesList = new QListWidget(tab1);
        completePropertiesList->setObjectName(QString::fromUtf8("completePropertiesList"));
        sizePolicy2.setHeightForWidth(completePropertiesList->sizePolicy().hasHeightForWidth());
        completePropertiesList->setSizePolicy(sizePolicy2);

        verticalLayout_3->addWidget(completePropertiesList);

        addButton = new QPushButton(tab1);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(addButton->sizePolicy().hasHeightForWidth());
        addButton->setSizePolicy(sizePolicy4);

        verticalLayout_3->addWidget(addButton);

        minusButton = new QPushButton(tab1);
        minusButton->setObjectName(QString::fromUtf8("minusButton"));
        sizePolicy4.setHeightForWidth(minusButton->sizePolicy().hasHeightForWidth());
        minusButton->setSizePolicy(sizePolicy4);

        verticalLayout_3->addWidget(minusButton);

        selectedPropertiesList = new QListWidget(tab1);
        selectedPropertiesList->setObjectName(QString::fromUtf8("selectedPropertiesList"));
        selectedPropertiesList->setEnabled(true);
        sizePolicy2.setHeightForWidth(selectedPropertiesList->sizePolicy().hasHeightForWidth());
        selectedPropertiesList->setSizePolicy(sizePolicy2);

        verticalLayout_3->addWidget(selectedPropertiesList);

        commonParamSearchButton = new QPushButton(tab1);
        commonParamSearchButton->setObjectName(QString::fromUtf8("commonParamSearchButton"));
        sizePolicy4.setHeightForWidth(commonParamSearchButton->sizePolicy().hasHeightForWidth());
        commonParamSearchButton->setSizePolicy(sizePolicy4);

        verticalLayout_3->addWidget(commonParamSearchButton);

        resultsScrollArea = new QScrollArea(tab1);
        resultsScrollArea->setObjectName(QString::fromUtf8("resultsScrollArea"));
        sizePolicy2.setHeightForWidth(resultsScrollArea->sizePolicy().hasHeightForWidth());
        resultsScrollArea->setSizePolicy(sizePolicy2);
        resultsScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 347, 175));
        resultsScrollArea->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_3->addWidget(resultsScrollArea);

        MDSParameterExplorerTabWidget->addTab(tab1, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout = new QGridLayout(tab_4);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_5 = new QLabel(tab_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        completePropertiesList4 = new QListWidget(tab_4);
        completePropertiesList4->setObjectName(QString::fromUtf8("completePropertiesList4"));
        sizePolicy.setHeightForWidth(completePropertiesList4->sizePolicy().hasHeightForWidth());
        completePropertiesList4->setSizePolicy(sizePolicy);

        gridLayout->addWidget(completePropertiesList4, 1, 0, 1, 1);

        label_6 = new QLabel(tab_4);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        sharedParamScrollArea = new QScrollArea(tab_4);
        sharedParamScrollArea->setObjectName(QString::fromUtf8("sharedParamScrollArea"));
        sharedParamScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_5 = new QWidget();
        scrollAreaWidgetContents_5->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_5"));
        scrollAreaWidgetContents_5->setGeometry(QRect(0, 0, 347, 291));
        sharedParamScrollArea->setWidget(scrollAreaWidgetContents_5);

        gridLayout->addWidget(sharedParamScrollArea, 3, 0, 1, 1);

        displaySharedParamButton = new QPushButton(tab_4);
        displaySharedParamButton->setObjectName(QString::fromUtf8("displaySharedParamButton"));

        gridLayout->addWidget(displaySharedParamButton, 4, 0, 1, 1);

        MDSParameterExplorerTabWidget->addTab(tab_4, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        verticalLayout_4 = new QVBoxLayout(tab_3);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_4 = new QLabel(tab_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy5(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy5);

        verticalLayout_4->addWidget(label_4);

        numClustersComboBox = new QComboBox(tab_3);
        numClustersComboBox->setObjectName(QString::fromUtf8("numClustersComboBox"));
        sizePolicy4.setHeightForWidth(numClustersComboBox->sizePolicy().hasHeightForWidth());
        numClustersComboBox->setSizePolicy(sizePolicy4);

        verticalLayout_4->addWidget(numClustersComboBox);

        label = new QLabel(tab_3);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy5.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy5);

        verticalLayout_4->addWidget(label);

        clusterListWidget = new QListWidget(tab_3);
        clusterListWidget->setObjectName(QString::fromUtf8("clusterListWidget"));
        sizePolicy.setHeightForWidth(clusterListWidget->sizePolicy().hasHeightForWidth());
        clusterListWidget->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(clusterListWidget);

        label_2 = new QLabel(tab_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy5.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy5);

        verticalLayout_4->addWidget(label_2);

        propertiesListWidget = new QListWidget(tab_3);
        propertiesListWidget->setObjectName(QString::fromUtf8("propertiesListWidget"));
        sizePolicy.setHeightForWidth(propertiesListWidget->sizePolicy().hasHeightForWidth());
        propertiesListWidget->setSizePolicy(sizePolicy);

        verticalLayout_4->addWidget(propertiesListWidget);

        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy6(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy6);

        verticalLayout_4->addWidget(label_3);

        sharedParameterScrollArea = new QScrollArea(tab_3);
        sharedParameterScrollArea->setObjectName(QString::fromUtf8("sharedParameterScrollArea"));
        sizePolicy.setHeightForWidth(sharedParameterScrollArea->sizePolicy().hasHeightForWidth());
        sharedParameterScrollArea->setSizePolicy(sizePolicy);
        sharedParameterScrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents_4 = new QWidget();
        scrollAreaWidgetContents_4->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_4"));
        scrollAreaWidgetContents_4->setGeometry(QRect(0, 0, 347, 116));
        sharedParameterScrollArea->setWidget(scrollAreaWidgetContents_4);

        verticalLayout_4->addWidget(sharedParameterScrollArea);

        displayPushButton = new QPushButton(tab_3);
        displayPushButton->setObjectName(QString::fromUtf8("displayPushButton"));
        QSizePolicy sizePolicy7(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(displayPushButton->sizePolicy().hasHeightForWidth());
        displayPushButton->setSizePolicy(sizePolicy7);

        verticalLayout_4->addWidget(displayPushButton);

        MDSParameterExplorerTabWidget->addTab(tab_3, QString());

        retranslateUi(MDSParameterExplorerTabWidget);

        MDSParameterExplorerTabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(MDSParameterExplorerTabWidget);
    } // setupUi

    void retranslateUi(QTabWidget *MDSParameterExplorerTabWidget)
    {
        MDSParameterExplorerTabWidget->setWindowTitle(QApplication::translate("MDSParameterExplorerTabWidget", "TabWidget", 0, QApplication::UnicodeUTF8));
        MDSParameterExplorerTabWidget->setTabText(MDSParameterExplorerTabWidget->indexOf(tab), QApplication::translate("MDSParameterExplorerTabWidget", "Parameters", 0, QApplication::UnicodeUTF8));
        findCommonModelButton->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Search", 0, QApplication::UnicodeUTF8));
        MDSParameterExplorerTabWidget->setTabText(MDSParameterExplorerTabWidget->indexOf(tab_2), QApplication::translate("MDSParameterExplorerTabWidget", "Common Models", 0, QApplication::UnicodeUTF8));
        addButton->setText(QApplication::translate("MDSParameterExplorerTabWidget", "+", 0, QApplication::UnicodeUTF8));
        minusButton->setText(QApplication::translate("MDSParameterExplorerTabWidget", "-", 0, QApplication::UnicodeUTF8));
        commonParamSearchButton->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Search", 0, QApplication::UnicodeUTF8));
        MDSParameterExplorerTabWidget->setTabText(MDSParameterExplorerTabWidget->indexOf(tab1), QApplication::translate("MDSParameterExplorerTabWidget", "Common Params", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Properties", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Shared parameters", 0, QApplication::UnicodeUTF8));
        displaySharedParamButton->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Plot", 0, QApplication::UnicodeUTF8));
        MDSParameterExplorerTabWidget->setTabText(MDSParameterExplorerTabWidget->indexOf(tab_4), QApplication::translate("MDSParameterExplorerTabWidget", "Shared Params", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Number of Clusters", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Available Clusters", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Realizations", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Shared Parameters", 0, QApplication::UnicodeUTF8));
        displayPushButton->setText(QApplication::translate("MDSParameterExplorerTabWidget", "Display", 0, QApplication::UnicodeUTF8));
        MDSParameterExplorerTabWidget->setTabText(MDSParameterExplorerTabWidget->indexOf(tab_3), QApplication::translate("MDSParameterExplorerTabWidget", "Cluster Params", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MDSParameterExplorerTabWidget: public Ui_MDSParameterExplorerTabWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDSPARAMETEREXPLORERTABWIDGET_H
