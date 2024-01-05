#pragma once
#include <iostream>
// Header files for Boost Serialization
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;
using namespace boost;

// Class object for enabling and disabling games
class GameManage {
private:
	// Hangman
	bool hangmanEnabled;

	// Mini quiz
	bool miniQuizEnabled;

public:
	bool getHangmanEnabled() { return hangmanEnabled; }

	bool getMiniQuizEnabled() { return miniQuizEnabled; }

	bool setHangmanEnabled(bool h) { return hangmanEnabled = h; }

	bool setMiniQuizEnabled(bool q) { return miniQuizEnabled = q; }

	// Boost serialisation
	friend class serialization::access;
	template<class Archive>
	void serialize(Archive& arch, const unsigned int version) {
		arch& hangmanEnabled;
		arch& miniQuizEnabled;
	}
};