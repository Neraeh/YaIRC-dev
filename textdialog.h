#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include "mainwindow.h"

class MainWindow;
class TextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextDialog(MainWindow *parent);
    ~TextDialog();

signals:
    void finished(QString);

private slots:
    void returnValue();

private:
    QLineEdit *lineEdit;
    QPushButton *ok;
    QHBoxLayout *layout;
};

#endif // TEXTDIALOG_H
