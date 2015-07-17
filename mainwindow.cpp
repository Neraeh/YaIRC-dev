#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    irc = new Irc;
    irc->getChans()->append(new Chan(irc, "#yairc"));
    connect(irc, SIGNAL(debugOutput(QString)), this, SLOT(debugOutput(QString)));
    connect(irc, SIGNAL(networkOutput(QString)), this, SLOT(networkStatus(QString)));
    connect(irc, SIGNAL(onMessage(QString, QString, QString)), this, SLOT(onMessage(QString, QString, QString)));
    connect(irc, SIGNAL(onJoin(QString, QString, QString, QString)), this, SLOT(onJoin(QString, QString, QString, QString)));
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(changeChannel(QModelIndex)));
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(ui->actionFermer_l_onglet, SIGNAL(triggered()), this, SLOT(closeChannel()));
    connect(ui->actionPlein_cran, SIGNAL(toggled(bool)), this, SLOT(toggleFullscreen(bool)));
    irc->connect("YaIRC", "irc.t411.io", 6667, "YaIRC");
    chanList.append("Serveur");
    chanListModel = new QStringListModel;
    ui->listView->setModel(chanListModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::debugOutput(QString text)
{
    qDebug() << text;
}

void MainWindow::networkStatus(QString text)
{
    if (text == "Connected")
    {
        chanContent["Serveur"] += "Connecté\n";
        if (chan == "Serveur") ui->textEdit->append("Connecté");
        ui->actionConnecter->setText("Reconnecter");
    }
    else if (text.startsWith("Connecting"))
    {
        chanContent["Serveur"] += "Connexion en cours\n";
        if (chan == "Serveur") ui->textEdit->append("Connexion en cours");
    }
    else if (text.contains("error", Qt::CaseInsensitive))
    {
        if (text.startsWith("Error: "))
        {
            chanContent["Serveur"] += "Impossible de se connecter: " + text.mid(7) + "\n";
            if (chan == "Serveur") ui->textEdit->append("Impossible de se connecter: " + text.mid(7));
        }
        else
        {
            chanContent["Serveur"] += "Impossible de se connecter: erreur inconnue\n";
            if (chan == "Serveur") ui->textEdit->append("Impossible de se connecter: erreur inconnue");
        }
        ui->actionConnecter->setText("Connecter");
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    irc->disconnect();
    event->accept();
}

void MainWindow::changeChannel(QModelIndex chanSelected)
{
    chan = chanSelected.data().toString();
    ui->textEdit->setText(chanContent[chan]);
}

void MainWindow::closeChannel()
{
    irc->part(chan);
    chanList.removeAll(chan);
    chanListModel->setStringList(chanList);
    chan = chanList.at(0);
    ui->textEdit->setText(chanContent["Serveur"]);
}

void MainWindow::toggleFullscreen(bool checked)
{
    if (checked)
        this->showFullScreen();
    else
        this->showNormal();
}

void MainWindow::sendMessage()
{
    irc->sendMessage(chan, ui->lineEdit->text());
    chanContent[chan] += "<" + irc->getNick() + "> " + ui->lineEdit->text() + "\n";
    ui->textEdit->append("<" + irc->getNick() + "> " + ui->lineEdit->text());
    ui->lineEdit->clear();
}

void MainWindow::onMessage(QString sender, QString target, QString message)
{
    chanContent[target] += "<" + sender + "> " + message + "\n";
    if (target == chan) ui->textEdit->append("<" + sender + "> " + message);
}

void MainWindow::onJoin(QString nick, QString target, QString host, QString realname)
{
    if (nick == irc->getNick())
    {
        chanList.append(target);
        chanListModel->setStringList(chanList);
    }
    chanContent[target] += nick + " (" + realname + "@" + host + ") a rejoint " + target + "\n";
    if (target == chan) ui->textEdit->append(nick + " (" + realname + "@" + host + ") a rejoint " + target);
}
