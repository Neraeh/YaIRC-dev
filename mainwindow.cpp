#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    textDialog = new TextDialog(this);
    irc = new Irc;
    QObject::connect(irc, SIGNAL(debugOutput(QString)), this, SLOT(debugOutput(QString)));
    QObject::connect(irc, SIGNAL(networkOutput(QString)), this, SLOT(networkStatus(QString)));
    QObject::connect(irc, SIGNAL(onMessage(QString, QString, QString)), this, SLOT(onMessage(QString, QString, QString)));
    QObject::connect(irc, SIGNAL(onJoin(QString, QString, QString, QString)), this, SLOT(onJoin(QString, QString, QString, QString)));
    QObject::connect(irc, SIGNAL(usersChange(QString, QStringList)), this, SLOT(displayUsers(QString, QStringList)));
    QObject::connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(changeChannel(QModelIndex)));
    QObject::connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    QObject::connect(ui->actionFermer_l_onglet, SIGNAL(triggered()), this, SLOT(closeChannel()));
    QObject::connect(ui->actionPlein_cran, SIGNAL(toggled(bool)), this, SLOT(toggleFullscreen(bool)));
    QObject::connect(ui->pushButton, SIGNAL(released()), this, SLOT(nickDialog()));
    QObject::connect(ui->actionRe_joindre_un_channel, SIGNAL(triggered()), this, SLOT(joinDialog()));
    QObject::connect(ui->actionConnecter, SIGNAL(triggered()), this, SLOT(connect()));
    QObject::connect(ui->actionD_connecter, SIGNAL(triggered()), this, SLOT(disconnect()));
    chanList.append("Serveur");
    chanListModel = new QStringListModel;
    chanListModel->setStringList(chanList);
    userListModel = new QStringListModel;
    ui->listView->setModel(chanListModel);
    ui->listView_2->setModel(userListModel);
    this->setTabOrder(ui->lineEdit, ui->listView);
    ui->lineEdit->setFocus();
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
        ui->actionD_connecter->setEnabled(true);
        ui->actionFermer_l_onglet->setEnabled(true);
        ui->actionRe_joindre_un_channel->setEnabled(true);
        ui->action_Liste_des_channels->setEnabled(true);
    }
    else if (text.startsWith("Connecting"))
    {
        chanContent["Serveur"] += "Connexion en cours\n";
        if (chan == "Serveur") ui->textEdit->append("Connexion en cours");
    }
    else if (text == "Disconnected")
    {
        chanContent["Serveur"] += "Déconnecté\n";
        if (chan == "Serveur") ui->textEdit->append("Déconnecté");
        else ui->textEdit->clear();
        foreach (QString w, chanContent.keys()) if (w != "Serveur") chanContent.remove(w);
        foreach (QString w, chanList) if (w != "Serveur") chanList.removeOne(w);
        chanListModel->setStringList(chanList);
        userListModel->setStringList(QStringList());
        ui->actionConnecter->setText("Connecter");
        ui->actionD_connecter->setEnabled(false);
        ui->actionFermer_l_onglet->setEnabled(false);
        ui->actionRe_joindre_un_channel->setEnabled(false);
        ui->action_Liste_des_channels->setEnabled(false);
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
        if (chan != "Serveur") ui->textEdit->clear();
        foreach (QString w, chanContent.keys()) if (w != "Serveur") chanContent.remove(w);
        foreach (QString w, chanList) if (w != "Serveur") chanList.removeOne(w);
        chanListModel->setStringList(chanList);
        userListModel->setStringList(QStringList());
        ui->actionConnecter->setText("Connecter");
        ui->actionD_connecter->setEnabled(false);
        ui->actionFermer_l_onglet->setEnabled(false);
        ui->actionRe_joindre_un_channel->setEnabled(false);
        ui->action_Liste_des_channels->setEnabled(false);
    }
}

void MainWindow::connect()
{
    if (irc->isConnected())
        this->disconnect();
    irc->connect(ui->pushButton->text(), "irc.t411.io", 6667, ui->pushButton->text());
}

void MainWindow::disconnect()
{
    if (irc->isConnected())
        irc->disconnect();
    chanListModel->removeColumn(0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (irc->isConnected()) irc->disconnect();
    event->accept();
}

void MainWindow::nickDialog()
{
    dialog = "nick";
    textDialog->exec();
}

void MainWindow::joinDialog()
{
    dialog = "join";
    textDialog->exec();
}

void MainWindow::dialogFinished(QString value)
{
    if (dialog == "nick" && value != "" && irc->getNick() != value)
    {
        irc->setNick(value);
        ui->pushButton->setText(value);
    }
    else if (dialog == "join" && value.startsWith("#"))
        irc->join(value);
}

void MainWindow::changeChannel(QModelIndex chanSelected)
{
    chan = chanSelected.data().toString();
    ui->textEdit->setText(chanContent[chan]);
    userListModel->setStringList(chanUserlist[chan]);
}

void MainWindow::closeChannel()
{
    if (chan != "Serveur" && chanListModel->hasIndex(0, 0))
    {
        irc->part(chan);
        chanList.removeAll(chan);
        chanListModel->setStringList(chanList);
        userListModel->removeColumn(0);
        chanContent.remove(chan);
        chan = chanList.at(0);
        ui->textEdit->setText(chanContent["Serveur"]);
    }
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

void MainWindow::displayUsers(QString channel, QStringList users)
{
    if (chan == channel)
        userListModel->setStringList(users);
    chanUserlist[channel] = users;
}
