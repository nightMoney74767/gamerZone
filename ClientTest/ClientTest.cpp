#include "pch.h"
// Test code for client
// The following directories were used for this project
// Additional Include: C:\Users\Nathan\Documents\Extracted\boost_1_80_0\boost_1_80_0
// Additional Library: C:\Users\Nathan\Documents\Extracted\boost_1_80_0\boost_1_80_0\stage\lib
// These may be changed in Visual Studio if required. Note that the Boost directories fro the project code and the test suites must be different
#include "pch.h"
#include "CppUnitTest.h"
#include "../Client/client.h"
using namespace client;

#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Header files for Boost Serialisation
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost;

namespace ClientTest
{
	TEST_CLASS(ClientTest)
	{
	public:
		// Titles
		TEST_METHOD(TestDisplayTitleText) {
			string result = client::displayTitleText("Test Title");
			string expected = "Test Title";
			Assert::AreEqual(expected, result);
		}

		// Input prompts
		TEST_METHOD(TestDisplayInputText) {
			string result = client::displayInputText("Enter a username and press enter:");
			string expected = "INPUT: Enter a username and press enter:";
			Assert::AreEqual(expected, result);

			result = client::displayInputText("Enter a password and press enter:");
			expected = "INPUT: Enter a password and press enter:";
			Assert::AreEqual(expected, result);
		}

		// Information
		TEST_METHOD(TestDisplayInfoText) {
			string result = client::displayInfoText("Word chosen for hangman:");
			string expected = "INFO: Word chosen for hangman:";
			Assert::AreEqual(expected, result);

			result = client::displayInfoText("Getting quiz questions...");
			expected = "INFO: Getting quiz questions...";
			Assert::AreEqual(expected, result);
		}

		// Positive text
		TEST_METHOD(TestDisplayPositiveText) {
			string result = client::displayPositiveText("Account created!");
			string expected = "SUCCESS: Account created!";
			Assert::AreEqual(expected, result);

			result = client::displayPositiveText("Hello World!");
			expected = "SUCCESS: Hello World!";
			Assert::AreEqual(expected, result);
		}

		// Warnings
		TEST_METHOD(TestDisplayWarningText) {
			string result = client::displayWarningText("Deleted account data cannot be recovered");
			string expected = "WARNING: Deleted account data cannot be recovered";
			Assert::AreEqual(expected, result);

			result = client::displayWarningText("Connection is taking longer than expected");
			expected = "WARNING: Connection is taking longer than expected";
			Assert::AreEqual(expected, result);
		}

		// Errors
		TEST_METHOD(TestDisplayErrorText) {
			string result = client::displayErrorText("Something went wrong. Please try again");
			string expected = "ERROR: Something went wrong. Please try again";
			Assert::AreEqual(expected, result);

			result = client::displayErrorText("Connection to the server failed. Please try again later");
			expected = "ERROR: Connection to the server failed. Please try again later";
			Assert::AreEqual(expected, result);
		}

		// Sending input
		TEST_METHOD(TestSendInput) {
			int result = sendInput(1);
			int expected = 1;
			Assert::AreEqual(expected, result);
		}

		// Admin
		TEST_METHOD(TestAdmin) {
			int result = admin(true, 1);
			int expected = 1;
			Assert::AreEqual(expected, result);

			result = admin(true, 2);
			expected = 1;
			Assert::AreEqual(expected, result);
		}

		// Deleting account data
		TEST_METHOD(TestDeleteData) {
			int result = deleteData("U", true, 1);
			int expected = 0;
			Assert::AreEqual(expected, result);

			result = deleteData("A", true, 0);
			expected = 1;
			Assert::AreEqual(expected, result);
		}

		// Playing mini quiz
		TEST_METHOD(TestPlayMiniQuiz) {
			int result = playMiniQuiz(true, 1);
			int expected = 0;
			Assert::AreEqual(expected, result);

			result = playMiniQuiz(true, 3);
			expected = 0;
			Assert::AreEqual(expected, result);
		}

		// Main menu
		TEST_METHOD(TestMenu) {
			int result = menu("theUnitTestingSuite", "U", true, 0);
			int expected = 0;
			Assert::AreEqual(expected, result);

			result = menu("theUnitTestingSuite", "U", true, 1);
			expected = 1;
			Assert::AreEqual(expected, result);

			result = menu("theUnitTestingSuite", "U", true, 2);
			expected = 2;
			Assert::AreEqual(expected, result);

			result = menu("theUnitTestingSuite", "U", true, 3);
			expected = 3;
			Assert::AreEqual(expected, result);

			result = menu("theUnitTestingSuite", "U", true, 4);
			expected = 4;
			Assert::AreEqual(expected, result);

			result = menu("theUnitTestingSuite", "U", true, 5);
			expected = -1;
			Assert::AreEqual(expected, result);

			result = menu("theUnitTestingSuite", "A", true, 5);
			expected = 5;
			Assert::AreEqual(expected, result);
		}
	};
}
