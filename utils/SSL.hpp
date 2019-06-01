#ifndef ANTARCTICA_SSL_HPP
#define ANTARCTICA_SSL_HPP

#include <QtNetwork/QSslConfiguration>

class SSL {
public:
    static const QSslConfiguration getConfig() {
        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        config.setCaCertificates(QSslCertificate::fromPath(":/certs/cert.pem"));
        config.setProtocol(QSsl::TlsV1_2OrLater);
        return config;
    }
};

#endif //ANTARCTICA_SSL_HPP
