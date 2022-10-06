# basicRAT
- **basicRAT** is a really simple **remote access trojan** written in C and made for **Windows**.
- malware.c is **intened to run on Windows**, server.c is **intended to run on Linux**

## Before you compile:
- **malware.c** is the payload file which has to be executed on the target computer.
- **server.c** is the server which handles all the connections and then lets you execute commands on remote systems.

### in server.c
- on line 259: Change the **IP** address to the IP you want to listen on (can be found in ipconfig).
- on line 260: Change the **PORT** to the port you want to listen on. It is less suspicious to set this to some commonly used port like 80 or 443.

### in malware.c
- on line 77: Change the **ServIP** variable to the server listener IP address (the one you specified in the server.c file).
- on line 78: Change the **ServPort** variable to the server listener PORT number (the one you specified in the server.c file).

## Compilation:
- Now compile both files using for example GCC.

## Execution
- First run the **server** file on the **attacker system**.
- Then run the **malware.exe** file on the **target system**.
- In few seconds you should **receive a connection on the server**.

## Usage
- After you get the connection, everything you need can be found using the `help` command:
```
in MENU:
quit - disconnect all clients
clients - show all connected clients
shell - get a remote shell on any client
botnet - activates botnet mode (shell which executes commands on all clients)

in SHELL:
q - disconnect client from server
bg - background client

in BotnetSHELL:
showOutput - toggles output(if on, slower)
q - disconnect all clients
bg - background all clients
```

## Important:
- Note that this is a really simple RAT and will be **flagged by any antivirus software**. If you want to bypass antivirus with this, you need to obfuscate the executable. For this I can suggest for example the ScareCrow and Donut repositories, but they are already getting flagged by some antiviruses so you really need to run it in a VM first to test if it won't get flagged.
- **This tool was made purely for educational purposes. Do not use this tool without permission of the system's owner. I am not responsible for anything you cause by using this tool.**

## Known Issues:
- The botnet mode is not working properly. The commands do not execute at the same time, they execute on the first machine, wait for the response and then go to the next. This is really inefficient.

## Development status:
- **I am currently not working on this project.**
