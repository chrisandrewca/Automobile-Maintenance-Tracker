#ifndef _AMT_DATABASE_HPP_
#define _AMT_DATABASE_HPP_

#include "DataStore.hpp"
#include "sqlite\sqlite3.h"
#include <string>

namespace AMT
{

class Database : public DataStore
{
public:
	Database();
	~Database();

	bool setup(const char* databaseName, std::string& errorMessage);

private:
	sqlite3* sqlite3;

};

}

#endif