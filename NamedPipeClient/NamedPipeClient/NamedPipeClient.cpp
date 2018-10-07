/*Author: Suraj Pandey
Done as StreamLabs assignment
Referenced from Microsoft NamedPipeClient Documentation
*/

#include "NamedPipeClient.h"

#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <strsafe.h>
#include <sstream>
#include <ctime>

#define BUFSIZE 512

void userInput(std::string& input) {
	std::cout << "*****************************\n";
	std::cout << "Enter: \n";
	std::cout << " --num to send a NUMBER\n";
	std::cout << " --inum to send a NUMBER asynchronously\n";
	std::cout << " --msg to send a MESSAGE\n";
	std::cout << " --imsg to send a MESSAGE asynchronously\n";
	std::cout << " --cobj to create an object of type employee\n";
	std::cout << " --icobj to create an object of type employee\n";
	std::cout << " --exit to terminate\n";
	std::cout << " --state to check your objects status in the server\n";
	std::cout << " --choose to access one of your object in the server\n";
	std::cout << "*******************************\n";
	std::cout << "Note: don't forget to use -- at the beginning\n";

	std::cin >> input;
}

std::string client_id = "";

void createNewUniqueClientId() {
	client_id = std::to_string(std::time(0));
}

std::string validateUserInput(std::string input) {
	if (client_id == "") createNewUniqueClientId();
	std::string client_msg = client_id;
	if (input == "--num" || input == "--inum") {
		std::cout << " Enter the number you want to send\n";
		double num;
		std::cin >> num;
		client_msg = client_msg + ",n," + std::to_string(num);
	}
	else if (input == "--msg" || input == "--imsg") {
		std::cout << " Enter the string/message you want to send\n";
		std::string msg;
		std::cin.ignore();
		std::getline(std::cin, msg);
		//std::cin >> msg;
		client_msg = client_msg + ",m," + msg;
	}
	else if (input == "--cobj" || input == "--icobj") {
		std::cout << "Employee class has the following attributes: {name, salary, address, mobile_num, company_name}\n";
		std::string name, address, company_name;
		std::cin.clear();
		std::string mobile_num;
		double salary;
		std::cout << "Enter name for the new Employee\n";
		std::cin.clear();
		std::cin.ignore();
		std::getline(std::cin, name);
		std::cout << "Enter the address of the new Employee\n";
		std::getline(std::cin, address);
		std::cout << "Enter the company_name of the new Employee\n";
		std::getline(std::cin, company_name);
		std::cout << "Enter the mobile_num of the new Employee\n";
		std::getline(std::cin, mobile_num);
		std::cout << "Enter the salary of the new Employee\n";
		std::cin >> salary;
		client_msg = client_msg + ",c," + name + "," + address + "," + company_name + "," + mobile_num + "," + std::to_string(salary);
	}
	else if (input == "--state") {
		client_msg = client_msg + ",state";
	}
	else if (input == "--choose") {
		std::cout << "Enter the index of the object that you want to access\n";
		int idx;
		std::cin >> idx;
		std::cout << "*******************************\n";
		std::cout << "Enter:\n";
		std::cout << " getn to execute getName() method on that object\n";
		std::cout << " setn to execute setName() method on that object\n";
		std::cout << " getad to execute getAddress() method on that object\n";
		std::cout << " setad to execute setAddress() method on that object\n";
		std::cout << " getcom to execute getCompanyName() method on that object\n";
		std::cout << " setcom to execute setCompanyName() method on that object\n";
		std::cout << " getmbl to execute getMobileNum() method on that object\n";
		std::cout << " setmbl to execute setMobileNum() method on that object\n";
		std::cout << " getsal to execute getSalary() method on that object\n";
		std::cout << " setsal to execute setSalary() method on that object\n";
		std::cout << "*******************************\n";
		std::string choose_option;
		std::cin.clear();
		std::cin.ignore();
		std::cin >> choose_option;
		std::cin.clear();
		std::cin.ignore();
		if (choose_option == "getn") {
			client_msg = client_msg + ",choose,getn," + std::to_string(idx);
		}
		else if (choose_option == "setn") {
			std::cout << "Enter the name that you want to set to this Employee object\n";
			std::string name;
			std::getline(std::cin, name);
			//std::cin >> name;
			client_msg = client_msg + ",choose,setn," + name + "," + std::to_string(idx);
		}
		else if (choose_option == "getad") {
			client_msg = client_msg + ",choose,getad," + std::to_string(idx);
		}
		else if (choose_option == "setad") {
			std::cout << "Enter the address that you want to set to this Employee object\n";
			std::string address;
			std::getline(std::cin, address);
			//std::cin >> address;
			client_msg = client_msg + ",choose,setad," + address + "," + std::to_string(idx);
		}
		else if (choose_option == "getcom") {
			client_msg = client_msg + ",choose,getcom," + std::to_string(idx);
		}
		else if (choose_option == "setcom") {
			std::cout << "Enter the company_name that you want to set to this Employee object\n";
			std::string company_name;
			std::getline(std::cin, company_name);
			//std::cin >> company_name;
			client_msg = client_msg + ",choose,setcom," + company_name + "," + std::to_string(idx);
		}
		else if (choose_option == "getmbl") {
			client_msg = client_msg + ",choose,getmbl," + std::to_string(idx);
		}
		else if (choose_option == "setmbl") {
			std::cout << "Enter the mobile number that you want to set to this Employee object\n";
			std::string mobile_num;
			std::getline(std::cin, mobile_num);
			//std::cin >> mobile_num;
			client_msg = client_msg + ",choose,setmbl," + mobile_num + "," + std::to_string(idx);
		}
		else if (choose_option == "getsal") {
			client_msg = client_msg + ",choose,getsal," + std::to_string(idx);
		}
		else if (choose_option == "setsal") {
			std::cout << "Enter the salary that you want to set to this Employee object\n";
			std::string salary;
			std::cin >> salary;
			client_msg = client_msg + ",choose,setsal," + salary + "," + std::to_string(idx);
		}
		else {
			std::cout << "Invalid input. Client Terminated\n";
			exit(1);
		}
	}
	else if (input == "--exit") {
		std::cout << "Client Terminated Gracefully\n";
		exit(0);
	}
	else {
		std::cout << "Invalid input. Client Terminated\n";
		exit(1);
	}
	return client_msg;
}

