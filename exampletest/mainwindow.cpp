/* Copyright (c) 2010, Antonie Jovanoski
 *
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#include <QNetworkAccessManager>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtweetfriendstimeline.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_oauthtwitter(new OAuthTwitter(this))
{
    ui->setupUi(this);

    m_oauthtwitter->setNetworkAccessManager(new QNetworkAccessManager(this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MainWindow::on_authenticateButton_clicked()
{
    m_oauthtwitter->authorizeXAuth(ui->userNameLineEdit->text(), ui->passwordLineEdit->text());

    ui->tokenLabel->setText(m_oauthtwitter->oauthToken());
    ui->tokenSecretLabel->setText(m_oauthtwitter->oauthTokenSecret());
}

void MainWindow::on_fetchFTPushButton_clicked()
{
    QTweetFriendsTimeline *friendsTimeline = new QTweetFriendsTimeline(m_oauthtwitter, this);
    connect(friendsTimeline, SIGNAL(finished(QByteArray)), this, SLOT(finishedFriendsTimeline(QByteArray)));

    QTweetNetBase::ResponseType respType;

    if (ui->typeFTComboBox->currentText() == "JSON")
        respType = QTweetNetBase::JSON;
    else
        respType = QTweetNetBase::Xml;

    bool skipUser;

    if (ui->skipUserFTComboBox->currentText() == "false")
        skipUser = false;
    else
        skipUser = true;

    bool includeRts;

    if (ui->includeRtsFTComboBox->currentText() == "false")
        includeRts = false;
    else
        includeRts = true;

    bool includeEntities;

    if (ui->includeEntitiesFTComboBox->currentText() == "false")
        includeEntities = false;
    else
        includeEntities = true;

    friendsTimeline->fetch(respType,
                           ui->sinceidFTLineEdit->text().toLongLong(),
                           ui->maxidFTLineEdit->text().toLongLong(),
                           ui->countFTLineEdit->text().toInt(),
                           ui->pageFTLineEdit->text().toInt(),
                           skipUser,
                           includeRts,
                           includeEntities);
}

void MainWindow::finishedFriendsTimeline(const QByteArray& response)
{
    QTweetFriendsTimeline *friendsTimeline = qobject_cast<QTweetFriendsTimeline*>(sender());

    if (friendsTimeline) {
        friendsTimeline->deleteLater();
    }

    ui->responseFTTextBrowser->setPlainText(response);
}
