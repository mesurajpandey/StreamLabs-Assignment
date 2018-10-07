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
- **Go to the NamedPipeClient Directory and cmake the NamedPipeClient project**

 ```
   cd NamedPipeClient
   cmake .
 ```
 
- **A new Visual studio project will be built from cmake command. So, open this project in Visual Studio and build the solution**
- **Go to the folder containing .exe of the project and run the NamedPipeClient**

```
  cd NamedPipeClient\NamedPipeClient\Debug
  NamedPipeCliet
```

**_NamedPipeServer_**

**_NamedPipeClient_**
