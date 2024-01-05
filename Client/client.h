// The header file for the client code. This was created to enable unit testing
#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include "UserClass.h"
// Header files for Boost Serialisation
#include <boost/serialization/list.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/asio.hpp>
using namespace std;
using boost::asio::ip::tcp;
using namespace boost;

namespace client {
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

	// Input prompts
	string displayInputText(string message) {
		string returnString = "INPUT: " + message;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		// End of adapted code
		std::cout << returnString << endl;
		// Adapted from CPlusPlus.com (Spot3, 2013)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		// End of adapted code
		return returnString;
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
		displayInfoText("Client and server have exited successfully. You may now close this window");
		exit(2);
	}

	int sendInput(int choice) {
		// Method for testing connection with the server
		// If successful, return 1. Otherwise, return 0
		try {
			tcp::iostream stream("127.0.1.1", "7000");
			if (choice == 0) {
				stream << "Connected_to_client";
			}
			else {
				displayInfoText("Connecting to server. Please wait...");
				displayPositiveText("Connected to server");
				stream << choice;
			}
		}
		catch (std::exception e) {
			displayErrorText("Connection to server failed. Please try again. Connection might have failed because:");
			displayErrorText("- You are not connected to a network");
			displayErrorText("- Your firewall might be blocking network requests for this application");
			displayErrorText("- The server might not be in operation due to an outage or scheduled maintenance");

			displayErrorText("Connection error information:");
			displayErrorText(e.what());
			return 0;
		}
		return 1;
	}

	int admin(bool test, int testInput) {
		// Method for admin section
		displayTitleText("Admin");
		displayInputText("Please enter the number for the game you would like to enable or disable and press enter:");
		std::cout << "0: Return to main menu" << endl;
		std::cout << "1: Hangman" << endl;
		std::cout << "2: Mini quiz" << endl;

		try {
			int userChoice;
			if (!test) {
				std::cin >> userChoice;
				tcp::iostream stream("127.0.1.1", "7006");
				stream << userChoice;
			}
			else {
				userChoice = testInput;
			}
		}
		catch (std::exception e) {
			displayErrorText("The input you have entered is not valid");
			if (test) {
				return 0;
			}
		}
		displayPositiveText("Game setting changed");
		return 1;
	}

	int deleteData(string userType, bool test, int testInput) {
		displayTitleText("Delete User Data");
		int confirmation;

		if (userType == "A") {
			displayErrorText("You cannot delete your account because it is an admin account");
			confirmation = 0;
			if (test) {
				return 1;
			}
		}
		else {
			displayWarningText("Are you sure you want to delete your account and gameplay data? Once deleted, the data cannot be recovered");
			displayInputText("Type 1 and press enter to confirm");
			if (!test) {
				std::cin >> confirmation;
			}
			else {
				confirmation = testInput;
			}
		}

		try {
			if (!test) {
				tcp::iostream stream("127.0.1.1", "7007");
				stream << confirmation;
				stream.close();
				if (confirmation == 1) {
					exitApp();
				}
				else {
					displayInfoText("You account has not been deleted");
				}
			}
			else {
				return 0;
			}

		}
		catch (std::exception e) {
			displayErrorText("The input you have entered is not valid");
		}
		return 0;
	}

	int playMiniQuiz(bool test, int testInput) {
		// Method for mini quiz (client)
		displayTitleText("Mini Quiz");

		int input = 0;
		for (int i = 0; i < 7; i++) {
			// Send input to server
			displayInfoText("Question is displayed on the server");
			displayInputText("Please enter the number for the answer you would like to select and press enter. Enter any other number or letter to pass");
			displayInfoText("Entering -1 exits the game");
			tcp::iostream stream("127.0.1.1", "7006");
			try {
				if (!test) {
					std::cin >> input;
				}
				else {
					input = testInput;
				}
			}
			catch (std::exception e) {
				displayWarningText("You have passed this question");
				input = 0;
				if (test) {
					return 1;
				}
			}
			if (!test) {
				stream << input;
				stream.close();

				// Exit game if user enters -1
				if (input == -1) {
					displayWarningText("You have exited from the game");
					return -1;
				}
			}
		}
		return 0;
	}

