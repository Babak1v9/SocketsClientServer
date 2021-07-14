#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>

#define BUF 1024

//encrypt decrypt by same operation
//+32 .. +126 whole ascii range = 95 -> 95/2 = 47,5 -> remove space
std::string encrypt(const std::string &message) {
    std::string result = "";
    for (unsigned char charToRot47 : message) {
        if (charToRot47 < 33 || 126 < charToRot47) {
        }
        charToRot47 += 47;
        if (charToRot47 > 126) charToRot47 -= 94;
        result += charToRot47;
    }
    return result;
}

void endOfCmd() {
    std::cout << "#############" << std::endl;
    std::cout << "End of command" << std::endl;
    std::cout << "#############" << std::endl;
}

void clientResponse(char *buffer, int create_socket, int size) {
    if (size > 0) {
        buffer[size]= '\0';
        printf ("%s", buffer);
        fgets (buffer, BUF, stdin);
        send(create_socket, buffer, strlen (buffer), 0);
    }
}

int main (int argc, char **argv) {

    char buffer[BUF];
    int create_socket;
    struct sockaddr_in address{};
    int size;
    char* serverPort;
    int notMyFirstTime = 0;

    if( argc < 3 ){
        printf("Usage: %s ServerAdresse\n", argv[0]);
        exit(EXIT_FAILURE);
    } else {
        serverPort = argv[2];
    }
    auto port = (unsigned short) strtoul(serverPort, nullptr, 0);

    if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket error");
        return EXIT_FAILURE;
    }

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons (port);
    inet_aton (argv[1], &address.sin_addr);

    if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0) {
        printf ("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
        size=recv(create_socket,buffer,BUF-1, 0);
        if (size>0) {
            buffer[size]= '\0';
            printf("%s",buffer);
        }
    }
    else {
        perror("Connect error - no server available");
        return EXIT_FAILURE;
    }

    do {
        if(notMyFirstTime > 0) {
            size=recv(create_socket,buffer,BUF-1, 0);
            if (size>0) {
                buffer[size]= '\0';
                printf ("%s", buffer);
            }
        }
        fgets (buffer, BUF, stdin);
        send(create_socket, buffer, strlen (buffer), 0);

        if (strncmp(buffer, "send", 4) == 0) {
            //get sender recipient subject
            for (int i = 0; i < 3; i++) {
                size=recv(create_socket,buffer,BUF-1, 0);
                clientResponse(buffer, create_socket, size);
            }
            //get encrypted message
            size=recv(create_socket,buffer,BUF-1, 0);
            if (size>0) {
                std::stringstream stream;
                buffer[size]= '\0';
                printf("%s",buffer);
                fgets (buffer, BUF, stdin);
                std::string tmp = buffer;
                //ENCRYPT ROT13
                std::string encryptedMessage = encrypt(tmp);
                std::cout << "Encrypted Message sent: " << encryptedMessage << std::endl;
                stream << encryptedMessage << '\n';

                for (unsigned int i = 0; i < BUF; i++){
                    buffer[i] = stream.str()[i];
                    if (i >= stream.str().size())
                        break;
                }
                send(create_socket, buffer, strlen (buffer), 0);
            }

            //receive ok/err from server
            size = recv (create_socket, buffer, BUF-1, 0);
            if( size > 0) {
                buffer[size] = '\0';
                printf ("Server Status: %s\n", buffer);
            }
            notMyFirstTime++;
            endOfCmd();

        } else if (strncmp(buffer, "list", 4) == 0) {

            //send user to server
            size=recv(create_socket,buffer,BUF-1, 0);
            clientResponse(buffer, create_socket, size);

            //get message count + messages
            size=recv(create_socket,buffer,BUF-1, 0);
            if (size>0) {
                buffer[size] = '\0';
                printf("%s", buffer);
            }
            notMyFirstTime++;
            endOfCmd();

        } else if (strncmp(buffer, "read", 4) == 0) {
            //User Input User + Num
            for (int i = 0; i < 2; i++) {
                size = recv(create_socket, buffer, BUF - 1, 0);
                clientResponse(buffer, create_socket, size);
            }
            //OK or ERR
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                printf("Server Response: %s", buffer);
            }

            if (strncmp(buffer, "ERR", 3) == 0) {
                std::cerr << "Username or Number not correct";
                exit(1);   // call system to stop
            } else {
                //Output of msg
                size = recv(create_socket, buffer, BUF - 1, 0);
                if (size > 0) {
                    buffer[size] = '\0';
                    std::string tmp = buffer;
                    std::string tmpStr = encrypt(tmp);
                    std::string decryptedMessage = tmpStr.substr(0, tmpStr.size()-2);
                    std::cout << "Decrypted Message: " << decryptedMessage;
                    printf("\n");
                }
            }
            endOfCmd();
            notMyFirstTime++;
        } else if (strncmp(buffer, "del", 3) == 0) {

            //User Input User + Num
            for (int i = 0; i < 2; i++) {
                size = recv(create_socket, buffer, BUF - 1, 0);
                clientResponse(buffer, create_socket, size);
            }
            //OK or ERR
            size = recv(create_socket, buffer, BUF - 1, 0);
            if (size > 0) {
                buffer[size] = '\0';
                printf("%s", buffer);
            }

            if (strncmp(buffer, "ERR", 3) == 0) {
                std::cerr << "Username or Number not correct";
                exit(1);   // call system to stop
            } else {
                //Output of msg
                size = recv(create_socket, buffer, BUF - 1, 0);
                if (size > 0) {
                    buffer[size] = '\0';
                    printf("%s", buffer);
                    printf("\n");
                }
            }
            notMyFirstTime++;
            endOfCmd();
        }
    } while (strcmp (buffer, "quit\n") != 0);
    //goodbye
    size=recv(create_socket,buffer,BUF-1, 0);
    if (size>0) {
        buffer[size]= '\0';
        printf("%s",buffer);
    }

    close (create_socket);
    return EXIT_SUCCESS;
}
