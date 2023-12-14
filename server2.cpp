#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <fstream>

#include "wrapped_funcs.h"

#define PORT 8889

using namespace std;

string execution_time(time_t start_time) {
    return to_string((int) difftime(time(0), start_time)) + " seconds";
}

string screen_resolution() {
    string message;

    system("xrandr > bin/xrandr");
    ifstream file ("bin/xrandr");
    
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            int start_index = line.find("current ") + 8;
            if (start_index != string::npos) {
                int length = line.find(", maximum") - start_index;
                message = line.substr(start_index, length);
                break;
            }
        }
    }
    file.close();
    system("rm bin/xrandr");

    return message;
}

int main() {
    time_t start_time = time(0);
    int server2_socket = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t adrlen = sizeof(addr);

    Bind(server2_socket, (struct sockaddr *) &addr, adrlen);

    Listen(server2_socket, 1);

    int fd = Accept(server2_socket, NULL, NULL);

    char buff[256];
    string command, msg;

    while (1) {
        memset(buff, 0, 256);
        recv(fd, buff, sizeof(buff), 0);
        command = buff;

        if (command == "exit") {
            break;
        }

        cout << endl << "Command received: " << command << endl;

        if(command == "server_help") {
            msg = "Server 2 commands:\n"
            "server_help - get server commands\n"
            "execution_time - server2 process execution time\n"
            "screen_resolution - get resolution of system sreen"; 
        } else if(command == "execution_time") {
            msg = execution_time(start_time);
        } else if(command == "screen_resolution") {
            msg = screen_resolution();
        } else {
            msg = "Invalid command"; 
        }

        send(fd, msg.c_str(), strlen(msg.c_str()), 0);
        cout << "Message for client:" << endl << msg << endl;  
    }

    close(server2_socket);
    return 0;
}