	int playHangman(bool test, char testInput) {
		// Method for hangman (client)
		asio::io_service ios;
		tcp::endpoint endpoint(tcp::v4(), 7007);
		tcp::acceptor acceptor(ios, endpoint);
		displayTitleText("Hangman");
		bool iterate = true;
		while (iterate) {
			tcp::iostream outcomeStream;
			// Get game outcome from server (1 if the game has concluded; 0 otherwise)
			if (!test) {
				try {
					acceptor.accept(*outcomeStream.rdbuf());
					int outcome;
					outcomeStream >> outcome;
					if (outcome == 1) {
						std::cout << endl;
						return 0;
					}
				}
				catch (std::exception e) {
					displayErrorText(e.what());
				}
			}

			displayInfoText("Hangman graphic and progress are displayed on the server");
			displayInputText("Enter a letter of the chosen word and press enter");
			displayInfoText("Entering a forward slash (/) exits the game");
			char input;
			// Check validity
			bool checkIterate = true;
			while (checkIterate) {
				try {
					if (!test) {
						std::cin >> input;
					}
					else {
						input = testInput;
					}
					break;
				}
				catch (std::exception e) {
					displayErrorText(e.what());
				}
			}

			if (!test) {
				// Send input to server
				tcp::iostream stream("127.0.1.1", "7006");
				stream << input;
				stream.close();
				outcomeStream.close();
				ios.stop();
			}

			// Exit game if user enters forward slash
			if (input == '/') {
				displayWarningText("You have exited from the game");
				return -1;
			}
		}
		return 0;
	}

	int viewLeaderBoard() {
		// Method for leaderboard (client)
		displayTitleText("Leaderboards");
		displayInputText("Which leaderboard would you like to view? Enter the number of the corresponding option and press enter");
		std::cout << "0: Return to main menu" << endl;
		std::cout << "1: View hangman leaderboard" << endl;
		std::cout << "2: View mini quiz leaderboard" << endl;

		try {
			int userChoice;
			std::cin >> userChoice;
			tcp::iostream stream("127.0.1.1", "7006");
			stream << userChoice;
		}
		catch (std::exception e) {
			displayErrorText("The input you have entered is not valid");
		}
		displayPositiveText("The leaderboard is displayed on the server");
		return 0;
	}

