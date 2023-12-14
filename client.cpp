#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

#include "wrapped_funcs.h"

#define PORT_1 8877
#define PORT_2 8878

using namespace std;

int main() {
    int sockets[2];
    int ports[2] = { PORT_1, PORT_2 };
    int choosen_server = -1;

    ConnectClient(&sockets[0], PORT_1);
    ConnectClient(&sockets[1], PORT_2);

    char buf[256];
    string command;

    while (1) {
        if (choosen_server == -1) {
            cout << "Choose the server: 1 or 2" << endl;
            cin >> command;

            if (command == "1") {
                choosen_server = 1;
            } else if (command == "2") {
                choosen_server = 2;
            } else {
                cout << "Wrong server. Only 1 and 2 available" << endl;
            }
        } else {
            cout << "Enter command (type 'help' for getting help)" << endl;
            cin >> command;

            if (command == "exit") {
                const char *send_com = command.c_str();
                memset(buf, 0, 256);
                send(sockets[0], send_com, strlen(send_com), 0);
                send(sockets[1], send_com, strlen(send_com), 0);

                break;
            }

            if (command == "change") {
                choosen_server = -1;
            } else if (command == "help") {
                cout << "Available commands:" << endl 
                    << "help - get client commands" << endl
                    << "server_help - get help for server commands" << endl
                    << "change - change server" << endl;
            } else {
                const char *send_com = command.c_str();
                memset(buf, 0, 256);
                send(sockets[choosen_server - 1], send_com, strlen(send_com), 0);
                recv(sockets[choosen_server - 1], buf, sizeof(buf), 0);

                cout << buf << endl;
            }
        }
    }

    cout << "Client exit" << endl;
    return 0;
}