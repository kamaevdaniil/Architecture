#include "person.h"
#include "database.h"
#include "../config/config.h"

#include "Poco/Data/MySQL/Connector.h"
#include "Poco/Data/MySQL/MySQLException.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"

#include <sstream>
#include <exception>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;

namespace database {
    void Person::init() {
        try 
        {
            Poco::Data::Session session = database::Database::get().createSession();

            Statement dropStatement(session);
            dropStatement << "DROP TABLE IF EXISTS Person", now;

            Statement createStatement(session);
            createStatement << "CREATE TABLE IF NO EXISTS `Person` (`login` VARCHAR(256),"
                            << "`first_name` VARCHAR(256),"
                            << "`last_name` VARCHAR(256),"
                            << "`age` SMALLINT,"
                            << "CONSTRAINT `pk_login` PRIMARY KEY (`login`) );",
                            now;
        }

        catch (Poco::Data::MySQL::ConnectionException &error) {
            std::cout << "connection:" << error.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException &error){
            std::cout << "statement:" << error.what() << std::endl;
            throw;
        }
    }

    Poco::JSON::Object::Ptr Person::toJSON() const {
        Poco::JSON::Object::Ptr pRoot = new Poco::JSON::Object();

        pRoot->set("login", m_login);
        pRoot->set("first_name", m_firstName);
        pRoot->set("last_name", m_lastName);
        pRoot->set("age", m_age);

        return pRoot;
    }

    Person Person::fromJSON(const std::string& str) {
        Person person;
        Poco::JSON::Parser parser;
        Poco::Dynamic::Var result = parser.parse(str);
        Poco::JSON::Object::Ptr pObject = result.extract<Poco::JSON::Object::Ptr>();

        person.login() = pObject->getValue<std::string>("login");
        person.firstName() = pObject->getValue<std::string>("first_name");
        person.lastName() = pObject->getValue<std::string>("last_name");
        person.age() = pObject->getValue<long>("age");

        return person;
    }
    
    Person Person::readByLogin(std::string login) {
        try {
            Poco::Data::Session session = database::Database::get().createSession();
            Poco::Data::Statement select(session);
            Person person;
            select << "SELECT login, first_name, last_name, age FROM Person where login=?",
                into(person.m_login),
                into(person.m_firstName),
                into(person.m_lastName),
                into(person.m_age),
                use(login),
                range(0, 1);
            
            select.execute();
            Poco::Data::RecordSet recordSet(select);
            if(!recordSet.moveFirst()) throw std::logic_error("not found");

            return person;
        }
        catch (Poco::Data::MySQL::ConnectionException& error) {
            std::cout << "connection:" << error.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException& error) {
            std::cout << "statement:" << error.what() << std::endl;
            throw;
        }
    }

    std::vector<Person> Person::readAll() {
        try {
            Poco::Data::Session session = database::Database::get().createSession();
            Statement select(session);
            std::vector<Person> result;
            Person person;

            select << "SELECT login, first_name, last_name, age FROM Person",
                into(person.m_login),
                into(person.m_firstName),
                into(person.m_lastName),
                into(person.m_age);
                range(0, 1);
            
            while (!select.done()) {
                if (select.execute()) {
                    result.push_back(person);
                }
            }
            return result;
        }
        catch (Poco::Data::MySQL::ConnectionException& error) {
            std::cout << "connection:" << error.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException& error) {
            std::cout << "statement:" << error.what() << std::endl;
            throw;
        }
    }

    std::vector<Person> Person::readByMask(std::string firstName, std::string lastName) {
        try {
            Poco::Data::Session session = database::Database::get().createSession();
            Statement select(session);
            std::vector<Person> result;
            Person person;

            firstName += "%";
            lastName += "%";
            select << "SELECT login, first_name, last_name, age FROM Person where first_name LIKE ? and last_name LIKE ?",
                into(person.m_login),
                into(person.m_firstName),
                into(person.m_lastName),
                into(person.m_age),
                use(firstName),
                use(lastName),
                range(0, 1);

            while (!select.done()) {
                if (select.execute()) result.push_back(person);
            }
            return result;
        }
        catch (Poco::Data::MySQL::ConnectionException& error) {
            std::cout << "connection:" << error.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException& error) {
            std::cout << "statement:" << error.what() << std::endl;
            throw;
        }
    }

    void Person::saveToMysql() {
        try {
            Poco::Data::Session session = database::Database::get().createSession();
            Poco::Data::Statement insert(session);

            insert << "INSERT INTO Person (login,first_name,last_name,age) VALUES(?, ?, ?, ?)",
                use(m_login),
                use(m_firstName),
                use(m_lastName),
                use(m_age);

            insert.execute();
            std::cout << "inserted: " << m_login << " " << m_firstName << " " << m_lastName << " " << m_age;
        }
        catch (Poco::Data::MySQL::ConnectionException& error) {
            std::cout << "connection:" << error.what() << std::endl;
            throw;
        }
        catch (Poco::Data::MySQL::StatementException& error) {
            std::cout << "statement:" << error.what() << std::endl;
            throw;
        }
    }

    const std::string& Person::getLogin() const {
        return m_login;
    }

    const std::string& Person::getFirstName() const {
        return m_firstName;
    }

    const std::string& Person::getLastName() const {
        return m_lastName;
    }

    long Person::getAge() const {
        return m_age;
    }

    std::string& Person::login() {
        return m_login;
    }

    std::string& Person::firstName() {
        return m_firstName;
    }

    std::string& Person::lastName() {
        return m_lastName;
    }

    long& Person::age() {
        return m_age;
    }
}