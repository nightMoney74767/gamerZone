#pragma once
#include <iostream>
// Header files for Boost Serialization
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;
using namespace boost;

// Class object representing quiz questions
class QuizQuestion {
private:
	// Question
	string question;

	// Possible answers
	string choiceA;
	string choiceB;
	string choiceC;
	string choiceD;

	// Correct answer
	int correctAnswer;

public:
	// Get question, possible answers and correct answer
	string getQuestion() { return question; }
	string getChoiceA() { return choiceA; }
	string getChoiceB() { return choiceB; }
	string getChoiceC() { return choiceC; }
	string getChoiceD() { return choiceD; }
	int getCorrectAnswer() { return correctAnswer; }

	// Set question, possible answers and correct answer
	string setQuestion(string q) { return question = q; }
	string setChoiceA(string c) { return choiceA = c; }
	string setChoiceB(string c) { return choiceB = c; }
	string setChoiceC(string c) { return choiceC = c; }
	string setChoiceD(string c) { return choiceD = c; }
	int setCorrectAnswer(int a) { return correctAnswer = a; }

	// Boost serialisation
	friend class serialization::access;
	template<class Archive>
	void serialize(Archive& arch, const unsigned int version) {
		arch& question;
		arch& choiceA;
		arch& choiceB;
		arch& choiceC;
		arch& choiceD;
		arch& correctAnswer;
	}
};