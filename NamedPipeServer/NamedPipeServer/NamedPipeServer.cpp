/*Author: Suraj Pandey
Done as StreamLabs assignment
Referenced from Microsoft NamedPipeServer Documentation
*/

#include "NamedPipeServer.h"

#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#define BUFSIZE 512

class Employee {
	std::string name;
	double salary;
	std::string company_name;
	std::string address;
	std::string mobile_num;
public:
	Employee() {
	}
	Employee(std::string name, std::string address, std::string company_name, std::string mobile_num, double salary) {
		this->name = name;
		this->salary = salary;
		this->company_name = company_name;
		this->address = address;
		this->mobile_num = mobile_num;
	}
	std::string getName() {
		return this->name;
	}
	std::string getCompanyName() {
		return this->company_name;
	}
	std::string getAddress() {
		return this->address;
	}
	double getSalary() {
		return this->salary;
	}
	std::string getMobileNum() {
		return this->mobile_num;
	}

	void setName(std::string name) {
		this->name = name;
	}
	void setCompanyName(std::string company_name) {
		this->company_name = company_name;
	}
	void setAddress(std::string address) {
		this->address = address;
	}
	void setSalary(double salary) {
		this->salary = salary;
	}
	void setMobileNum(std::string mobile_num) {
		this->mobile_num = mobile_num;
	}
};

/*employee instances created by different clients
	std::string:			client_id
	vector<Employee*>:		employee instances created by a client
*/
std::map<std::string, std::vector<Employee*>> employee_instances;
std::map<std::string, std::vector<std::string>> messages_sent_by_client;
std::map<std::string, std::vector<double>> nums_sent_by_client;

DWORD WINAPI InstanceThread(LPVOID);
VOID GetAnswerToRequest(LPTSTR, LPTSTR, LPDWORD);
int _tmain(VOID)
{
	BOOL   fConnected = FALSE;
	DWORD  dwThreadId = 0;
	HANDLE hPipe = INVALID_HANDLE_VALUE, hThread = NULL;
	LPTSTR pipeName = TEXT("\\\\.\\pipe\\mynamedpipe");

	while (true)
	{
		_tprintf(TEXT("\n********* Named Pipe Server Started on %s ***********\n"), pipeName);
		hPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, BUFSIZE, BUFSIZE, 0, NULL);

		if (hPipe == INVALID_HANDLE_VALUE) {
			_tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError());
			return -1;
		}

		//wait until client connects
		fConnected = ConnectNamedPipe(hPipe, NULL) ?
			TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

		if (fConnected) {
			printf("Client connected, creating a processing thread.\n");
			hThread = CreateThread(NULL, 0, InstanceThread, (LPVOID)hPipe, 0, &dwThreadId);

			if (hThread == NULL) {
				_tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError());
				return -1;
			}
			else CloseHandle(hThread);
		}
		else
			// The client could not connect, so close the pipe. 
			CloseHandle(hPipe);
	}

	return 0;
}

DWORD WINAPI InstanceThread(LPVOID lpvParam) {
	HANDLE hHeap = GetProcessHeap();
	TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));
	TCHAR* pchReply = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));

	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
	BOOL fSuccess = FALSE;
	HANDLE hPipe = NULL;

	if (lpvParam == NULL) {
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL value in lpvParam.\n");
		printf("   InstanceThread exitting.\n");
		if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
		if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}

	if (pchRequest == NULL) {
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL heap allocation.\n");
		printf("   InstanceThread exitting.\n");
		if (pchReply != NULL) HeapFree(hHeap, 0, pchReply);
		return (DWORD)-1;
	}

	if (pchReply == NULL) {
		printf("\nERROR - Pipe Server Failure:\n");
		printf("   InstanceThread got an unexpected NULL heap allocation.\n");
		printf("   InstanceThread exitting.\n");
		if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
		return (DWORD)-1;
	}

	hPipe = (HANDLE)lpvParam;

	// Loop until done reading
	while (1) {
		fSuccess = ReadFile(hPipe, pchRequest, BUFSIZE * sizeof(TCHAR), &cbBytesRead, NULL);

		if (!fSuccess || cbBytesRead == 0)
		{
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				_tprintf(TEXT("InstanceThread: client disconnected.\n"), GetLastError());
			}
			else
			{
				_tprintf(TEXT("InstanceThread ReadFile failed, GLE=%d.\n"), GetLastError());
			}
			break;
		}

		// Process the incoming message.
		GetAnswerToRequest(pchRequest, pchReply, &cbReplyBytes);


		// Write the reply to the pipe. 
		fSuccess = WriteFile(hPipe, pchReply, cbReplyBytes, &cbWritten, NULL);

		if (!fSuccess || cbReplyBytes != cbWritten)
		{
			_tprintf(TEXT("InstanceThread WriteFile failed, GLE=%d.\n"), GetLastError());
			break;
		}
	}

	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);

	HeapFree(hHeap, 0, pchRequest);
	HeapFree(hHeap, 0, pchReply);

	printf("InstanceThread exitting.\n");
	return 1;
}