	int menu(string userName, string userType, bool test, int testInput) {
		// Method for main menu
		// Number of games a player has played
		int sessionGames = 0;

		bool iterate = true;
		int userChoice = -1;
		while (iterate) {
			displayTitleText("Main Menu");
			std::cout << "What would you like to do, ";
			// Adapted from CPlusPlus.com (Spot3, 2013)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			// Adapted from CPlusPlus.com (Spot3, 2013)
			std::cout << userName;
			// Adapted from CPlusPlus.com (Spot3, 2013)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			// Adapted from CPlusPlus.com (Spot3, 2013)
			std::cout << "?" << endl;

			if (sessionGames > 5) {
				displayWarningText("You have played over five games today. Please take a break");
			}

			try {
				displayInputText("Please enter the number of the corresponding option and press enter:");
				std::cout << "0: Sign out and exit" << endl;
				std::cout << " - Select this option to sign out of your account and exit" << endl;
				std::cout << "1: View leaderboard" << endl;
				std::cout << " - Select this option to view the leaderboards" << endl;
				std::cout << "2: Play hangman" << endl;
				std::cout << " - Select this option play hangman. Can you guess a word before the hangman is constructed?" << endl;
				std::cout << "3: Play mini quiz" << endl;
				std::cout << " - Select this option to play a mini quiz. Can you answer seven multiple-choice questions?" << endl;
				std::cout << "4: Delete my account and leaderboard data" << endl;
				std::cout << " - Select this option to delete your account and associated leaderboard data" << endl;

				// Display option 5 if the user is an admin
				if (userType == "A") {
					std::cout << "5: Manage games" << endl;
					std::cout << " - Select this option to allow or prevent users from accessing certain games" << endl;
				}

				tcp::iostream stream("127.0.1.1", "7005");
				if (!test) {
					std::cin >> userChoice;
				}
				else {
					userChoice = testInput;
				}

				int highestAcceptedInput;
				if (userType == "A") {
					highestAcceptedInput = 5;
				}
				else {
					highestAcceptedInput = 4;
				}

				if (userChoice >= 0 && userChoice <= highestAcceptedInput) {
					if (!test) {
						stream << userChoice;
						stream.close();

					}

					asio::io_service ios;
					tcp::endpoint hangmanEndpoint(tcp::v4(), 7008);
					tcp::acceptor hangmanAcceptor(ios, hangmanEndpoint);
					tcp::iostream hangmanEnableStream;

					tcp::endpoint miniQuizEndpoint(tcp::v4(), 7009);
					tcp::acceptor miniQuizAcceptor(ios, miniQuizEndpoint);
					tcp::iostream miniQuizEnableStream;

					switch (userChoice) {
					case 0:
						if (test) {
							return 0;
						}
						exitApp();
					case 1:
						if (test) {
							return 1;
						}
						viewLeaderBoard();
						break;
					case 2:
						if (test) {
							return 2;
						}
						bool hangmanEnabled;
						// Get hangman availability from server
						displayInfoText("Getting hangman status...");
						hangmanAcceptor.accept(*hangmanEnableStream.rdbuf());
						hangmanEnableStream >> hangmanEnabled;
						displayPositiveText("Status received");

						if (hangmanEnabled) {
							playHangman(false, '0');
							sessionGames++;
						}
						else {
							std::cout << endl;
							displayErrorText("Hangman is not available for play at this time. Please try again later");
							std::cout << endl;
						}
						break;
					case 3:
						if (test) {
							return 3;
						}
						bool miniQuizEnabled;
						// Get mini quiz availability from server
						displayInfoText("Getting mini quiz status...");
						miniQuizAcceptor.accept(*miniQuizEnableStream.rdbuf());
						miniQuizEnableStream >> miniQuizEnabled;
						std::cout << "Received" << endl;

						hangmanEnableStream.close();
						miniQuizEnableStream.close();
						ios.stop();
						if (miniQuizEnabled) {
							playMiniQuiz(false, 0);
							sessionGames++;
						}
						else {
							std::cout << endl;
							displayErrorText("Mini quiz is not available for play at this time. Please try again later");
							std::cout << endl;
						}
						break;
					case 4:
						if (test) {
							return 4;
						}
						deleteData(userType, false, 0);
						break;
					case 5:
						if (test) {
							return 5;
						}
						admin(false, -1);
						break;
					default:
						throw "Unexpected option detected";
					}
				}
				else if (userChoice == 5 && userType == "A") {
					admin(false, -1);
				}
				else {
					// If no valid options were selected, notify user
					displayErrorText("The input you have entered could not be accepted");
				}
			}
			catch (std::exception e) {
				// If no valid options were selected, notify user
				displayErrorText("The input you have entered could not be accepted");
			}

			if (test) {
				return -1;
			}
		}
		return 0;
	}

