#ifndef QSSHFILE_H
#define QSSHFILE_H

#include <QIODevice>

#include <libssh2.h>
#include <libssh2_sftp.h>

class QSSHFile : public QIODevice
{
    Q_OBJECT

public:
    QSSHFile(LIBSSH2_SESSION *session, const QString &fileName,  QObject *parent);
    ~QSSHFile();

    bool isSequential() const;
protected:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

private:
    LIBSSH2_SFTP *sftp;
    LIBSSH2_SESSION *session;
    LIBSSH2_SFTP_HANDLE *sftpFile;

    qint64 filesize;
    qint64 pos;

    Q_DISABLE_COPY(QSSHFile)
};

#endif // QSSHFILE_H
