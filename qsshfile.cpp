#include "qsshfile.h"
#include "common.h"

QSSHFile::QSSHFile(LIBSSH2_SESSION *session, const QString &fileName, QObject *parent) : QIODevice(parent)
{
    int libssh_errno;
    this->session = session;
    sftp = libssh2_sftp_init(session);


    QString fname = __FILE_PATH__ + fileName;

    LIBSSH2_SFTP_ATTRIBUTES attrs;
    libssh2_sftp_stat(sftp, fname.toLocal8Bit().data(), &attrs);
    filesize = attrs.filesize;
    pos = 0;

    while (true) {
        sftpFile = libssh2_sftp_open(sftp, fname.toLocal8Bit().data(), LIBSSH2_FXF_READ, 0);
        if (sftpFile)
            break;
        libssh_errno = libssh2_session_last_errno(session);
        if (libssh_errno != LIBSSH2_ERROR_EAGAIN)
            break;
    }

    if (!sftpFile) {
        if (sftpFile)
             libssh2_sftp_close(sftpFile);
        throw(std::runtime_error(tr("Ошибка открытия файла по sh2").toStdString()));
    }
}


QSSHFile::~QSSHFile()
{
    if (sftpFile)
         libssh2_sftp_close(sftpFile);

    if (sftp)
        libssh2_sftp_shutdown(sftp);
}

qint64 QSSHFile::readData(char *data, qint64 maxlen)
{
    if (!sftpFile)
        return 0;

    ssize_t rc = libssh2_sftp_read(sftpFile, data, maxlen);
    if (rc > 0)
        pos += rc;

    return rc;
}

qint64 QSSHFile::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;

}

qint64 QSSHFile::bytesAvailable() const
{
    return (filesize - pos) + QIODevice::bytesAvailable();;
}

bool QSSHFile::isSequential() const
{
    return true;
}