	// Code for hiding console input. Adapted from CPlusPlus.com (Null, 2023)
	string enterPassword(bool show_asterisk = true)
	{
		const char BACKSPACE = 8;
		const char RETURN = 13;

		string password;
		unsigned char ch = 0;

		DWORD con_mode;
		DWORD dwRead;

		HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

		GetConsoleMode(hIn, &con_mode);
		SetConsoleMode(hIn, con_mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

		while (ReadConsoleA(hIn, &ch, 1, &dwRead, NULL) && ch != RETURN)
		{
			if (ch == BACKSPACE)
			{
				if (password.length() != 0)
				{
					if (show_asterisk)
						cout << "\b \b";
					password.resize(password.length() - 1);
				}
			}
			else
			{
				password += ch;
				if (show_asterisk)
					cout << '*';
			}
		}
		cout << endl;
		return password;
	}
	// End of adapted code

	int login(list<User> users) {
		displayTitleText("Login");
		string status = "";
		bool iterate = true;
		string userName;
		string password;
		int response = -1;

		while (iterate) {
			// Ask for username
			tcp::iostream userNameStream("127.0.1.1", "7001");
			displayInputText("Please enter your username then press enter (or -1 to cancel): ");
			std::cin >> userName;
			userNameStream << userName;
			userNameStream.close();

			// Cancel if user entered -1
			if (userName == "-1") {
				displayWarningText("You have cancelled the login process");
				return 0;
			}

			// Ask for password
			tcp::iostream passwordStream("127.0.1.1", "7002");
			displayInputText("Please enter your password then press enter (or -1 to cancel): ");
			std::cin >> password;
			// Adapted from CPlusPlus.com (Null, 2023)
			//password = enterPassword(true);
			// End of adapted code

			// Adapted from cppreference.com (2023)
			size_t hashedPassword = std::hash<string>{}(password);
			// End of adapted code
			passwordStream << hashedPassword;
			passwordStream.close();

			// Adapted from cppreference.com (2023)
			size_t hashedCancelString = std::hash<string>{}("-1");
			// End of adapted code

			// Cancel if user entered -1
			if (hashedPassword == hashedCancelString) {
				displayWarningText("You have cancelled the login process");
				return 0;
			}

			// Get user type from server. If the account authentication check passes on the server, the variable response should not be -1
			asio::io_service ios;
			tcp::endpoint responseEndpoint(tcp::v4(), 7004);
			tcp::acceptor responseAcceptor(ios, responseEndpoint);
			tcp::iostream responseStream;
			responseAcceptor.accept(*responseStream.rdbuf());
			responseStream >> response;
			responseStream.close();

			// Cancel if user entered -1
			if (response != -1) {
				break;
			}
			else {
				displayErrorText("The username and password that you have entered could not be verified. Please try again");
			}

			ios.stop();
		}

		string type;
		switch (response) {
		case 0:
			type = "U";
			break;
		case 1:
			type = "A";
			break;
		default:
			type = "X";
		}

		displayPositiveText("Welcome back!");
		menu(userName, type, false, 0);
		return 0;
	}

	list<User> createAccount(list<User> users) {
		displayTitleText("Create an Account");
		bool iterate = true;
		string userName;
		while (iterate) {
			displayInfoText("Create an account by entering a username and password. Please note that the username you choose should not have been taken by another user. We will let you know if you choose an existing username");
			// Ask for username
			tcp::iostream userNameStream("127.0.1.1", "7001");
			displayInputText("Please enter a username then press enter (or -1 to cancel): ");
			std::cin >> userName;
			userNameStream << userName;
			userNameStream.close();

			// Cancel if user entered -1
			if (userName == "-1") {
				displayWarningText("You have cancelled the account creation process");
				return users;
			}

			// Ask for password twice
			tcp::iostream passwordStream("127.0.1.1", "7002");
			displayInputText("Please enter a password then press enter (or -1 to cancel): ");
			string password;
			std::cin >> password;

			// Adapted from CPlusPlus.com (Null, 2023)
			//password = enterPassword(true);
			// End of adapted code

			// Cancel if user entered -1
			if (password == "-1") {
				displayWarningText("You have cancelled the account creation process");
				return users;
			}

			// Adapted from cppreference.com (2023)
			size_t hashedPassword = std::hash<string>{}(password);
			// End of adapted code
			passwordStream << hashedPassword;
			passwordStream.close();

			displayInputText("Please enter your password once more then press enter (or -1 to cancel): ");
			tcp::iostream passwordAgainStream("127.0.1.1", "7003");
			string passwordAgain;
			std::cin >> passwordAgain;

			// Adapted from CPlusPlus.com (Null, 2023)
			//passwordAgain = enterPassword(true);
			// End of adapted code

			// Cancel if user entered -1
			if (passwordAgain == "-1") {
				displayWarningText("You have cancelled the account creation process");
				return users;
			}

			// Adapted from cppreference.com (2023)
			size_t hashedPasswordAgain = std::hash<string>{}(passwordAgain);
			// End of adapted code
			passwordAgainStream << hashedPasswordAgain;
			passwordAgainStream.close();

			// Get response from server
			asio::io_service ios;
			tcp::endpoint responseEndpoint(tcp::v4(), 7004);
			tcp::acceptor responseAcceptor(ios, responseEndpoint);
			tcp::iostream responseStream;
			responseAcceptor.accept(*responseStream.rdbuf());
			int response;
			responseStream >> response;
			responseStream.close();

			if (response == 1) {
				break;
			}
			else {
				displayErrorText("Your account could not be created. Make sure you have entered the same password twice. Otherwise, please try a different username");
			}
			ios.stop();
		}

		displayPositiveText("Account created. Loading main menu...");
		menu(userName, "U", false, 0);
		return users;
	}
}