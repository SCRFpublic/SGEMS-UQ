/********************************************************************************
** Form generated from reading UI file 'param_search_widget.ui'
**
** Created: Thu Dec 6 00:22:36 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARAM_SEARCH_WIDGET_H
#define UI_PARAM_SEARCH_WIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QScrollArea>
#include <QtGui/QStackedWidget>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_param_search_widget
{
public:
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *comboBoxProperty;
    QScrollArea *scrollAreaParam;
    QWidget *scrollAreaWidgetContents;
    QWidget *page_2;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QComboBox *comboBox_3;
    QComboBox *comboBox_4;
    QComboBox *comboBox_5;
    QComboBox *comboBox_2;
    QFrame *line_2;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QLabel *label_10;
    QComboBox *comboBoxSearch;
    QLineEdit *lineEditSearchValue;
    QLabel *label_11;
    QPushButton *pushButtonSearch;
    QTableView *tableViewResults;
    QFrame *line;
    QPushButton *pushButtonPlot;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QLineEdit *lineEditParam;
    QLabel *label_3;
    QLineEdit *lineEditType;
    QLabel *label_4;
    QLineEdit *lineEditCastable;
    QLabel *label_5;
    QLineEdit *lineEditValue;

    void setupUi(QWidget *param_search_widget)
    {
        if (param_search_widget->objectName().isEmpty())
            param_search_widget->setObjectName(QString::fromUtf8("param_search_widget"));
        param_search_widget->resize(375, 694);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(param_search_widget->sizePolicy().hasHeightForWidth());
        param_search_widget->setSizePolicy(sizePolicy);
        stackedWidget = new QStackedWidget(param_search_widget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setGeometry(QRect(20, -10, 331, 251));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayoutWidget = new QWidget(page);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 20, 301, 231));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        comboBoxProperty = new QComboBox(verticalLayoutWidget);
        comboBoxProperty->setObjectName(QString::fromUtf8("comboBoxProperty"));

        verticalLayout->addWidget(comboBoxProperty);

        scrollAreaParam = new QScrollArea(verticalLayoutWidget);
        scrollAreaParam->setObjectName(QString::fromUtf8("scrollAreaParam"));
        scrollAreaParam->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 297, 171));
        scrollAreaParam->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollAreaParam);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        gridLayoutWidget = new QWidget(page_2);
        gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 20, 160, 128));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        comboBox_3 = new QComboBox(gridLayoutWidget);
        comboBox_3->setObjectName(QString::fromUtf8("comboBox_3"));

        gridLayout->addWidget(comboBox_3, 3, 0, 1, 1);

        comboBox_4 = new QComboBox(gridLayoutWidget);
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));

        gridLayout->addWidget(comboBox_4, 2, 0, 1, 1);

        comboBox_5 = new QComboBox(gridLayoutWidget);
        comboBox_5->setObjectName(QString::fromUtf8("comboBox_5"));

        gridLayout->addWidget(comboBox_5, 1, 0, 1, 1);

        comboBox_2 = new QComboBox(gridLayoutWidget);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));

        gridLayout->addWidget(comboBox_2, 0, 0, 1, 1);

        stackedWidget->addWidget(page_2);
        line_2 = new QFrame(param_search_widget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(-140, 380, 521, 21));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);
        gridLayoutWidget_3 = new QWidget(param_search_widget);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(0, 390, 371, 71));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_10 = new QLabel(gridLayoutWidget_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 0, 0, 1, 1);

        comboBoxSearch = new QComboBox(gridLayoutWidget_3);
        comboBoxSearch->setObjectName(QString::fromUtf8("comboBoxSearch"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBoxSearch->sizePolicy().hasHeightForWidth());
        comboBoxSearch->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(comboBoxSearch, 1, 0, 1, 1);

        lineEditSearchValue = new QLineEdit(gridLayoutWidget_3);
        lineEditSearchValue->setObjectName(QString::fromUtf8("lineEditSearchValue"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineEditSearchValue->sizePolicy().hasHeightForWidth());
        lineEditSearchValue->setSizePolicy(sizePolicy2);

        gridLayout_3->addWidget(lineEditSearchValue, 1, 1, 1, 1);

        label_11 = new QLabel(gridLayoutWidget_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 0, 1, 1, 1);

        pushButtonSearch = new QPushButton(param_search_widget);
        pushButtonSearch->setObjectName(QString::fromUtf8("pushButtonSearch"));
        pushButtonSearch->setGeometry(QRect(40, 640, 97, 27));
        tableViewResults = new QTableView(param_search_widget);
        tableViewResults->setObjectName(QString::fromUtf8("tableViewResults"));
        tableViewResults->setGeometry(QRect(10, 470, 351, 161));
        line = new QFrame(param_search_widget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 240, 371, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        pushButtonPlot = new QPushButton(param_search_widget);
        pushButtonPlot->setObjectName(QString::fromUtf8("pushButtonPlot"));
        pushButtonPlot->setGeometry(QRect(240, 640, 97, 27));
        layoutWidget = new QWidget(param_search_widget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(60, 250, 233, 134));
        gridLayout_2 = new QGridLayout(layoutWidget);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        lineEditParam = new QLineEdit(layoutWidget);
        lineEditParam->setObjectName(QString::fromUtf8("lineEditParam"));

        gridLayout_2->addWidget(lineEditParam, 0, 1, 1, 1);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        lineEditType = new QLineEdit(layoutWidget);
        lineEditType->setObjectName(QString::fromUtf8("lineEditType"));

        gridLayout_2->addWidget(lineEditType, 1, 1, 1, 1);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        lineEditCastable = new QLineEdit(layoutWidget);
        lineEditCastable->setObjectName(QString::fromUtf8("lineEditCastable"));

        gridLayout_2->addWidget(lineEditCastable, 2, 1, 1, 1);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);

        lineEditValue = new QLineEdit(layoutWidget);
        lineEditValue->setObjectName(QString::fromUtf8("lineEditValue"));

        gridLayout_2->addWidget(lineEditValue, 3, 1, 1, 1);


        retranslateUi(param_search_widget);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(param_search_widget);
    } // setupUi

    void retranslateUi(QWidget *param_search_widget)
    {
        param_search_widget->setWindowTitle(QApplication::translate("param_search_widget", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("param_search_widget", "Realization", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("param_search_widget", "Search Type", 0, QApplication::UnicodeUTF8));
        comboBoxSearch->clear();
        comboBoxSearch->insertItems(0, QStringList()
         << QApplication::translate("param_search_widget", "<=", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("param_search_widget", "<", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("param_search_widget", ">", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("param_search_widget", ">=", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("param_search_widget", "==", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("param_search_widget", "!=", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("param_search_widget", "+/-", 0, QApplication::UnicodeUTF8)
        );
        label_11->setText(QApplication::translate("param_search_widget", "Value", 0, QApplication::UnicodeUTF8));
        pushButtonSearch->setText(QApplication::translate("param_search_widget", "Search", 0, QApplication::UnicodeUTF8));
        pushButtonPlot->setText(QApplication::translate("param_search_widget", "Plot", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("param_search_widget", "Parameter", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("param_search_widget", "Type", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("param_search_widget", "Castable", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("param_search_widget", "Value", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class param_search_widget: public Ui_param_search_widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARAM_SEARCH_WIDGET_H
