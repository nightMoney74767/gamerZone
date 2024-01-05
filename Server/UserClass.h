#pragma once
#include <iostream>
// Header files for Boost Serialization
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;
using namespace boost;

// Class object representing users
class User {
private:
	// Username
	string userName;

	// User password
	size_t userPassword;

	// User type. This should be 0 for regular users and 1 for admins
	int userType;

public:
	// Get the username
	string getUserName() { return userName; }

	// Get user type
	int getUserType() { return userType; }

	// Set the username
	string setUserName(string u) { return userName = u; }

	// Set the password
	// Note: passwords must be hashed for security reasons
	size_t setPassword(size_t p) {
		return userPassword = p;
	}

	// Set the usertype
	int setUserType(int t) { return userType = t; }

	// Get the hashed password
	size_t getPasswordHash() { return userPassword; }

	// Boost serialisation
	friend class serialization::access;
	template<class Archive>
	void serialize(Archive& arch, const unsigned int version) {
		arch& userName;
		arch& userPassword;
		arch& userType;
	}
};