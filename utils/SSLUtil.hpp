#ifndef ANTARCTICA_SSLUTIL_HPP
#define ANTARCTICA_SSLUTIL_HPP

#include <QtNetwork/QSslConfiguration>

class SSLUtil {
public:
    static const QSslConfiguration getSSLConfig() {
        QSslConfiguration config = QSslConfiguration::defaultConfiguration();
        config.setCaCertificates(QSslCertificate::fromPath(":/certs/cert.pem"));
        config.setProtocol(QSsl::TlsV1_2OrLater);
        return config;
    }
};

#endif //ANTARCTICA_SSLUTIL_HPP
