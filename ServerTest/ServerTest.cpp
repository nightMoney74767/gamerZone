// Test code for server
// The following directories were used for this project
// Additional Include: C:\Users\Nathan\Documents\Extracted\boost_1_80_0\boost_1_80_0
// Additional Library: C:\Users\Nathan\Documents\Extracted\boost_1_80_0\boost_1_80_0\stage\lib
// These may be changed in Visual Studio if required. Note that the Boost directories fro the project code and the test suites must be different
#include "pch.h"
#include "CppUnitTest.h"
#include "../Server/server.h"
using namespace server;

#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "../Server/UserClass.h"
#include "../Server/QuestionClass.h"
//#include "../Server/LeaderboardHangman.h"
//#include "../Server/LeaderboardQuiz.h"
#include "../Server/GameManagement.h"
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

namespace ServerTest
{
	TEST_CLASS(ServerTest)
	{
	public:
		// Titles
		TEST_METHOD(TestDisplayTitleText) {
			string result = server::displayTitleText("Test Title");
			string expected = "Test Title";
			Assert::AreEqual(expected, result);
		}
		
		// Information
		TEST_METHOD(TestDisplayInfoText) {
			string result = server::displayInfoText("Word chosen for hangman:");
			string expected = "INFO: Word chosen for hangman:";
			Assert::AreEqual(expected, result);

			result = server::displayInfoText("Getting quiz questions...");
			expected = "INFO: Getting quiz questions...";
			Assert::AreEqual(expected, result);
		}

		// Positive text
		TEST_METHOD(TestDisplayPositiveText) {
			string result = server::displayPositiveText("Account created!");
			string expected = "SUCCESS: Account created!";
			Assert::AreEqual(expected, result);

			result = server::displayPositiveText("Hello World!");
			expected = "SUCCESS: Hello World!";
			Assert::AreEqual(expected, result);
		}

		// Warnings
		TEST_METHOD(TestDisplayWarningText) {
			string result = server::displayWarningText("Deleted account data cannot be recovered");
			string expected = "WARNING: Deleted account data cannot be recovered";
			Assert::AreEqual(expected, result);

			result = server::displayWarningText("Connection is taking longer than expected");
			expected = "WARNING: Connection is taking longer than expected";
			Assert::AreEqual(expected, result);
		}

		// Errors
		TEST_METHOD(TestDisplayErrorText) {
			string result = server::displayErrorText("Something went wrong. Please try again");
			string expected = "ERROR: Something went wrong. Please try again";
			Assert::AreEqual(expected, result);

			result = server::displayErrorText("Connection to the server failed. Please try again later");
			expected = "ERROR: Connection to the server failed. Please try again later";
			Assert::AreEqual(expected, result);
		}

		// Admin
		TEST_METHOD(TestAdmin) {
			GameManage result;
			result.setHangmanEnabled(false);
			result.setMiniQuizEnabled(false);

			// Enable hangman
			result = admin(result, true, 1);
			Assert::AreEqual(true, result.getHangmanEnabled());
			Assert::AreEqual(false, result.getMiniQuizEnabled());

			// Disable hangman and enable mini quiz
			result = admin(result, true, 1);
			result = admin(result, true, 2);
			Assert::AreEqual(false, result.getHangmanEnabled());
			Assert::AreEqual(true, result.getMiniQuizEnabled());

			// Enable hangman, leaving both games available to the user
			result = admin(result, true, 1);
			Assert::AreEqual(true, result.getHangmanEnabled());
			Assert::AreEqual(true, result.getMiniQuizEnabled());

			// Disable both games
			result = admin(result, true, 1);
			result = admin(result, true, 2);
			Assert::AreEqual(false, result.getHangmanEnabled());
			Assert::AreEqual(false, result.getMiniQuizEnabled());

			// Enter an input that should make no changes
			result = admin(result, true, 0);
			Assert::AreEqual(false, result.getHangmanEnabled());
			Assert::AreEqual(false, result.getMiniQuizEnabled());
		}

		// Deleting user data
		TEST_METHOD(TestDeleteData) {
			// Add sample users
			User u1;
			u1.setUserName("userOne");

			User u2;
			u2.setUserName("userTwo");

			list<User> users;
			users.push_back(u1);
			users.push_back(u2);

			// Leaderboard
			LeaderboardHangmanItem item;
			item.setUserName(u1.getUserName());

			LeaderboardQuizItem quizItem;
			quizItem.setUserName(u1.getUserName());

			list<LeaderboardHangmanItem> leaderboardHangman;
			leaderboardHangman.push_back(item);

			list<LeaderboardQuizItem> leaderboardQuiz;
			leaderboardQuiz.push_back(quizItem);

			// With confirmation
			list<User> result = deleteData(users, u2, leaderboardHangman, leaderboardQuiz, true, 1);
			int count = 0;
			for (User u : result) {
				count++;
			}
			Assert::AreEqual(1, count);
		}

		// Playing mini quiz
		TEST_METHOD(TestPlayMiniQuiz) {
			User u1;
			u1.setUserName("userOne");

			LeaderboardQuizItem quizItem;
			quizItem.setUserName(u1.getUserName());

			list<LeaderboardQuizItem> leaderboardQuiz;
			leaderboardQuiz.push_back(quizItem);

			list<LeaderboardQuizItem> result = playMiniQuiz(u1, leaderboardQuiz, true, 1);
			int count = 0;
			for (LeaderboardQuizItem i : result) {
				count++;
			}
			Assert::AreEqual(2, count);
		}

		// Playing hangman
		TEST_METHOD(TestPlayHangman) {
			User u1;
			u1.setUserName("userOne");

			LeaderboardHangmanItem hangmanItem;
			hangmanItem.setUserName(u1.getUserName());

			list<LeaderboardHangmanItem> leaderboardHangman;
			leaderboardHangman.push_back(hangmanItem);

			// Winning game
			list<LeaderboardHangmanItem> result = playHangman(u1, leaderboardHangman, true, 'a', "aaa");
			int count = 0;
			for (LeaderboardHangmanItem i : result) {
				count++;
			}
			Assert::AreEqual(2, count);

			leaderboardHangman.clear();
			leaderboardHangman.push_back(hangmanItem);
			// A game that would go on forever if the same input is used
			result = playHangman(u1, leaderboardHangman, true, 'a', "ab");
			count = 0;
			for (LeaderboardHangmanItem i : result) {
				count++;
			}
			Assert::AreEqual(2, count);
		}

		// Login
		TEST_METHOD(TestLogin) {
			// Users
			User u1;
			u1.setUserName("userOne");
			string password = "1234";
			// Adapted from cppreference.com (2023)
			size_t hashedPasswordUser1 = std::hash<string>{}(password);
			// End of adapted code
			u1.setPassword(hashedPasswordUser1);
			u1.setUserType(0);

			User u2;
			u2.setUserName("userOne");
			string password2 = "5678";
			// Adapted from cppreference.com (2023)
			size_t hashedPasswordUser2 = std::hash<string>{}(password);
			// End of adapted code
			u2.setPassword(hashedPasswordUser2);
			u2.setUserType(0);

			list<User> users;
			users.push_back(u1);
			users.push_back(u2);

			// User 1; correct password
			int result = login(users, true, u1.getUserName(), hashedPasswordUser1);
			Assert::AreEqual(1, result);

			// User 1; wrong password
			result = login(users, true, u1.getUserName(), 0);
			Assert::AreEqual(0, result);
			
			// User 2; correct password
			result = login(users, true, u2.getUserName(), hashedPasswordUser2);
			Assert::AreEqual(1, result);

			// User 2; wrong password
			result = login(users, true, u2.getUserName(), 0);
			Assert::AreEqual(0, result);
		}

		// Account creation
		TEST_METHOD(TestCreateAccount) {
			list<User> users;
			string userName = "newUser";
			string password = "1234";
			// Adapted from cppreference.com (2023)
			size_t hashedPassword = std::hash<string>{}(password);
			// End of adapted code

			string otherPassword = "5678";
			size_t otherPasswordHash = std::hash<string>{}(otherPassword);

			User u1;
			u1.setUserName(userName);
			u1.setPassword(hashedPassword);
			u1.setUserType(0);

			int count = 0;

			// Matching password and username not taken
			users = createAccount(users, true, userName, hashedPassword, hashedPassword);
			for (User u : users) {
				count++;
			}
			Assert::AreEqual(1, count);
			count = 0;

			// Password mismatch
			users = createAccount(users, true, "passwordMismatched", hashedPassword, otherPasswordHash);
			for (User u : users) {
				count++;
			}
			Assert::AreEqual(1, count);
			count = 0;

			// Create an account with the same username that has been taken
			users = createAccount(users, true, userName, hashedPassword, hashedPassword);
			for (User u : users) {
				count++;
			}
			Assert::AreEqual(1, count);
		}
	};
}
