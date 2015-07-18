#include "textdialog.h"

TextDialog::TextDialog(MainWindow *parent = 0) : QDialog(parent, Qt::WindowTitleHint)
{
    this->setFixedWidth(220);
    layout = new QHBoxLayout;
    lineEdit = new QLineEdit(this);
    lineEdit->setFixedWidth(200);
    layout->addWidget(lineEdit);
    lineEdit->setFocus();
    this->setWindowModality(Qt::ApplicationModal);
    connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(returnValue()));
    connect(this, SIGNAL(finished(QString)), parent, SLOT(dialogFinished(QString)));
}

TextDialog::~TextDialog() {}

void TextDialog::returnValue()
{
    emit finished(lineEdit->text());
    lineEdit->clear();
    this->close();
}
