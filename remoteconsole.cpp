#include "remoteconsole.h"
#include "dc_motor_pc.h"

#include <QUdpSocket>
#include <QStringList>

RemoteConsole * RemoteConsole::inst = NULL;

RemoteConsole::RemoteConsole(int port, QObject *parent) :
    QObject(parent)
{
    inst = this;
    sock = new QUdpSocket(this);
    sock->bind(QHostAddress::Any, port);
    streamEnabled = false;

    connect(sock, SIGNAL(readyRead()), SLOT(readPendingDatagrams()));
}

int RemoteConsole::send(const char *data, int len)
{
    sock->writeDatagram(data, len, streamHost, streamHostPort);
    return 0;
}
void RemoteConsole::readPendingDatagrams()
{
    while (sock->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(sock->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        sock->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        QByteArray ba = processMessage(QString::fromUtf8(datagram), sender, senderPort).toUtf8();
        if (ba.size())
            sock->writeDatagram(ba, sender, senderPort);
    }
}
static QString cerror(int code = 0)
{
    return "";
}
QString RemoteConsole::processMessage(QString mes,  const QHostAddress sender, qint16 senderPort)
{
    if (mes.trimmed().isEmpty())
        return cerror();
    QString resp;
    QStringList fields = mes.trimmed().split(" ", QString::SkipEmptyParts);
    if (fields.size() > 1 && fields[0] == "get") {
        QString set = fields[1].trimmed();
        DC_Motor_PC *motor = (DC_Motor_PC *)parent();
        resp = QString("get:%1:%2").arg(set).arg(motor->readSettingEthernet(set));
    } else if (fields.size() > 2 && fields[0] == "set") {
        QString set = fields[1].trimmed();
        QString value = fields[2].trimmed();

        if (set == "device_settings.stream.enable") {
            if (value.toInt()) {
                streamHost = sender;
                streamHostPort = senderPort;
                streamEnabled = true;
            } else {
                streamEnabled = false;
            }
            return QString("set:%1:%2").arg(set).arg(0);
        }

        /*resp = QString("set:%1:%2").arg(set).arg(0);*/
        DC_Motor_PC *motor = (DC_Motor_PC *)parent();
        resp = QString("set:%1:%2").arg(set).arg(motor->setSettingEthernet(set, value));
    }
    return resp;
}
RemoteDataStream::RemoteDataStream()
{
    memset(data, 0, 1460);
}
void RemoteDataStream::addData(int d, int pos)
{
    memcpy(&data[pos], &d, 4);
}
void RemoteDataStream::addData(float d, int pos)
{
    memcpy(&data[pos], &d, 4);
}
void RemoteDataStream::addChData(float ch1)
{
    memcpy(&data, &ch1, 4);
}
void RemoteDataStream::addChData(float ch1, float ch2)
{
    memcpy(&data, &ch1, 4);
    memcpy(&data + 4, &ch2, 4);
}
void RemoteDataStream::addChData(float ch1, float ch2, float ch3)
{
    memcpy(&data, &ch1, 4);
    memcpy(&data + 4, &ch2, 4);
    memcpy(&data + 8, &ch3, 4);
}
void RemoteDataStream::addChData(float ch1, float ch2, float ch3, float ch4)
{
    memcpy(&data, &ch1, 4);
    memcpy(&data + 4, &ch2, 4);
    memcpy(&data + 8, &ch3, 4);
    memcpy(&data + 12, &ch4, 4);
}
void RemoteDataStream::addData(char c, int pos)
{
    data[pos] = c;
}
