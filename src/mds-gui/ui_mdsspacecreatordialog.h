/********************************************************************************
** Form generated from reading UI file 'mdsspacecreatordialog.ui'
**
** Created: Fri Apr 26 17:30:34 2013
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MDSSPACECREATORDIALOG_H
#define UI_MDSSPACECREATORDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MDSSpaceCreatorDialog
{
public:
    QGridLayout *gridLayout_2;
    QVBoxLayout *metricLayout;
    QLabel *label_4;
    QVBoxLayout *kernelLayout;
    QGridLayout *metricNameLayout;
    QLineEdit *gridNameLineEdit;
    QVBoxLayout *propertiesLayout;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_5;
    QGridLayout *gridLayout;
    QPushButton *generateButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *MDSSpaceCreatorDialog)
    {
        if (MDSSpaceCreatorDialog->objectName().isEmpty())
            MDSSpaceCreatorDialog->setObjectName(QString::fromUtf8("MDSSpaceCreatorDialog"));
        MDSSpaceCreatorDialog->resize(833, 520);
        gridLayout_2 = new QGridLayout(MDSSpaceCreatorDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        metricLayout = new QVBoxLayout();
        metricLayout->setObjectName(QString::fromUtf8("metricLayout"));
        label_4 = new QLabel(MDSSpaceCreatorDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        metricLayout->addWidget(label_4);


        gridLayout_2->addLayout(metricLayout, 1, 1, 4, 1);

        kernelLayout = new QVBoxLayout();
        kernelLayout->setObjectName(QString::fromUtf8("kernelLayout"));

        gridLayout_2->addLayout(kernelLayout, 1, 2, 2, 1);

        metricNameLayout = new QGridLayout();
        metricNameLayout->setObjectName(QString::fromUtf8("metricNameLayout"));
        gridNameLineEdit = new QLineEdit(MDSSpaceCreatorDialog);
        gridNameLineEdit->setObjectName(QString::fromUtf8("gridNameLineEdit"));

        metricNameLayout->addWidget(gridNameLineEdit, 2, 1, 1, 1);


        gridLayout_2->addLayout(metricNameLayout, 1, 0, 1, 1);

        propertiesLayout = new QVBoxLayout();
        propertiesLayout->setObjectName(QString::fromUtf8("propertiesLayout"));
        label_2 = new QLabel(MDSSpaceCreatorDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        propertiesLayout->addWidget(label_2);


        gridLayout_2->addLayout(propertiesLayout, 2, 0, 3, 1);

        label_3 = new QLabel(MDSSpaceCreatorDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(label_3, 0, 1, 1, 1);

        label = new QLabel(MDSSpaceCreatorDialog);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(MDSSpaceCreatorDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        sizePolicy1.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(label_5, 0, 2, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        generateButton = new QPushButton(MDSSpaceCreatorDialog);
        generateButton->setObjectName(QString::fromUtf8("generateButton"));

        gridLayout->addWidget(generateButton, 0, 0, 1, 1);

        cancelButton = new QPushButton(MDSSpaceCreatorDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 3, 2, 1, 1);


        retranslateUi(MDSSpaceCreatorDialog);

        QMetaObject::connectSlotsByName(MDSSpaceCreatorDialog);
    } // setupUi

    void retranslateUi(QDialog *MDSSpaceCreatorDialog)
    {
        MDSSpaceCreatorDialog->setWindowTitle(QApplication::translate("MDSSpaceCreatorDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_4->setText(QString());
        gridNameLineEdit->setText(QApplication::translate("MDSSpaceCreatorDialog", "DefaultTestName", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MDSSpaceCreatorDialog", "Select Realization", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MDSSpaceCreatorDialog", "Select Metric", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MDSSpaceCreatorDialog", "Enter Metric Space Name:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MDSSpaceCreatorDialog", "Select Kernel", 0, QApplication::UnicodeUTF8));
        generateButton->setText(QApplication::translate("MDSSpaceCreatorDialog", "Generate", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("MDSSpaceCreatorDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MDSSpaceCreatorDialog: public Ui_MDSSpaceCreatorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MDSSPACECREATORDIALOG_H