int _tmain(int argc, TCHAR *argv[])
{
	createNewUniqueClientId();
	HANDLE handleToPipe;
	LPTSTR messageToSend = TEXT("state");
	TCHAR  receivedMessage[BUFSIZE];
	BOOL isSuccess = FALSE;
	BOOL isAysnc = FALSE;
	DWORD  numBytesRead, numBytesToWrite, bytesWritten, dwMode;
	LPTSTR pipeName = TEXT("\\\\.\\pipe\\mynamedpipe");


	//let's read in some arguments
	if (argc > 2) {
		if (argc % 2 == 0) {
			printf("Error: Some arguments are not complete\n");
			return -1;
		}
		int idx = 1;
		while (idx < argc) {
			std::string arg = argv[idx];
			if (strcmp(arg.c_str(), "-m") == 0) {
				messageToSend = argv[++idx];
				++idx;
			}
			else if (strcmp(arg.c_str(), "-im") == 0) {
				messageToSend = argv[++idx];
				isAysnc = TRUE;
				++idx;
			}
			else {
				printf("Unknown argument provided.\n");
				return -1;
			}
			printf("arg: %s\n", arg.c_str());
		}
		// messageToSend = argv[1]; 
	}

	while (1) {
		std::string user_input;
		userInput(user_input);
		std::string client_msg = validateUserInput(user_input);

		while (1) {

			if (!isAysnc) {
				handleToPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			}
			else {
				handleToPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			}

			if (handleToPipe != INVALID_HANDLE_VALUE)
				break;

			if (GetLastError() != ERROR_PIPE_BUSY) {
				_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
				return -1;
			}

			if (!WaitNamedPipe(pipeName, 20000)) {
				printf("Pipe cannot be openend, timed out after 20 seconds");
				return -1;
			}
		}

		std::cout << "Connected to server\n";

		dwMode = PIPE_READMODE_MESSAGE;
		isSuccess = SetNamedPipeHandleState(handleToPipe, &dwMode, NULL, NULL);

		if (!isSuccess) {
			_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
			return -1;
		}

		messageToSend = LPTSTR(client_msg.c_str());
		numBytesToWrite = (lstrlen(messageToSend) + 1) * sizeof(TCHAR);
		_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), numBytesToWrite, messageToSend);

		isSuccess = WriteFile(handleToPipe, messageToSend, numBytesToWrite, &bytesWritten, NULL);

		if (!isSuccess) {
			_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
			return -1;
		}

		printf("\nMessage sent to server\n");

		do {
			isSuccess = ReadFile(handleToPipe, receivedMessage, BUFSIZE * sizeof(TCHAR), &numBytesRead, NULL);
			if (!isSuccess && GetLastError() != ERROR_MORE_DATA)
				break;
			_tprintf(TEXT("\"%s\"\n"), receivedMessage);
		} while (!isSuccess);

		if (!isSuccess) {
			_tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
			return -1;
		}

		printf("\nEnd of operation, enter x to terminate or c to continue next operation\n");
		std::cin >> user_input;
		if (user_input == "x") {
			return 0;
		}
		else if (user_input == "c") {
		}
		else {
			std::cout << "Invalid input. Terminating\n";
			return -1;
		}
		CloseHandle(handleToPipe);
	}

	return 0;
}
