#ifndef MYSQL_DATABASE_WRAPPER_HPP_
#define MYSQL_DATABASE_WRAPPER_HPP_

#include <stdexcept>
#include <mutex>

#include "IDatabaseWrapper.hpp"

#define SOCI_USE_BOOST

#include "common/external/soci/src/core/soci.h"
#include "common/external/soci/src/backends/mysql/soci-mysql.h"
#include "common/utilities/Console.hpp"
#include <boost/tuple/tuple.hpp>

class DatabaseWrapper : public IDatabaseWrapper
{
public:
	DatabaseWrapper(std::string databaseName) :
		console_("DatabaseWrapper")
	{
		try
		{
			databaseConnector_ = std::make_shared<soci::session>(soci::mysql,
				"db=" + databaseName + " user=root password='root'");
		}
		catch(const std::runtime_error& e)
		{
		  //throw std::runtime_error(e.what());
		}
	}

	Users getUsers();
	Users getUsersBy(UserTypes type, unsigned& id);
	Users getUsersBy(UserTypes type, std::string& login);
	Users getUsersBy(UserTypes type, std::vector<int> position);
	Users getUsersBy(UserTypes type, bool isOnline);
	Users getAllUsersExcept(unsigned& id);
	bool updateUser(User userToUpdate);
private:
	MarkedPositions checkMarkedPositions(std::vector<int> position);
	bool userIsEqualToPattern(unsigned idToCheck, User userToCheck);
	bool areUsersEqual(User alice, User bob);

	std::shared_ptr<soci::session> databaseConnector_;
	Console console_;
	std::mutex connectorMutex;
};

#endif 