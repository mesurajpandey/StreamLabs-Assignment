# StreamLabs-Assignment
This is an assignment provided by StreamLabs. This code is specially written 
for an Windows Operating system. You need CMake and Visual Studio Compiler to build this project.

## How to build this project ##
There are two projects (NamedPipeClient and NamedPipeServer). You have to build those two projects 
separately.
### Build NamedPipeServer
- **Clone the repo**
- **Go to the NamedPipeServer Directory and cmake the NamedPipeServer project**

 ```
   cd NamedPipeServer
   cmake .
 ```
 
- **A new Visual studio project will be built from cmake command. So, open this project in Visual Studio and build the solution**
- **Go to the folder containing .exe of the project and run the NamedPipeServer**

```
  cd NamedPipeServer\NamedPipeServer\Debug
  NamedPipeServer.exe
```
### Build NamedPipeClient
- **In a new terminal, Go to the NamedPipeClient Directory and cmake the NamedPipeClient project**

 ```
   cd NamedPipeClient
   cmake .
 ```
 
- **A new Visual studio project will be built from cmake command. So, open this project in Visual Studio and build the solution**
- **Go to the folder containing .exe of the project and run the NamedPipeClient**

```
  cd NamedPipeClient\NamedPipeClient\Debug
  NamedPipeClient
```

Now you have two processes (NamedPipeServer and NamedPipeClient) running in two different terminals.

## How to Test this project ##
  - To use NamedPipeClient, the below instructions are provided by NamedPipeClient when you run it.
  
  ```
     *****************************
     Enter:
     --num to send a NUMBER
     --inum to send a NUMBER asynchronously
     --msg to send a MESSAGE
     --imsg to send a MESSAGE asynchronously
     --cobj to create an object of type employee
     --icobj to create an object of type employee
     --exit to terminate
     --state to check your objects status in the server
     --choose to access one of your object in the server
     *******************************
  ```
  
  ***Note: Don't Froget to use -- when you type any of the above commands***

Whenever you send something from NamedPipeClient to NamedPipeServer, you can check the status of your 
server by sending --state command to the server. The server will reply with the state that 
it is in. But it will only reply the state information related to this client.


### Connecting to NamedPipeServer by two NamedPipeClients ###
 - Run NamedPipeClient.exe in two terminals
 - Run NamedPipeServer.exe in another third terminal
 - Send a --msg or --num command to the NamedPipeServer from one of the NamedPipeClient
 - And again, send a --msg or --num command to the NamedPipeServer from another NamedPipeClient
 - Now, when you do --state, the server replies with the corresponding changes to its state done by the 
 corresponding clients


### Create an Employee Object to NamedPipeServer by two NamedPipeClients ###
The server has a custom class (Employee Class) whose objects can be created by the clients.
The Employee class has the following attributes:
 ```
  name: string
  address: string
  company_name: string
  mobile_num: string
  salary: double
 ```

The Employee class has the following methods which can be called by the client using a particular Employee Object.

```
getName()
setName()
getAddress()
setAddress()
getCompanyName()
setCompnayName()
getMobileNum()
setMobileNum()
getSalary()
setSalary()
```




