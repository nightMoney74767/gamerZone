// This file contains the source code for the server interface
// Click the Start button in the toolbar for Visual Studio to run the client and server interfaces
// The following directories have been used for Boost
// These may be changed in Visual Studio project settings if Boost has been setup in a different location
// - Additional Include directory (in C/C++ settings page): C:\Boost\boost_1_80_0\boost_1_80_0
// - Additional Library directory (in Linker settings page): C:\Boost\boost_1_80_0\boost_1_80_0\stage\lib

#include <iostream>
#include <fstream>
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

#include "server.h"
using namespace server;

int main() {
	// Main method for server interface
	// Set text colour to white and background colour to black. Adapted from GeeksForGeeks (tanmaythole, 2020)
	std::system("Color 07");
	// End of adapted code

	list<User> users;
	//users = setup(users);
	//setupHangmanLeaderboard();
	//setupQuizLeaderboard();
	//setupGameManagement();

	// Get users
	ifstream importFile("userList.dat");
	archive::text_iarchive archIn(importFile);
	archIn >> users;

	std::cout << "This is the server interface" << endl;
	displayInfoText("Waiting for a client to connect...");
	try {
		asio::io_service ios;
		tcp::endpoint endpoint(tcp::v4(), 7000);
		tcp::acceptor acceptor(ios, endpoint);
		tcp::iostream stream;
		acceptor.accept(*stream.rdbuf());
		string recv;
		stream >> recv;
		stream.close();
		ios.stop();
		displayPositiveText("Client connected");
	}
	catch (std::exception e) {
		displayErrorText(e.what());
		exitApp();
	}

	// Get user choice from client
	bool accountIterate = true;
	while (accountIterate) {
		displayInfoText("Waiting for a client to select an option...");
		try {
			asio::io_service ios;
			tcp::endpoint endpoint(tcp::v4(), 7000);
			tcp::acceptor acceptor(ios, endpoint);
			tcp::iostream stream;
			acceptor.accept(*stream.rdbuf());
			int recv;
			stream >> recv;
			stream.close();
			ios.stop();

			switch (recv) {
			case 0:
				exit(2);
				break;
			case 1:
				login(users, false, "", 0);
				break;
			case 2:
				createAccount(users, false, "", 0, 0);
				break;
			}
		}
		catch (std::exception e) {
			displayErrorText(e.what());
			exitApp();
		}
	}
}