#include "settingsserver.h"

#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QStringList>

#define mDebug qDebug
#define mInfo

SettingsServer * SettingsServer::inst = NULL;

SettingsServer::SettingsServer(QObject *parent) :
    QObject(parent)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), SLOT(newTcpConnection()));
    tcpServer->listen(QHostAddress::Any, 55555);

    inst = this;
}
void SettingsServer::sendToClients(QString msg)
{
    foreach (QTcpSocket *client, tcpClients)
        client->write(msg.toUtf8());
}
void SettingsServer::newTcpConnection()
{
    QTcpSocket *sock = tcpServer->nextPendingConnection();
    connect(sock, SIGNAL(disconnected()), SLOT(tcpClientDisconnected()));
    connect(sock, SIGNAL(readyRead()), SLOT(tcpReadyRead()));
    mDebug("client %s connected", qPrintable(sock->peerAddress().toString()));
    tcpClients << sock;
}
void SettingsServer::tcpClientDisconnected()
{
    QTcpSocket *sock = (QTcpSocket *)sender();
    if (tcpClients.contains(sock))
        tcpClients.removeAll(sock);
    mDebug("client gone: '%s'", qPrintable(sock->peerAddress().toString()));
    sock->deleteLater();
}
QByteArray SettingsServer::handleCommand(const QString &cmd)
{
    QByteArray ba;
    mDebug("%s", qPrintable(cmd));
    QStringList flds = cmd.split(" ", QString::SkipEmptyParts);
    if (flds[0] == "get") {
        if (flds.size() > 1)
            ba = handleGetCommand(flds[1]);
        else
            ba = QString("wrong command:%1").arg(cmd).toUtf8();
    } else if (flds[0] == "set") {
        if (flds.size() > 2)
            ba = handleSetCommand(flds[1], flds[2]);
        else
            ba = QString("wrong command:%1").arg(cmd).toUtf8();
    }
    return ba;
}
QByteArray SettingsServer::handleGetCommand(const QString &cmd)
{
    //return QString("get:%1:%2").arg(cmd).arg(gs(cmd).toString()).toUtf8();
}
QByteArray SettingsServer::handleSetCommand(const QString &cmd, const QString &value)
{
    //return QString("set:%1:%2").arg(cmd).arg(ss(cmd, value)).toUtf8();
}
void SettingsServer::tcpReadyRead()
{
    QTcpSocket *sock = (QTcpSocket *)sender();
#if 1
    QString s = QString::fromUtf8(sock->readAll()).trimmed();
    bool emitLast = false;
    if (s.endsWith("@"))
        emitLast = true;
    QStringList cmds = s.split("@", QString::SkipEmptyParts);
    mInfo("*** incoming: %s", qPrintable(s));
    for (int i = 0; i < cmds.size(); i++) {
        QString cmd = cmds[i];
        if (i == 0) {
            if (cmds.size() == 1) {
                /* this is also the last */
                if (emitLast) {
                    cmd.prepend(partialMessage);
                    partialMessage.clear();
                    mInfo("*** emit 1: %s", qPrintable(cmd));
                    emit dataReady(cmd);
                }
            } else {
                cmd.prepend(partialMessage);
                partialMessage.clear();
                mInfo("*** emit 2: %s", qPrintable(cmd));
                emit dataReady(cmd);
            }
        } else if (i == cmds.size() - 1) {
            if (emitLast) {
                mInfo("*** emit 3: %s", qPrintable(cmd));
                emit dataReady(cmd);
            } else {
                partialMessage = cmd;
            }
        } else {
            mInfo("*** emit 4: %s", qPrintable(cmd));
            emit dataReady(cmd);
        }
    }
#else
    msgBuffer.append(QString::fromUtf8(sock->readAll()));
    if (msgBuffer.endsWith("\n")) {
        QStringList cmds = msgBuffer.split("\n");
        foreach (QString cmd, cmds) {
            cmd = cmd.trimmed();
            if (cmd.isEmpty())
                continue;
            QByteArray ba = handleCommand(cmd);
            if (ba.size()) {
                ba.append(QChar('\n'));
                sock->write(ba);
            } else
                emit dataReady(cmd);
        }
        msgBuffer.clear();
    }
#endif
}
