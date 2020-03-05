#ifndef REMOTECONSOLE_H
#define REMOTECONSOLE_H

#include <QObject>
#include <QHostAddress>

class QUdpSocket;
class QDataStream;

class RemoteDataStream
{
public:
	RemoteDataStream();
	const char * getData() {  return data; }
	int getDataLen() { return 1460; }
	void addData(int d, int pos);
	void addData(float d, int pos);
	void addChData(float ch1);
	void addChData(float ch1, float ch2);
	void addChData(float ch1, float ch2, float ch3);
	void addChData(float ch1, float ch2, float ch3, float ch4);
	void addData(char c, int pos);
protected:
	char data[1460];
};

class RemoteConsole : public QObject
{
	Q_OBJECT
public:
	explicit RemoteConsole(int port = 8945, QObject *parent = 0);
	int send(const char *data, int len);
	bool isStreamEnabled() { return streamEnabled; }
	static RemoteConsole * instance() { return inst; }

signals:
	
private slots:
	void readPendingDatagrams();
protected:
	QUdpSocket *sock;
	QHostAddress streamHost;
	qint16 streamHostPort;
	bool streamEnabled;
	static RemoteConsole *inst;

	QString processMessage(QString mes, const QHostAddress sender, qint16 senderPort);
};

#endif // REMOTECONSOLE_H
