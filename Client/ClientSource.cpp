// This file contains the source code for the client interface
// Click the Start button in the toolbar for Visual Studio to run the client and server interfaces
// The following directories have been used for Boost
// These may be changed in Visual Studio project settings if Boost has been setup in a different location
// - Additional Include directory (in C/C++ settings page): C:\Boost\boost_1_80_0\boost_1_80_0
// - Additional Library directory (in Linker settings page): C:\Boost\boost_1_80_0\boost_1_80_0\stage\lib

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

#include "client.h"
using namespace client;

int main() {
	// Main method for client interface
	// Set text colour to white and background colour to black. Adapted from GeeksForGeeks (tanmaythole, 2020)
	std::system("Color 07");
	// End of adapted code
	std::cout << "This is the client interface" << endl;
	
	int connectionResult = client::sendInput(0);

	if (connectionResult == 0) {
		exit(3);
	}

	// Setup users data file. Comment out once file has been generated
	list<User> users;
	//ofstream exportFile("users.dat");
	//archive::text_oarchive arch(exportFile);
	//arch << users;

	// Get users
	ifstream importFile("users.dat");
	archive::text_iarchive archIn(importFile);
	archIn >> users;

	bool accountIterate = true;
	while (accountIterate) {
		try {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
			std::cout << endl;
			std::cout << "|-------|    --------|      |      |  " << endl;
			std::cout << "|                   |     --|--  --|--" << endl;
			std::cout << "|                 |         |      |  " << endl;
			std::cout << "|    ---|        |                    " << endl;
			std::cout << "|       |       |                     " << endl;
			std::cout << "|       |     |                       " << endl;
			std::cout << "|-------|    |--------                " << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

			std::cout << "\nWelcome to GameZone++. What would you like to do?" << endl;
			displayInputText("Enter the corresponding number and press enter:");
			std::cout << "0: Exit" << endl;
			std::cout << " - Select this option to exit" << endl;
			std::cout << "1: Login" << endl;
			std::cout << " - Select this option to login to an existing account" << endl;
			std::cout << "2: Create account " << endl;
			std::cout << " - Select this option to create an account if you do not already have one" << endl;
			int accountChoice = 0;
			std::cin >> accountChoice;

			if (accountChoice == 1 || accountChoice == 2 || accountChoice == 0) {
				sendInput(accountChoice);
				switch (accountChoice) {
				case 0:
					exitApp();
					break;
				case 1:
					login(users);
					break;
				case 2:
					createAccount(users);
					break;
				}
				break;
			}
			else {
				displayErrorText("The input you have entered could not be accepted");
			}
		}
		catch (std::exception e) {
			displayErrorText("The input you have entered could not be accepted");
		}
	}
}