VOID GetAnswerToRequest(LPTSTR pchRequest,
	LPTSTR pchReply,
	LPDWORD pchBytes) {
	_tprintf(TEXT("Client Request String:\"%s\"\n"), pchRequest);

	std::string server_reply = "";
	char* token = strtok(pchRequest, ",");
	std::vector<std::string> tokens = {};
	while (token != NULL) {
		tokens.push_back(std::string(token));
		token = strtok(NULL, ",");
	}
	//let's check the type of request the client sent
	std::string client_id = tokens[0];
	std::string req = tokens[1];
	if (req == "n") {
		double num = std::stod(tokens[2]);
		std::cout << "Client sent a number " << tokens[2] << "\n";
		if (nums_sent_by_client.find(client_id) == nums_sent_by_client.end()) {
			nums_sent_by_client[client_id] = { num };
		}
		else {
			nums_sent_by_client[client_id].push_back(num);
		}
	}
	else if (req == "m") {
		std::string msg = tokens[2];
		for (int i = 3; i < tokens.size(); i++) msg += " " + tokens[i];
		std::cout << "Client sent a message " << msg << "\n";
		if (messages_sent_by_client.find(client_id) == messages_sent_by_client.end()) {
			messages_sent_by_client[client_id] = { msg };
		}
		else {
			messages_sent_by_client[client_id].push_back(msg);
		}
	}
	else if (req == "c") {
		printf("Client requested to create an object of Employee class\n");
		std::string name = tokens[2];
		std::string address = tokens[3];
		std::string company_name = tokens[4];
		std::string mobile_num = tokens[5];
		double salary = std::stod(tokens[6]);
		if (employee_instances.find(client_id) == employee_instances.end()) {
			employee_instances[client_id] = { new Employee(name, address, company_name, mobile_num, salary) };
		}
		else {
			employee_instances[client_id].push_back(new Employee(name, address, company_name, mobile_num, salary));
		}
	}
	else if (req == "choose") {
		printf("Client requested to choose one of the object of Employee class\n");
		int tokens_size = tokens.size();
		int employee_idx = stoi(tokens[tokens_size - 1]);
		if (employee_idx >= employee_instances[client_id].size()) {
			server_reply = "The employee indices runs from 0 to " + std::to_string(employee_instances[client_id].size() - 1) +
				" . Employee index " +
				std::to_string(employee_idx) +
				" does not exist\n";
		}
		else {
			std::string method_to_exec = tokens[2];
			if (method_to_exec == "getn") {
				server_reply = "The name attribute of the selected employee is " +
					employee_instances[client_id][employee_idx]->getName() + "\n";
			}
			else if (method_to_exec == "setn") {
				std::string name = tokens[tokens_size - 2];
				employee_instances[client_id][employee_idx]->setName(name);
				server_reply = "The name attribute of the selected employee is changed to " +
					employee_instances[client_id][employee_idx]->getName() + "\n";
			}
			else if (method_to_exec == "getad") {
				server_reply = "The address attribute of the selected employee is " +
					employee_instances[client_id][employee_idx]->getAddress() + "\n";
			}
			else if (method_to_exec == "setad") {
				std::string address = tokens[tokens_size - 2];
				employee_instances[client_id][employee_idx]->setAddress(address);
				server_reply = "The address attribute of the selected employee is changed to " +
					employee_instances[client_id][employee_idx]->getAddress() + "\n";
			}
			else if (method_to_exec == "getcom") {
				server_reply = "The company_name attribute of the selected employee is " +
					employee_instances[client_id][employee_idx]->getCompanyName() + "\n";
			}
			else if (method_to_exec == "setcom") {
				std::string company_name = tokens[tokens_size - 2];
				employee_instances[client_id][employee_idx]->setCompanyName(company_name);
				server_reply = "The company_name attribute of the selected employee is changed to " +
					employee_instances[client_id][employee_idx]->getCompanyName() + "\n";
			}
			else if (method_to_exec == "getmbl") {
				server_reply = "The mobile_num attribute of the selected employee is " +
					employee_instances[client_id][employee_idx]->getMobileNum() + "\n";
			}
			else if (method_to_exec == "setmbl") {
				std::string mobile_num = tokens[tokens_size - 2];
				employee_instances[client_id][employee_idx]->setMobileNum(mobile_num);
				server_reply = "The mobile_num attribute of the selected employee is changed to " +
					employee_instances[client_id][employee_idx]->getMobileNum() + "\n";
			}
			else if (method_to_exec == "getsal") {
				server_reply = "The salary attribute of the selected employee is " +
					std::to_string(employee_instances[client_id][employee_idx]->getSalary()) + "\n";
			}
			else if (method_to_exec == "setsal") {
				std::string salary = tokens[tokens_size - 2];
				employee_instances[client_id][employee_idx]->setSalary(std::stod(salary));
				server_reply = "The salary attribute of the selected employee is changed to " +
					std::to_string(employee_instances[client_id][employee_idx]->getSalary()) + "\n";
			}
		}
	}
	else if (strcmp(req.c_str(), "state") == 0) {
		server_reply = "Your available objects:\n[\n";
		if (employee_instances.find(client_id) != employee_instances.end()) {
			std::vector<Employee*> clients_employee_instances = employee_instances[client_id];
			int count = 0;
			std::string name;
			double salary;
			std::string company_name;
			std::string address;
			std::string mobile_num;
			for (auto item : clients_employee_instances) {
				server_reply += "idx: " + std::to_string(count++) + " \n{\n name: " + item->getName() +
					"\n salary: " + std::to_string(item->getSalary()) + "\n company_name: " + item->getCompanyName() +
					"\n address: " + item->getAddress() + "\n mobile_num: " + item->getMobileNum() +
					"\n}\n";
			}
		}
		server_reply += "\n]\n";
		server_reply += "Your sent string messages were:\n[";
		if (messages_sent_by_client.find(client_id) != messages_sent_by_client.end()) {
			int count = 0;
			std::vector<std::string> clients_messages = messages_sent_by_client[client_id];
			for (auto item : clients_messages) {
				server_reply += "\nidx: " + std::to_string(count++) + ", value: " + item + " \n";
			}
		}
		server_reply += "\n]\n";
		server_reply += "Your sent numbers were:\n[";
		if (nums_sent_by_client.find(client_id) != nums_sent_by_client.end()) {
			int count = 0;
			std::vector<double> clients_nums = nums_sent_by_client[client_id];
			for (auto item : clients_nums) {
				server_reply += "\nidx: " + std::to_string(count++) + ", value: " + std::to_string(item) + " \n";
			}
		}
		server_reply += "\n]\n";
	}

	// Check the outgoing message to make sure it's not too long for the buffer.
	if (FAILED(StringCchCopy(pchReply, BUFSIZE, TEXT(server_reply.c_str()))))
	{
		*pchBytes = 0;
		pchReply[0] = 0;
		printf("StringCchCopy failed, no outgoing message.\n");
		return;
	}
	*pchBytes = (lstrlen(pchReply) + 1) * sizeof(TCHAR);
}
