// The header file for the server code. This was created to enable unit testing
#pragma once
#include <iostream>
#include <fstream>
#include "UserClass.h"
#include "QuestionClass.h"
#include "LeaderboardHangman.h"
#include "LeaderboardQuiz.h"
#include "GameManagement.h"
// Header files for Boost Serialisation
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <random>
#include <ctime>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost;

namespace server {
	// Titles
	string displayTitleText(string message) {
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 13);
		// End of adapted code
		std::cout << endl;
		std::cout << message << endl;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		// End of adapted code
		return message;
	}

	// Information
	string displayInfoText(string message) {
		string returnString = "INFO: " + message;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		// End of adapted code
		std::cout << returnString << endl;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		// End of adapted code
		return returnString;
	}

	// Positive text
	string displayPositiveText(string message) {
		string returnString = "SUCCESS: " + message;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		// End of adapted code
		std::cout << returnString << endl;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		// End of adapted code
		return returnString;
	}

	// Warnings
	string displayWarningText(string message) {
		string returnString = "WARNING: " + message;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		// End of adapted code
		std::cout << returnString << endl;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		// End of adapted code
		return returnString;
	}

	// Errors
	string displayErrorText(string message) {
		string returnString = "ERROR: " + message;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		// End of adapted code
		std::cout << returnString << endl;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		// End of adapted code
		return returnString;
	}

	void exitApp() {
		// Exit the application
		std::cout << endl << "Client and server have exited successfully. You may now close this window" << endl;
		exit(2);
	}

	GameManage admin(GameManage gameSettings, bool test, int testInput) {
		// Method for admin section
		displayTitleText("Admin");

		// Show the status of each game
		std::cout << "Hangman: ";
		if (gameSettings.getHangmanEnabled()) {
			displayPositiveText(" Enabled");
		}
		else {
			displayErrorText(" Disabled");
		}

		std::cout << "Mini Quiz: ";
		if (gameSettings.getMiniQuizEnabled()) {
			displayPositiveText(" Enabled");
		}
		else {
			displayErrorText(" Disabled");
		}

		asio::io_service ios;
		int userInput;
		try {
			if (!test) {
				tcp::endpoint endpoint(tcp::v4(), 7006);
				tcp::acceptor acceptor(ios, endpoint);
				tcp::iostream stream;
				acceptor.accept(*stream.rdbuf());
				stream >> userInput;
				stream.close();
			}
			else {
				userInput = testInput;
			}

			switch (userInput) {
			case 0:
				return gameSettings;
			case 1:
				if (gameSettings.getHangmanEnabled()) {
					gameSettings.setHangmanEnabled(false);
					displayInfoText("Hangman is no longer available to players");
				}
				else {
					gameSettings.setHangmanEnabled(true);
					displayInfoText("Hangman is now available to players");
				}
				break;
			case 2:
				if (gameSettings.getMiniQuizEnabled()) {
					gameSettings.setMiniQuizEnabled(false);
					displayInfoText("Mini quiz is no longer available to players");
				}
				else {
					gameSettings.setMiniQuizEnabled(true);
					displayInfoText("Mini quiz is now available to players");
				}
			}

			// Update settings file
			if (!test) {
				ofstream exportFile("gameSettings.dat");
				archive::text_oarchive arch(exportFile);
				arch << gameSettings;
			}
		}
		catch (std::exception e) {
			displayErrorText("The input you have entered is not valid");
		}
		return gameSettings;
	}

	list<User> deleteData(list<User> users, User currentUser, list<LeaderboardHangmanItem> leaderboardHangman, list<LeaderboardQuizItem> leaderboardQuiz, bool test, int testInput) {
		// Method for deleting user account
		displayTitleText("Delete User Data");
		displayInfoText("Waiting for user confirmation before deleting data...");
		list<LeaderboardHangmanItem> updatedHangmanLeaderboard;
		list<LeaderboardQuizItem> updatedQuizLeaderboard;
		list<User> updatedUsers;

		// Get input from client
		asio::io_service ios;
		int userInput;
		try {
			if (!test) {
				tcp::endpoint endpoint(tcp::v4(), 7007);
				tcp::acceptor acceptor(ios, endpoint);
				tcp::iostream stream;
				acceptor.accept(*stream.rdbuf());
				stream >> userInput;
				stream.close();
			}
			else {
				userInput = testInput;
			}

			if (userInput == 1) {
				// Remove all leaderboard records for the user. Such records cannot be removed directly
				for (LeaderboardHangmanItem h : leaderboardHangman) {
					std::cout << "Removing hangman records..." << endl;
					if (h.getUserName() != currentUser.getUserName()) {
						updatedHangmanLeaderboard.push_back(h);
					}
				}

				for (LeaderboardQuizItem q : leaderboardQuiz) {
					std::cout << "Removing mini quiz records..." << endl;
					if (q.getUserName() != currentUser.getUserName()) {
						updatedQuizLeaderboard.push_back(q);
					}
				}

				for (User u : users) {
					std::cout << "Removing user..." << endl;
					if (u.getUserName() != currentUser.getUserName()) {
						updatedUsers.push_back(u);
					}
				}

				std::cout << "Deletion complete. Thank you for playing!" << endl;

				// Update Boost serialisation files
				if (!test) {
					ofstream exportUserFile("userList.dat");
					archive::text_oarchive userArch(exportUserFile);
					userArch << updatedUsers;

					ofstream exportHangmanFile("leaderHangman.dat");
					archive::text_oarchive archHangman(exportHangmanFile);
					archHangman << updatedHangmanLeaderboard;

					ofstream exportQuizFile("leaderQuiz.dat");
					archive::text_oarchive archQuiz(exportQuizFile);
					archQuiz << updatedQuizLeaderboard;

					ios.stop();
					exitApp();
				}
				else {
					return updatedUsers;
				}
			}
			else {
				displayInfoText("You account has not been deleted");
				return users;
			}
		}
		catch (std::exception e) {
			displayErrorText(e.what());
		}
		ios.stop();
		return users;
	}

	vector<QuizQuestion> getQuizQuestions() {
		// Get quiz questions
		// This was originally intended to be stored in a Boost serialisation file but this was not feasible
		// The list of questions declared here needs to be a vector so that individual items can be removed directly using an index. This is not possible in lists
		// Compilation errors appear when trying to store a vector in a Boost serialisation file
		QuizQuestion q1;
		q1.setQuestion("The Plaka is the oldest quarter of which city?");
		q1.setChoiceA("Athens");
		q1.setChoiceB("Prague");
		q1.setChoiceC("Rome");
		q1.setChoiceD("Vienna");
		q1.setCorrectAnswer(1);

		QuizQuestion q2;
		q2.setQuestion("What is an axolotl?");
		q2.setChoiceA("A nerve in the brain");
		q2.setChoiceB("A multi-axled vehicle");
		q2.setChoiceC("A type of mortice lock");
		q2.setChoiceD("A species of salamander");
		q2.setCorrectAnswer(4);

		QuizQuestion q3;
		q3.setQuestion("The Panama Canal was officially opened by which US president?");
		q3.setChoiceA("Calvin Coolidge");
		q3.setChoiceB("Herbert Hoover");
		q3.setChoiceC("Theodore Roosevelt");
		q3.setChoiceD("Woodrow Wilson");
		q3.setCorrectAnswer(4);

		QuizQuestion q4;
		q4.setQuestion("In which opera did Maria Callas make her last appearance at Covent Garden?");
		q4.setChoiceA("Carmen");
		q4.setChoiceB("Tosca");
		q4.setChoiceC("Madamme Butterfly");
		q4.setChoiceD("La Boheme");
		q4.setCorrectAnswer(2);

		QuizQuestion q5;
		q5.setQuestion("After Adam, Eve, Cain and Abel, who is the next person mentioned in the Bible?");
		q5.setChoiceA("Enoch");
		q5.setChoiceB("Jubal");
		q5.setChoiceC("Lamech");
		q5.setChoiceD("Zillah");
		q5.setCorrectAnswer(1);

		QuizQuestion q6;
		q6.setQuestion("What is a kudzu?");
		q6.setChoiceA("Antelope");
		q6.setChoiceB("Bird");
		q6.setChoiceC("Jewish settlement");
		q6.setChoiceD("Climbing paint");
		q6.setCorrectAnswer(1);

		QuizQuestion q7;
		q7.setQuestion("Outlawed from 1603 to 1774, which Scottish clan was known as the Faceless Clan?");
		q7.setChoiceA("Campbell");
		q7.setChoiceB("MacGregor");
		q7.setChoiceC("MacLeod");
		q7.setChoiceD("MacDonald");
		q7.setCorrectAnswer(2);

		QuizQuestion q8;
		q8.setQuestion("From which country does tennis player Andres Gomez, winner of the 1990 French Championships, come?");
		q8.setChoiceA("Ecuador");
		q8.setChoiceB("Peru");
		q8.setChoiceC("Portugal");
		q8.setChoiceD("Spain");
		q8.setCorrectAnswer(1);

		QuizQuestion q9;
		q9.setQuestion("Which sign of the zodiac comes between Leo and Libra");
		q9.setChoiceA("Virgo");
		q9.setChoiceB("Taurus");
		q9.setChoiceC("Aries");
		q9.setChoiceD("Capricorn");
		q9.setCorrectAnswer(1);

		QuizQuestion q10;
		q10.setQuestion("Which tennis player, famous for her two-handed backhand and baseline-dominsted play was nicknamed The Ice Maiden?");
		q10.setChoiceA("Steffi Graf");
		q10.setChoiceB("Chris Evert");
		q10.setChoiceC("Evonne Cawley");
		q10.setChoiceD("Kim Clijsters");
		q10.setCorrectAnswer(2);

		QuizQuestion q11;
		q11.setQuestion("What type of cheese is Stilton?");
		q11.setChoiceA("Blue");
		q11.setChoiceB("Green");
		q11.setChoiceC("Hard");
		q11.setChoiceD("Mouldy");
		q11.setCorrectAnswer(1);

		QuizQuestion q12;
		q12.setQuestion("Which British monarch succeeded Queen Victoria?");
		q12.setChoiceA("Elizabeth II");
		q12.setChoiceB("Edward VI");
		q12.setChoiceC("Edward VII");
		q12.setChoiceD("George VII");
		q12.setCorrectAnswer(3);

		// Add questions to a list
		vector<QuizQuestion> questions;
		questions.push_back(q1);
		questions.push_back(q2);
		questions.push_back(q3);
		questions.push_back(q4);
		questions.push_back(q5);
		questions.push_back(q6);
		questions.push_back(q7);
		questions.push_back(q8);
		questions.push_back(q9);
		questions.push_back(q10);
		questions.push_back(q11);
		questions.push_back(q12);

		return questions;
	}

	int displayQuizScore(int score) {
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
		// End of adapted code
		switch (score) {
		case 0:
			std::cout << "|-------|      |    |-------|" << endl;
			std::cout << "|       |      |           | " << endl;
			std::cout << "|       |     |           |  " << endl;
			std::cout << "|       |     |          |   " << endl;
			std::cout << "|       |     |          |   " << endl;
			std::cout << "|       |    |          |    " << endl;
			std::cout << "|-------|    |          |    " << endl;
			break;
		case 1:
			std::cout << "        |      |    |-------|" << endl;
			std::cout << "        |      |           | " << endl;
			std::cout << "        |     |           |  " << endl;
			std::cout << "        |     |          |   " << endl;
			std::cout << "        |     |          |   " << endl;
			std::cout << "        |    |          |    " << endl;
			std::cout << "        |    |          |    " << endl;
			break;
		case 2:
			std::cout << "--------|      |    |-------|" << endl;
			std::cout << "        |      |           | " << endl;
			std::cout << "        |     |           |  " << endl;
			std::cout << "|-------|     |          |   " << endl;
			std::cout << "|             |          |   " << endl;
			std::cout << "|            |          |    " << endl;
			std::cout << "|--------    |          |    " << endl;
			break;
		case 3:
			std::cout << "--------|      |    |-------|" << endl;
			std::cout << "        |      |           | " << endl;
			std::cout << "        |     |           |  " << endl;
			std::cout << "--------|     |          |   " << endl;
			std::cout << "        |     |          |   " << endl;
			std::cout << "        |    |          |    " << endl;
			std::cout << "--------|    |          |    " << endl;
			break;
		case 4:
			std::cout << "|       |      |    |-------|" << endl;
			std::cout << "|       |      |           | " << endl;
			std::cout << "|       |     |           |  " << endl;
			std::cout << "|-------|     |          |   " << endl;
			std::cout << "        |     |          |   " << endl;
			std::cout << "        |    |          |    " << endl;
			std::cout << "        |    |          |    " << endl;
			break;
		case 5:
			std::cout << "|--------      |    |-------|" << endl;
			std::cout << "|              |           | " << endl;
			std::cout << "|             |           |  " << endl;
			std::cout << "|-------|     |          |   " << endl;
			std::cout << "        |     |          |   " << endl;
			std::cout << "        |    |          |    " << endl;
			std::cout << "--------|    |          |    " << endl;
			break;
		case 6:
			std::cout << "|              |    |-------|" << endl;
			std::cout << "|              |           | " << endl;
			std::cout << "|             |           |  " << endl;
			std::cout << "|-------|     |          |   " << endl;
			std::cout << "|       |     |          |   " << endl;
			std::cout << "|       |    |          |    " << endl;
			std::cout << "|-------|    |          |    " << endl;
			break;
		case 7:
			std::cout << "|-------|      |    |-------|" << endl;
			std::cout << "       |       |           | " << endl;
			std::cout << "      |       |           |  " << endl;
			std::cout << "     |        |          |   " << endl;
			std::cout << "     |        |          |   " << endl;
			std::cout << "    |        |          |    " << endl;
			std::cout << "    |        |          |    " << endl;
			break;
		}
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		// End of adapted code
		return 0;
	}

	list<LeaderboardQuizItem> playMiniQuiz(User currentUser, list<LeaderboardQuizItem> leaderboardQuiz, bool test, int testInput) {
		// Method for mini quiz (server)
		displayTitleText("Mini Quiz");
		std::cout << endl << "In this game, you will be presented with seven multiple-choice questions. Can you answer them correctly?" << endl;

		// Get the questions
		vector<QuizQuestion> questionBank = getQuizQuestions();
		vector<QuizQuestion> questionsToAsk;

		// Select 7 random questions to the user
		int questionsAsked = 0;
		bool iterate = true;
		for (int i = 0; i < 7; i++) {
			// Choose a random word. Adapted from StackOverflow (Cubbi, 2011)
			int highestIndex = questionBank.size() - 1;
			random_device device;
			mt19937 gen(device());
			uniform_int_distribution<> distribution(0, highestIndex);
			int randomIndex = distribution(gen);
			// End of adapted code
			questionsToAsk.push_back(questionBank[randomIndex]);
			// Use an iterator to remove a specific element from the question bank. Adapted from GeeksForGeeks (2023)
			// This ensures that none of the questions can appear twice in one session
			vector<QuizQuestion>::iterator it = questionBank.begin() + randomIndex;
			questionBank.erase(it);
			// End of adapted code
		}

		// Present each question to the user
		int points = 0;
		for (QuizQuestion q : questionsToAsk) {
			std::cout << endl;
			std::cout << q.getQuestion() << endl;
			std::cout << "1: " << q.getChoiceA() << endl;
			std::cout << "2: " << q.getChoiceB() << endl;
			std::cout << "3: " << q.getChoiceC() << endl;
			std::cout << "4: " << q.getChoiceD() << endl;
			int correctAnswer = q.getCorrectAnswer();

			// Get answer from user
			int userAnswer = 0;
			try {
				if (!test) {
					asio::io_service ios;
					tcp::endpoint endpoint(tcp::v4(), 7006);
					tcp::acceptor acceptor(ios, endpoint);
					tcp::iostream stream;
					acceptor.accept(*stream.rdbuf());
					stream >> userAnswer;
					stream.close();
					ios.stop();
				}
				else {
					userAnswer = testInput;
				}

				// If -1 is entered, exit the game
				if (userAnswer == -1) {
					displayWarningText("You have exited from the game");
					return leaderboardQuiz;
				}

				if (userAnswer == correctAnswer) {
					// If user answered correctly, add a point
					displayPositiveText("Correct!");
					points++;
				}
				else if (userAnswer == 0) {
					cout << "You have passed this question. The correct answer was: " << correctAnswer << endl;
				}
				else {
					displayErrorText("Incorrect!");
					std::cout << "The correct answer was: ";
					// Adapted from CPlusPlus.com (Spot3, 2013)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
					// End of adapted code
					std::cout << correctAnswer << endl;
					// Adapted from CPlusPlus.com (Spot3, 2013)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					// End of adapted code
				}
			}
			catch (std::exception e) {
				displayErrorText(e.what());
			}
		}

		cout << "\nGame over! Your score is: " << endl;
		displayQuizScore(points);

		// Add record to leaderboard
		LeaderboardQuizItem record;
		record.setUserName(currentUser.getUserName());
		record.setCorrectAnswers(points);

		// Get current date and time. Adapted from StackOverflow (Black, 2017)
		chrono::system_clock::time_point date = chrono::system_clock::now();
		time_t t = chrono::system_clock::to_time_t(date);
		// End of adapted code
		char dateString[26];
		ctime_s(dateString, sizeof dateString, &t);
		record.setDate(dateString);

		// Save record to leaderboard
		leaderboardQuiz.push_back(record);
		ofstream exportFile("leaderQuiz.dat");
		archive::text_oarchive arch(exportFile);
		arch << leaderboardQuiz;
		displayInfoText("Your game record has been added to leaderboard");
		return leaderboardQuiz;
	}

	vector<string> setupHangmanWords() {
		vector<string> words;

		// Add a selection of words into a list
		words.push_back("boost");
		words.push_back("server");
		words.push_back("client");
		words.push_back("window");
		words.push_back("apple");
		words.push_back("mountain");
		words.push_back("television");
		words.push_back("lawyer");
		words.push_back("vehicle");
		words.push_back("grapefruit");
		words.push_back("darts");
		words.push_back("football");
		words.push_back("witchcraft");
		words.push_back("atlantic");
		words.push_back("pencil");
		words.push_back("voltage");
		words.push_back("literature");
		words.push_back("comedian");
		return words;
	}

	int constructHangman(int lives) {
		// Build the hangman graphic depending on the number of lives the player has
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
		// End of adapted code
		switch (lives) {
		case 5:
			std::cout << "  |---------- " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "-----         " << endl;
			break;
		case 4:
			std::cout << "  |---------- " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |      |   |" << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "-----         " << endl;
			break;
		case 3:
			std::cout << "  |---------- " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |      |   |" << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "-----         " << endl;
			break;
		case 2:
			std::cout << "  |---------- " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |      |   |" << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "  |           " << endl;
			std::cout << "-----         " << endl;
			break;
		case 1:
			std::cout << "  |---------- " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |      |   |" << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			// Add backslash using "\\". Adapted from W3Schools (2023)
			std::cout << "  |       / \\ " << endl;
			std::cout << "  |      /   \\" << endl;
			// End of adapted code
			std::cout << "  |           " << endl;
			std::cout << "-----         " << endl;
			break;
		case 0:
			std::cout << "  |---------- " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |        |  " << endl;
			std::cout << "  |      |---|" << endl;
			std::cout << "  |      |   |" << endl;
			std::cout << "  |      |---|" << endl;
			// Add backslash using "\\". Adapted from W3Schools (2023)
			std::cout << "  |       /|\\ " << endl;
			std::cout << "  |      / | \\" << endl;
			std::cout << "  |       / \\ " << endl;
			std::cout << "  |      /   \\" << endl;
			// End of adapted code
			std::cout << "  |           " << endl;
			std::cout << "-----         " << endl;
			break;
		default:
			throw "Unusual parameter";
		}
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		// End of adapted code
	}

	list<LeaderboardHangmanItem> playHangman(User currentUser, list<LeaderboardHangmanItem> leaderboardHangman, bool test, char testInput, string testWord) {
		// Method for hangman (server)
		displayTitleText("Hangman");
		std::cout << endl << "In this game, you must correctly guess a word by entering one letter at a time. Incorrect guesses will add a body part onto the hangman graphic. Can you guess the word before the hangman is fully created?" << endl;
		string chosenWord = "";

		if (!test) {
			vector<string> words = setupHangmanWords();

			// Choose a random word. Adapted from StackOverflow (Cubbi, 2011)
			int highestIndex = words.size() - 1;
			random_device device;
			mt19937 gen(device());
			uniform_int_distribution<> distribution(0, highestIndex);
			int randomIndex = distribution(gen);
			// End of adapted code
			chosenWord = words[randomIndex];
		}
		else {
			chosenWord = testWord;
		}

		int lives = 5;
		vector<char> correctLetters;
		vector<char> foundLetters;
		for (char& character : chosenWord) {
			correctLetters.push_back(character);
			foundLetters.push_back('_');
		}

		// Send outcome to client so that it can return to the main menu once the game ends
		bool iterate = true;
		bool finishedGame = false;
		LeaderboardHangmanItem record;
		int guesses = 0;

		while (iterate) {
			constructHangman(lives);

			// Output the word as follows:
			// For each letter that has been found by the player, output that letter
			// Otherwise, output an underscore (_)
			// For example, if mountain was chosen as the word and the letters A, O and U have been found, output _ O U _ _ A _ _
			// Adapted from StackOverflow (R. Martinho Fernandes, 2018)
			std::cout << "Number of letters: " << foundLetters.size() << endl;
			std::cout << "Word: ";
			for (char& character : foundLetters) {
				if (character == '_') {
					// Adapted from CPlusPlus.com (Spot3, 2013)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					// End of adapted code
				}
				else {
					// Adapted from CPlusPlus.com (Spot3, 2013)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
					// End of adapted code
				}
				std::cout << character << " ";
			}
			std::cout << endl;
			char userInput = '0';

			if (!test) {
				tcp::iostream outcomeStream("127.0.1.1", "7007");
				if (finishedGame) {
					outcomeStream << 1;
					outcomeStream.close();
					break;
				}
				else {
					outcomeStream << 0;
					outcomeStream.close();
				}

				// Get input from client
				try {
					asio::io_service ios;
					tcp::endpoint endpoint(tcp::v4(), 7006);
					tcp::acceptor acceptor(ios, endpoint);
					tcp::iostream stream;
					acceptor.accept(*stream.rdbuf());
					stream >> userInput;
					stream.close();
					ios.stop();
				}
				catch (std::exception e) {
					displayErrorText(e.what());
				}
			}
			else {
				userInput = testInput;
			}

			// Exit if user enters forward slash
			if (userInput == '/') {
				displayWarningText("You have exited from the game");
				return leaderboardHangman;
			}

			// Check if the user input matches any of the letters of the word and update accordingly
			bool changesMade = false;
			for (int i = 0; i < correctLetters.size(); i++) {
				if (correctLetters[i] == userInput) {
					foundLetters[i] = userInput;
					changesMade = true;
				}
			}

			// If no changes have been made, the user loses a life and a new part is drawn on the hangman graphic
			if (!changesMade) {
				std::cout << endl;
				displayErrorText("Incorrect!");
				lives--;
			}
			else {
				std::cout << endl;
				displayPositiveText("Correct!");
			}

			// If no lives are left, mark the game as a defeat for the player
			if (lives == 0) {
				displayErrorText("Game over!");
				std::cout << "The word was: ";
				// Adapted from CPlusPlus.com (Spot3, 2013)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				// End of adapted code
				std::cout << chosenWord << endl;
				// Adapted from CPlusPlus.com (Spot3, 2013)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				// End of adapted code
				record.setOutcome(false);
				finishedGame = true;
			}

			// If the player has found all words, mark the game as a win for the player
			if (correctLetters == foundLetters) {
				displayPositiveText("You won!");
				record.setOutcome(true);
				finishedGame = true;
			}

			if (test) {
				break;
			}

			guesses++;
		}

		// Add record to leaderboard
		record.setUserName(currentUser.getUserName());
		record.setGuesses(guesses);

		// Get current date and time. Adapted from StackOverflow (Black, 2017)
		chrono::system_clock::time_point date = chrono::system_clock::now();
		time_t t = chrono::system_clock::to_time_t(date);
		// End of adapted code
		char dateString[26];
		ctime_s(dateString, sizeof dateString, &t);
		record.setDate(dateString);

		// Save record to leaderboard
		leaderboardHangman.push_back(record);

		if (!test) {
			ofstream exportFile("leaderHangman.dat");
			archive::text_oarchive arch(exportFile);
			arch << leaderboardHangman;
		}

		displayInfoText("Your game record has been added to leaderboard");
		return leaderboardHangman;
	}

	int viewLeaderBoard(list<LeaderboardHangmanItem> leaderboardHangman, list<LeaderboardQuizItem> leaderboardQuiz) {
		// Method for leaderboard (server)
		displayTitleText("Leaderboards");
		int userChoice;
		try {
			asio::io_service ios;
			tcp::endpoint endpoint(tcp::v4(), 7006);
			tcp::acceptor acceptor(ios, endpoint);
			tcp::iostream stream;
			displayInfoText("Waiting for client to select a leaderboard to view");
			acceptor.accept(*stream.rdbuf());
			stream >> userChoice;
			stream.close();
			ios.stop();

			switch (userChoice) {
			case 0:
				std::cout << "\nLeaderboard not selected. Returning to main menu..." << endl;
				return 0;
				break;
			case 1:
				std::cout << "\nHangman Leaderboard" << endl;
				std::cout << "Username, Victory?, Guesses, Date" << endl;
				std::cout << "---------------------------------" << endl;
				if (leaderboardHangman.size() > 0) {
					for (LeaderboardHangmanItem h : leaderboardHangman) {
						bool outcome = h.getOutcome();
						string outcomeText;
						if (outcome) {
							outcomeText = "Win";
						}
						else {
							outcomeText = "Loss";
						}
						std::cout << h.getUserName() << ", ";
						std::cout << outcomeText << ", ";
						std::cout << h.getGuesses() << ", ";
						std::cout << h.getDate() << endl;
					}
				}
				else {
					std::cout << "The leaderboard for hangman is currently empty. You can add your own scores by playing this game!" << endl;
				}
				break;
			case 2:
				std::cout << "\nMini Quiz Leaderboard" << endl;
				std::cout << "Username, Correct Answers, Date" << endl;
				std::cout << "-------------------------------" << endl;
				if (leaderboardQuiz.size() > 0) {
					for (LeaderboardQuizItem h : leaderboardQuiz) {
						std::cout << h.getUserName() << ", ";
						std::cout << h.getCorrectAnswers() << ", ";
						std::cout << h.getDate() << endl;
					}
				}
				else {
					std::cout << "The leaderboard for the mini quiz is currently empty. You can add your own scores by playing this game!" << endl;
				}
				break;
			default:
				std::cout << "The input you have entered is not valid" << endl;
				break;
			}
		}
		catch (std::exception e) {
			displayErrorText(e.what());
		}
		return 0;
	}

	int menu(list<User> users, User currentUser) {
		// Get leaderboards
		list<LeaderboardQuizItem> leaderboardQuiz;
		ifstream importQuizFile("leaderQuiz.dat");
		archive::text_iarchive archInQuiz(importQuizFile);
		archInQuiz >> leaderboardQuiz;

		list<LeaderboardHangmanItem> leaderboardHangman;
		ifstream importHangmanFile("leaderHangman.dat");
		archive::text_iarchive archInHangman(importHangmanFile);
		archInHangman >> leaderboardHangman;

		GameManage gameSettings;
		ifstream importSettingsFile("gameSettings.dat");
		archive::text_iarchive archInGameSettings(importSettingsFile);
		archInGameSettings >> gameSettings;

		bool iterate = true;
		while (iterate) {
			displayTitleText("Main Menu");
			displayInfoText("Waiting for client to choose a menu option...");
			try {
				asio::io_service ios;
				tcp::endpoint endpoint(tcp::v4(), 7005);
				tcp::acceptor acceptor(ios, endpoint);
				tcp::iostream stream;
				acceptor.accept(*stream.rdbuf());
				int recv;
				stream >> recv;
				stream.close();

				tcp::iostream hangmanEnableStream("127.0.1.2", "7008");
				tcp::iostream miniQuizEnableStream("127.0.1.2", "7009");

				switch (recv) {
				case 0:
					exitApp();
				case 1:
					viewLeaderBoard(leaderboardHangman, leaderboardQuiz);
					break;
				case 2:
					// Send status of hangman availability to client
					std::cout << "Sending hangman status..." << endl;
					hangmanEnableStream << gameSettings.getHangmanEnabled();
					displayPositiveText("Sent");
					hangmanEnableStream.close();
					if (gameSettings.getHangmanEnabled()) {
						leaderboardHangman = playHangman(currentUser, leaderboardHangman, false, '0', "noTest");
					}
					break;
				case 3:
					// Send status of mini quiz availability to client
					std::cout << "Sending mini quiz status..." << endl;
					miniQuizEnableStream << gameSettings.getMiniQuizEnabled();
					displayPositiveText("Sent");
					miniQuizEnableStream.close();
					if (gameSettings.getMiniQuizEnabled()) {
						leaderboardQuiz = playMiniQuiz(currentUser, leaderboardQuiz, false, 0);
					}
					break;
				case 4:
					deleteData(users, currentUser, leaderboardHangman, leaderboardQuiz, false, 0);
					break;
				case 5:
					gameSettings = admin(gameSettings, false, 0);
					break;
				default:
					std::cout << "Unexpected option" << endl;
				}
				ios.stop();
			}
			catch (std::exception e) {
				displayErrorText(e.what());
			}
		}
		return 0;
	}

	int login(list<User> users, bool test, string testName, size_t testPass) {
		displayTitleText("Login");
		bool iterate = true;
		string userName;
		size_t hashedPassword;
		User loginUser;

		while (iterate) {
			// Get username and password from client
			asio::io_service ios;
			try {
				if (!test) {
					displayInfoText("Getting username from the client...");
					tcp::endpoint userNameEndpoint(tcp::v4(), 7001);
					tcp::acceptor userNameAcceptor(ios, userNameEndpoint);
					tcp::iostream userNameStream;
					userNameAcceptor.accept(*userNameStream.rdbuf());
					userNameStream >> userName;
					userNameStream.close();

					// Cancel if user entered -1
					if (userName == "-1") {
						displayWarningText("You have cancelled the login process");
						return 0;
					}

					displayInfoText("Getting password from the client...");
					tcp::endpoint passwordEndpoint(tcp::v4(), 7002);
					tcp::acceptor passwordAcceptor(ios, passwordEndpoint);
					tcp::iostream passwordStream;
					passwordAcceptor.accept(*passwordStream.rdbuf());
					passwordStream >> hashedPassword;
					passwordStream.close();
				}
				else {
					userName = testName;
					hashedPassword = testPass;
				}
			}
			catch (std::exception e) {
				displayErrorText(e.what());
			}

			// Adapted from cppreference.com (2023)
			size_t hashedCancelString = std::hash<string>{}("-1");
			// End of adapted code

			// Cancel if user entered -1
			if (hashedPassword == hashedCancelString) {
				displayWarningText("You have cancelled the login process");
				return 0;
			}

			// Send response to client (user type if username and password match)
			tcp::iostream respondStream("127.0.1.2", "7004");
			bool gotCorrectUsername = false;
			bool gotCorrectPassword = false;
			int type;
			for (User u : users) {
				if (u.getUserName() == userName) {
					gotCorrectUsername = true;
					size_t checkPassword = u.getPasswordHash();
					if (hashedPassword == checkPassword) {
						gotCorrectPassword = true;
						type = u.getUserType();
						loginUser = u;
					}
					break;
				}
			}
			if (gotCorrectUsername && gotCorrectPassword) {
				displayPositiveText("Authentication passed. Sending response signal to client...");
				if (!test) {
					respondStream << type;
				}
				else {
					return 1;
				}
				break;
			}
			else {
				displayErrorText("Authentication failed. Sending response signal to client...");
				if (!test) {
					respondStream << -1;
				}
				else {
					return 0;
				}
			}
			respondStream.close();
			ios.stop();
		}

		menu(users, loginUser);
		return 0;
	}

	list<User> createAccount(list<User> users, bool test, string testName, size_t testPass, size_t testPassAgain) {
		displayTitleText("Create an Account");
		bool iterate = true;
		string userName;
		size_t hashedPassword;
		size_t hashedPasswordAgain;
		bool userNameTaken = false;

		while (iterate) {
			// Get username and password from client
			asio::io_service ios;
			try {
				if (!test) {
					displayInfoText("Getting username from the client...");
					tcp::endpoint userNameEndpoint(tcp::v4(), 7001);
					tcp::acceptor userNameAcceptor(ios, userNameEndpoint);
					tcp::iostream userNameStream;
					userNameAcceptor.accept(*userNameStream.rdbuf());
					userNameStream >> userName;
					userNameStream.close();
				}
				else {
					userName = testName;
				}

				// Determine if username has been taken
				for (User u : users) {
					if (userName == u.getUserName()) {
						userNameTaken = true;
					}
				}

				// Cancel if user entered -1
				if (userName == "-1") {
					displayWarningText("You have cancelled the account creation process");
					return users;
				}

				if (!test) {
					displayInfoText("Getting password from the client...");
					tcp::endpoint passwordEndpoint(tcp::v4(), 7002);
					tcp::acceptor passwordAcceptor(ios, passwordEndpoint);
					tcp::iostream passwordStream;
					passwordAcceptor.accept(*passwordStream.rdbuf());
					passwordStream >> hashedPassword;
					passwordStream.close();

					displayInfoText("Getting password once more from the client...");
					tcp::endpoint passwordAgainEndpoint(tcp::v4(), 7003);
					tcp::acceptor passwordAgainAcceptor(ios, passwordAgainEndpoint);
					tcp::iostream passwordAgainStream;
					passwordAgainAcceptor.accept(*passwordAgainStream.rdbuf());
					passwordAgainStream >> hashedPasswordAgain;
					passwordAgainStream.close();
				}
				else {
					hashedPassword = testPass;
					hashedPasswordAgain = testPassAgain;
				}
			}
			catch (std::exception e) {
				displayErrorText(e.what());
			}

			// Adapted from cppreference.com (2023)
			size_t hashedCancelString = std::hash<string>{}("-1");
			// End of adapted code

			// Cancel if user entered -1
			if (hashedPassword == hashedCancelString || hashedPasswordAgain == hashedCancelString) {
				displayWarningText("You have cancelled the account creation process");
				return users;
			}

			// Send response to client (1 if passwords match; 0 otherwise)
			tcp::iostream respondStream("127.0.1.2", "7004");
			if (hashedPassword == hashedPasswordAgain && !userNameTaken) {
				if (!test) {
					displayPositiveText("Account created. Sending response signal to client...");
					respondStream << 1;
					break;
				}
				else {
					break;
				}
			}
			else {
				if (!test) {
					displayErrorText("Account could not be created. Sending response signal to client...");
					respondStream << 0;
				}
				else {
					return users;
				}
			}

			respondStream.close();
			ios.stop();
		}

		User newUser;
		newUser.setUserName(userName);
		newUser.setPassword(hashedPassword);
		newUser.setUserType(0);

		// Add user to file
		users.push_back(newUser);

		if (!test) {
			ofstream exportFile("userList.dat");
			archive::text_oarchive arch(exportFile);
			arch << users;
			menu(users, newUser);
		}
		return users;
	}

	list<User> setup(list<User> users) {
		// Method for setting up the users file with an admin account
		User admin;
		admin.setUserName("admin");
		std::cout << "Set a password for admin" << endl;
		string password;
		cin >> password;
		// Adapted from cppreference.com (2023)
		size_t hashedPassword = std::hash<string>{}(password);
		// End of adapted code
		admin.setPassword(hashedPassword);
		admin.setUserType(1);
		users.push_back(admin);

		ofstream exportFile("userList.dat");
		archive::text_oarchive arch(exportFile);
		arch << users;

		std::cout << users.size() << endl;
		for (User u : users) {
			std::cout << u.getUserName() << endl;
		}

		return users;
	}

	int setupHangmanLeaderboard() {
		// Provision a Boost serialisation file for the hangman leaderboard
		// This should only be run once
		list<LeaderboardHangmanItem> leaderboard;
		LeaderboardHangmanItem sample;
		sample.setUserName("SAMPLE");
		sample.setOutcome(false);
		sample.setGuesses(5);
		sample.setDate("NULL");
		leaderboard.push_back(sample);

		ofstream exportFile("leaderHangman.dat");
		archive::text_oarchive arch(exportFile);
		arch << leaderboard;
		std::cout << "Leaderboard created for hangman" << endl;
		return 0;
	}

	int setupQuizLeaderboard() {
		// Provision a Boost serialisation file for the hangman quiz
		// This should only be run once
		list<LeaderboardQuizItem> leaderboard;
		LeaderboardQuizItem sample;
		sample.setUserName("SAMPLE");
		sample.setCorrectAnswers(4);
		sample.setDate("NULL");
		leaderboard.push_back(sample);

		ofstream exportFile("leaderQuiz.dat");
		archive::text_oarchive arch(exportFile);
		arch << leaderboard;
		std::cout << "Leaderboard created for quiz" << endl;
		return 0;
	}

	int setupGameManagement() {
		// Provision a Boost serialisation file for allowing or preventing users from accessing games
		// This should only be run once
		GameManage gameSettings;
		gameSettings.setHangmanEnabled(true);
		gameSettings.setMiniQuizEnabled(true);

		ofstream exportFile("gameSettings.dat");
		archive::text_oarchive arch(exportFile);
		arch << gameSettings;
		std::cout << "Game settings file created" << endl;
		return 0;
	}
}