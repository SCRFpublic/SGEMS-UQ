/********************************************************************************
** Form generated from reading UI file 'mdsspaceanalystframe.ui'
**
** Created: Tue Dec 4 18:01:25 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDSSPACEANALYSTFRAME_H
#define UI_MDSSPACEANALYSTFRAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpacerItem>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MDSSpaceAnalystFrame
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_3;
    QPushButton *loadMDSSpaceButton;
    QPushButton *saveMDSSpaceButton;
    QPushButton *deleteMDSSpaceButton;
    QPushButton *createMDSSpaceButton;
    QScrollArea *viewerWidget;
    QWidget *scrollAreaWidgetContents;
    QTreeView *mdsListTreeView;
    QTreeView *propertiesTreeExplorer;
    QLabel *label_2;
    QLabel *label_3;
    QTreeView *responsesTreeExplorer;
    QLabel *label_4;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;

    void setupUi(QFrame *MDSSpaceAnalystFrame)
    {
        if (MDSSpaceAnalystFrame->objectName().isEmpty())
            MDSSpaceAnalystFrame->setObjectName(QString::fromUtf8("MDSSpaceAnalystFrame"));
        MDSSpaceAnalystFrame->resize(1184, 772);
        MDSSpaceAnalystFrame->setFrameShape(QFrame::StyledPanel);
        MDSSpaceAnalystFrame->setFrameShadow(QFrame::Raised);
        gridLayoutWidget = new QWidget(MDSSpaceAnalystFrame);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 20, 1141, 721));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

        gridLayout->addLayout(horizontalLayout, 8, 1, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        loadMDSSpaceButton = new QPushButton(gridLayoutWidget);
        loadMDSSpaceButton->setObjectName(QString::fromUtf8("loadMDSSpaceButton"));

        gridLayout_3->addWidget(loadMDSSpaceButton, 0, 0, 1, 1);

        saveMDSSpaceButton = new QPushButton(gridLayoutWidget);
        saveMDSSpaceButton->setObjectName(QString::fromUtf8("saveMDSSpaceButton"));

        gridLayout_3->addWidget(saveMDSSpaceButton, 0, 1, 1, 1);

        deleteMDSSpaceButton = new QPushButton(gridLayoutWidget);
        deleteMDSSpaceButton->setObjectName(QString::fromUtf8("deleteMDSSpaceButton"));

        gridLayout_3->addWidget(deleteMDSSpaceButton, 1, 1, 1, 1);

        createMDSSpaceButton = new QPushButton(gridLayoutWidget);
        createMDSSpaceButton->setObjectName(QString::fromUtf8("createMDSSpaceButton"));

        gridLayout_3->addWidget(createMDSSpaceButton, 1, 0, 1, 1);


        gridLayout->addLayout(gridLayout_3, 7, 1, 1, 1);

        viewerWidget = new QScrollArea(gridLayoutWidget);
        viewerWidget->setObjectName(QString::fromUtf8("viewerWidget"));
        viewerWidget->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 823, 717));
        viewerWidget->setWidget(scrollAreaWidgetContents);

        gridLayout->addWidget(viewerWidget, 0, 2, 9, 1);

        mdsListTreeView = new QTreeView(gridLayoutWidget);
        mdsListTreeView->setObjectName(QString::fromUtf8("mdsListTreeView"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mdsListTreeView->sizePolicy().hasHeightForWidth());
        mdsListTreeView->setSizePolicy(sizePolicy);

        gridLayout->addWidget(mdsListTreeView, 2, 1, 1, 1);

        propertiesTreeExplorer = new QTreeView(gridLayoutWidget);
        propertiesTreeExplorer->setObjectName(QString::fromUtf8("propertiesTreeExplorer"));
        sizePolicy.setHeightForWidth(propertiesTreeExplorer->sizePolicy().hasHeightForWidth());
        propertiesTreeExplorer->setSizePolicy(sizePolicy);

        gridLayout->addWidget(propertiesTreeExplorer, 4, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 3, 1, 1, 1);

        responsesTreeExplorer = new QTreeView(gridLayoutWidget);
        responsesTreeExplorer->setObjectName(QString::fromUtf8("responsesTreeExplorer"));
        sizePolicy.setHeightForWidth(responsesTreeExplorer->sizePolicy().hasHeightForWidth());
        responsesTreeExplorer->setSizePolicy(sizePolicy);

        gridLayout->addWidget(responsesTreeExplorer, 6, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 5, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);


        retranslateUi(MDSSpaceAnalystFrame);

        QMetaObject::connectSlotsByName(MDSSpaceAnalystFrame);
    } // setupUi

    void retranslateUi(QFrame *MDSSpaceAnalystFrame)
    {
        MDSSpaceAnalystFrame->setWindowTitle(QApplication::translate("MDSSpaceAnalystFrame", "Frame", 0, QApplication::UnicodeUTF8));
        loadMDSSpaceButton->setText(QApplication::translate("MDSSpaceAnalystFrame", "Load", 0, QApplication::UnicodeUTF8));
        saveMDSSpaceButton->setText(QApplication::translate("MDSSpaceAnalystFrame", "Save", 0, QApplication::UnicodeUTF8));
        deleteMDSSpaceButton->setText(QApplication::translate("MDSSpaceAnalystFrame", "Delete", 0, QApplication::UnicodeUTF8));
        createMDSSpaceButton->setText(QApplication::translate("MDSSpaceAnalystFrame", "New", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MDSSpaceAnalystFrame", "Existing Metric Spaces", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MDSSpaceAnalystFrame", "Properties Explorer", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MDSSpaceAnalystFrame", "Responses Explorer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MDSSpaceAnalystFrame: public Ui_MDSSpaceAnalystFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDSSPACEANALYSTFRAME_H
