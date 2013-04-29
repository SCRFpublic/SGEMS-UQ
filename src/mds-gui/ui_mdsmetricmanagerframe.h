/********************************************************************************
** Form generated from reading UI file 'mdsmetricmanagerframe.ui'
**
** Created: Fri Apr 26 17:30:34 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDSMETRICMANAGERFRAME_H
#define UI_MDSMETRICMANAGERFRAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QTreeView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MDSMetricManagerFrame
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QTreeView *responseListTreeView;
    QFrame *line;
    QTreeView *propertyListTreeView;
    QFormLayout *formLayout_2;
    QPushButton *loadResponseButton;
    QPushButton *deleteResponseButton;
    QLabel *label;
    QLabel *label_2;
    QTreeView *gridListTreeView;
    QLabel *label_3;
    QFrame *line_2;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QFrame *MDSMetricManagerFrame)
    {
        if (MDSMetricManagerFrame->objectName().isEmpty())
            MDSMetricManagerFrame->setObjectName(QString::fromUtf8("MDSMetricManagerFrame"));
        MDSMetricManagerFrame->resize(1829, 856);
        MDSMetricManagerFrame->setFrameShape(QFrame::StyledPanel);
        MDSMetricManagerFrame->setFrameShadow(QFrame::Raised);
        gridLayoutWidget = new QWidget(MDSMetricManagerFrame);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(20, 20, 311, 831));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        responseListTreeView = new QTreeView(gridLayoutWidget);
        responseListTreeView->setObjectName(QString::fromUtf8("responseListTreeView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(responseListTreeView->sizePolicy().hasHeightForWidth());
        responseListTreeView->setSizePolicy(sizePolicy);

        gridLayout->addWidget(responseListTreeView, 3, 0, 1, 1);

        line = new QFrame(gridLayoutWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 7, 0, 1, 1);

        propertyListTreeView = new QTreeView(gridLayoutWidget);
        propertyListTreeView->setObjectName(QString::fromUtf8("propertyListTreeView"));
        sizePolicy.setHeightForWidth(propertyListTreeView->sizePolicy().hasHeightForWidth());
        propertyListTreeView->setSizePolicy(sizePolicy);

        gridLayout->addWidget(propertyListTreeView, 5, 0, 1, 1);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        loadResponseButton = new QPushButton(gridLayoutWidget);
        loadResponseButton->setObjectName(QString::fromUtf8("loadResponseButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(loadResponseButton->sizePolicy().hasHeightForWidth());
        loadResponseButton->setSizePolicy(sizePolicy1);

        formLayout_2->setWidget(0, QFormLayout::SpanningRole, loadResponseButton);

        deleteResponseButton = new QPushButton(gridLayoutWidget);
        deleteResponseButton->setObjectName(QString::fromUtf8("deleteResponseButton"));
        sizePolicy1.setHeightForWidth(deleteResponseButton->sizePolicy().hasHeightForWidth());
        deleteResponseButton->setSizePolicy(sizePolicy1);

        formLayout_2->setWidget(1, QFormLayout::SpanningRole, deleteResponseButton);


        gridLayout->addLayout(formLayout_2, 6, 0, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 2, 0, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 4, 0, 1, 1);

        gridListTreeView = new QTreeView(gridLayoutWidget);
        gridListTreeView->setObjectName(QString::fromUtf8("gridListTreeView"));

        gridLayout->addWidget(gridListTreeView, 1, 0, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        line_2 = new QFrame(MDSMetricManagerFrame);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(340, -50, 20, 1181));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        scrollArea = new QScrollArea(MDSMetricManagerFrame);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setGeometry(QRect(370, 20, 1451, 1081));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
        scrollArea->setSizePolicy(sizePolicy2);
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 1449, 1079));
        scrollArea->setWidget(scrollAreaWidgetContents);

        retranslateUi(MDSMetricManagerFrame);

        QMetaObject::connectSlotsByName(MDSMetricManagerFrame);
    } // setupUi

    void retranslateUi(QFrame *MDSMetricManagerFrame)
    {
        MDSMetricManagerFrame->setWindowTitle(QApplication::translate("MDSMetricManagerFrame", "Frame", 0, QApplication::UnicodeUTF8));
        loadResponseButton->setText(QApplication::translate("MDSMetricManagerFrame", "Load Response", 0, QApplication::UnicodeUTF8));
        deleteResponseButton->setText(QApplication::translate("MDSMetricManagerFrame", "Delete Response", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MDSMetricManagerFrame", "Available Responses", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MDSMetricManagerFrame", "Corresponding Properties", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MDSMetricManagerFrame", "Available Grids", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MDSMetricManagerFrame: public Ui_MDSMetricManagerFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDSMETRICMANAGERFRAME_H
