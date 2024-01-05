#pragma once
#include <iostream>
// Header files for Boost Serialization
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;
using namespace boost;

// Class object for leaderboards
class LeaderboardQuizItem {
private:
	// Username
	string userName;
	
	// Number of correct answers
	int correctAnswers;

	// Date and time in which the game was recorded
	string date;

public:
	// Get username
	string getUserName() { return userName; }

	// Get correct answers
	int getCorrectAnswers() { return correctAnswers; }

	// Get date
	string getDate() { return date; }

	// Set username
	string setUserName(string u) { return userName = u; }

	// Set number of correct answers
	int setCorrectAnswers(int c) { return correctAnswers = c; }

	// Set date
	string setDate(string d) { return date = d; }

	// Boost serialisation
	friend class serialization::access;
	template<class Archive>
	void serialize(Archive& arch, const unsigned int version) {
		arch& userName;
		arch& correctAnswers;
		arch& date;
	}
};