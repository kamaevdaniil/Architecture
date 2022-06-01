#include "database.h"
#include "../config/config.h"

namespace database {
    Database& Database::get() {
        static Database instance;
        return instance;
    }

    Poco::Data::Session Database::createSession() {
        return Poco::Data::Session(Poco::Data::SessionFactory::instance().create(Poco::Data::MySQL::Connector::KEY, m_connectionString));
    }
    
    Database::Database() {
        m_connectionString += "host=";
        m_connectionString += Config::get().getHost();

        m_connectionString += ";user=";
        m_connectionString += Config::get().getLogin();

        m_connectionString += ";db=";
        m_connectionString += Config::get().getDatabase();

        m_connectionString += ";password=";
        m_connectionString += Config::get().getPassword();

        Poco::Data::MySQL::Connector::registerConnector();
    }
}