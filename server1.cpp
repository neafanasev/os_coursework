#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <bits/stdc++.h>
#include <fstream>

#include "wrapped_funcs.h"

#define PORT 8877

using namespace std;

vector<string> smart_split(string str, char separator) {
    int start_index = 0, end_index = 0, str_len = str.size();
    vector<string> strings;

    for (int i = 0; i <= str_len; i++) {
        
        if (str[i] == separator|| i == str_len) {
            end_index = i;
            string temp;
            temp.append(str, start_index, end_index - start_index);
            if (temp != " ") {
                strings.push_back(temp);
            }
            start_index = end_index + 1;
        }
    }

    return strings;
}

string replace(string str, char to_replace, char to_insert) {
    string new_str;

    for (int i = 0; i < str.size(); i++) {
        if (str[i] == to_replace) {
            new_str += to_insert;
        } else {
            new_str += str[i];
        }
    }

    return new_str;
}

string parse_swapon(int command) {
    system("swapon --show > bin/swapon");
    ifstream file ("bin/swapon");

    string line;
    if (file.is_open()) {
        getline(file, line);
        getline(file, line);
    }

    file.close();
    system("rm bin/swapon");

    vector<string> strings = smart_split(line, ' ');

    if (command == 1) {
        return replace(strings[4], ',', '.');
    } else {
        return replace(strings[5], ',', '.');
    }
}

int get_mul_factor(char size_char) {
    if (size_char == 'G') {
        return 1024*1024*1024;
    } else if (size_char = 'M') {
        return 1024*1024;
    } else if (size_char = 'K') {
        return 1024;
    } else {
        return 1;
    }
}

float convert_to_bytes(string str) {
    char size_char = str[str.size() - 1];

    int mul_factor = get_mul_factor(size_char);

    float num = (float) mul_factor * stod(replace(str, size_char, ' '));

    return num;
}

string swap_total() {
    return to_string(convert_to_bytes(parse_swapon(1))) + " bytes";
}

string swap_free() {
    string total = parse_swapon(1);
    string used = parse_swapon(0);
    return to_string(convert_to_bytes(total) - convert_to_bytes(used)) + " bytes";
}

int main() {
    int server1_socket = Socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = { 0 };
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t adrlen = sizeof(addr);

    Bind(server1_socket, (struct sockaddr *) &addr, adrlen);

    Listen(server1_socket, 1);

    int fd = Accept(server1_socket, NULL, NULL);

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

        if (command == "shelp") {
            msg = "Server 1 commands:\n"
            "shelp - get server commands (you're here)\n"
            "swap_total - get total size of swap file \n"
            "swap_free - get aviable number of bytes of swap file"; 
        } else if (command == "swap_total") {
            msg = swap_total();
        } else if (command == "swap_free") {
            msg = swap_free();
        } else {
            msg = "Invalid command, try shelp"; 
        }

        send(fd, msg.c_str(), strlen(msg.c_str()), 0);
        cout << "Message for client:" << endl << msg << endl;  
    }

    close(server1_socket);
    return 0;
}