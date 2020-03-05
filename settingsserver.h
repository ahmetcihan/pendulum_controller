#ifndef SETTINGSSERVER_H
#define SETTINGSSERVER_H

#include <QObject>

class QTcpServer;
class QTcpSocket;

/* TODO: Make this class singleton */
class SettingsServer : public QObject
{
	Q_OBJECT
public:
	explicit SettingsServer(QObject *parent = 0);
	void sendToClients(QString msg);
	static SettingsServer * instance() { return inst; }
signals:
	void dataReady(const QString &);
protected slots:
	void tcpReadyRead();
	void newTcpConnection();
	void tcpClientDisconnected();
protected:
	QString msgBuffer;
	QString partialMessage;
	QTcpServer *tcpServer;
	QList<QTcpSocket *> tcpClients;
	static SettingsServer *inst;

	QByteArray handleCommand(const QString &cmd);
	QByteArray handleGetCommand(const QString &cmd);
	QByteArray handleSetCommand(const QString &cmd, const QString &value);
};

#endif // SETTINGSSERVER_H
