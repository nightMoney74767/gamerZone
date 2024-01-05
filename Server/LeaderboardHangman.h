#include <iostream>
// Header files for Boost Serialization
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;
using namespace boost;

// Class object for leaderboards
class LeaderboardHangmanItem {
private:
	// Username
	string userName;

	// Outcome of the game (true if the player won; false otherwise)
	bool outcome;

	// Number of guesses
	int guesses;

	// Date and time in which the game was recorded
	string date;

public:
	// Get username
	string getUserName() { return userName; }

	// Get outcome
	bool getOutcome() { return outcome; }

	// Get correct answers
	int getGuesses() { return guesses; }

	// Get date
	string getDate() { return date; }

	// Set username
	string setUserName(string u) { return userName = u; }

	// Set outcome
	bool setOutcome(bool o) { return outcome = o; }

	// Set number of correct answers
	int setGuesses(int g) { return guesses = g; }

	// Set date
	string setDate(string d) { return date = d; }

	// Boost serialisation
	friend class serialization::access;
	template<class Archive>
	void serialize(Archive& arch, const unsigned int version) {
		arch& userName;
		arch& outcome;
		arch& guesses;
		arch& date;
	}
};