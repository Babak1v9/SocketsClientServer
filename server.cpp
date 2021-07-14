#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream> // write to file
#include <dirent.h>
#include <filesystem>
#include <vector>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>
#include <thread>
#define BUF 1024

void wrongInput(int new_socket, char *buffer) {
    strcpy(buffer,"Invalid Input\n");
    send(new_socket, buffer, strlen(buffer),0);
    exit(1);
}

int countMsgInDir(DIR *dir, dirent *ent, std::string &file, const std::string &userPath, int counter) {
    if ((dir = opendir(userPath.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            file = ent->d_name;
            if (file.find("Subject") != std::string::npos) {
                counter++;
            }
        } closedir(dir);
    }
    return counter;
}

void sendStream(char *buffer, const std::stringstream &stream) {
    for (unsigned int i = 0; i < BUF; i++){
        buffer[i] = stream.str()[i];
        if (i >= stream.str().size())
            break;
    }
}

void endOfCommand() {
    std::cout << "#############" << std::endl;
    std::cout << "End of Command" << std::endl;
    std::cout << "#############" << std::endl;
}

//handle new requests
void newClient (int new_socket, const char* mailSpoolDir) {

    char buffer[BUF];
    int size;
    std::string mailDir(mailSpoolDir);

    do {

        strcpy(buffer,"Welcome to myserver, Please enter your command:\n");
        send(new_socket, buffer, strlen(buffer),0);

        size = recv (new_socket, buffer, BUF-1, 0);
        if( size > 0)
        {
            buffer[size] = '\0';
            printf ("Message received: %s\n", buffer);

            char sender[9] = "";
            char recipient[9] = "";
            char subject[80] = "";
            char message[1024] = "";
            std::stringstream stream;

            //START of SEND command
            if (strncmp(buffer, "send", 4) == 0) {

                int recipientLen;
                int subjectLen;
                int senderLen;
                struct stat st = {0};
                std::string recipientStr;
                std::string subjectStr;
                std::string messageStr;

                if (stat(mailSpoolDir, &st) == -1) { //create mail directory if not existing
                    if (mkdir(mailSpoolDir, 0777) != 0) {
                        std::cout << "Could not create Dir!" << std::endl;
                        exit(1);
                    }
                }
                //get sender from client
                strcpy(buffer,"Please enter a Sender: ");
                send(new_socket, buffer, strlen(buffer),0);

                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0 && size <= 9) {
                    senderLen = size;
                    buffer[size] = '\0';
                    printf ("Sender received: %s\n", buffer);
                    int i = 0;
                    while (buffer[i] != '\n') {
                            sender[i] = buffer[i];
                            i++;
                    }
                } else wrongInput(new_socket, buffer);

                //get recipient from client
                strcpy(buffer,"Please enter a Recipient: ");
                send(new_socket, buffer, strlen(buffer),0);
                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0 && size <= 9) {
                    recipientLen = size;
                    buffer[size] = '\0';
                    printf ("Recipient received: %s\n", buffer);
                    int i = 0;
                    while (buffer[i] != '\n') {
                        recipient[i] = buffer[i];
                        i++;
                    }
                    recipientStr = recipient;
                } else wrongInput(new_socket, buffer);

                //subject
                strcpy(buffer,"Please enter a Subject: ");
                send(new_socket, buffer, strlen(buffer),0);

                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0 && size <= 81) {
                    subjectLen = size;
                    buffer[size] = '\0';
                    printf ("Subject received: %s\n", buffer);
                    int i = 0;
                    while (buffer[i] != '\n') {
                        subject[i] = buffer[i];
                        i++;
                    }
                } else wrongInput(new_socket, buffer);

                //message
                strcpy(buffer,"Please enter a Message: ");
                send(new_socket, buffer, strlen(buffer),0);

                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0) {
                    buffer[size] = '\0';
                    printf ("Message received: %s\n", buffer);
                    int i = 0;
                    while (buffer[i] != '\n') {
                        message[i] = buffer[i];
                        i++;
                    }
                } else wrongInput(new_socket, buffer);

                // continue if input is correct
                if (recipientLen <= 9 && senderLen <= 9 && subjectLen <= 81) {
                    strcpy(buffer,"OK");
                    send(new_socket, buffer, strlen(buffer),0);

                    std::string recipientPath = mailDir + recipientStr + "/";

                    if (stat(recipientPath.c_str(), &st) == -1) {
                        if (mkdir(recipientPath.c_str(), 0777) != 0) {
                            exit(1);
                        }
                    }
                    subjectStr = subject;

                    //generates uuid
                    boost::uuids::uuid uuid = boost::uuids::random_generator()();
                    std::string file = "Subject:" + subjectStr + "_Sender:_" + sender + "_" + to_string(uuid) + ".txt";
                    std::cout << file;
                    std::string filePath = recipientPath + file;

                    //write to file
                    std::ofstream myFile(filePath);
                    if (myFile.is_open()) {
                        myFile << message << "\n";
                        myFile.close();
                    } else {
                        printf("Unable to open file");
                    }
                    // input invalid
                }else {
                    strcpy(buffer,"ERR");
                    send(new_socket, buffer, strlen(buffer),0);
                }
                endOfCommand();
                //END of SEND command
                //START of LIST command
            } else if (strncmp(buffer, "list", 4) == 0) {

                std::string username;
                char user[9] = "";

                strcpy(buffer,"Please enter a User: ");
                send(new_socket, buffer, strlen(buffer),0);
                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0 && size <=9) {
                    buffer[size] = '\0';
                    for (int i = 0; i < 8; i++) {
                        if (buffer[i] == '\n') {
                            break;
                        }
                        user[i] = buffer[i];
                    }
                    username = user;
                    std::cout << "Username: " << username << std::endl;
                } else wrongInput(new_socket, buffer);

                std::string file;
                std::string filename;

                DIR* dir;
                struct dirent* ent;
                std::string userPath = mailDir + username;
                std::cout << "userPath: " << userPath << std::endl;

                if ((dir = opendir(userPath.c_str())) != nullptr) {
                    int counter = 0;
                    while ((ent = readdir(dir)) != nullptr) {
                        file = ent->d_name;
                        if (file.find("Subject") != std::string::npos) {
                            counter++;
                        }
                    }
                    closedir(dir);

                    if (counter) {
                        stream << "Message count: " << counter << '\n';
                    }else {
                        stream << "No messages found." << '\n';
                    }

                    counter = 0;
                    dir = opendir(userPath.c_str());

                    while ((ent = readdir(dir)) != nullptr){
                        file = ent->d_name;

                        if (file.find("Subject") != std::string::npos) {                  //npos == "Subj" was not found in the string
                            filename = file; //.substr(4, (file.find("___") - 4));
                            counter++;
                            std::cout << ent->d_name << std::endl;
                            stream << "" << counter << ". " << filename << '\n';
                        }
                    }
                    sendStream(buffer, stream);
                    closedir(dir);
                }
                else{
                    stream << "User has not received any messages yet." << '\n';
                    sendStream(buffer, stream);
                }
                send(new_socket, buffer, BUF, 0);
                std::cout << "End of List Command" << std::endl;

                endOfCommand();
                //END of LIST command
                //START of READ command
            } else if (strncmp(buffer, "read",4) == 0) {
                strcpy(buffer,"Message received: read\n");
                send(new_socket, buffer, strlen(buffer),0);

                char user[9] ="";
                std::string temp;
                std::string username;

                strcpy(buffer,"Please enter a User (max. 8 symbols): ");
                send(new_socket, buffer, strlen(buffer),0);
                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0 && size <= 9) {
                    buffer[size] = '\0';
                    for (int i = 0; i < 8; i++) {
                        if (buffer[i] == '\n') {
                            break;
                        }
                        user[i] = buffer[i];
                    }
                    username = user;
                } else wrongInput(new_socket, buffer);

                strcpy(buffer,"Please enter a Number: ");
                send(new_socket, buffer, strlen(buffer),0);

                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0) {
                    buffer[size] = '\0';
                    temp = buffer;
                }

                int desiredMsg = std::stoi(temp);
                if (username.length() == 8 && desiredMsg > 0) {
                    strcpy(buffer,"OK\n");
                    send(new_socket, buffer, strlen(buffer),0);

                    DIR* dir;
                    struct dirent* ent;
                    std::string file;
                    std::string userPath = mailDir + username;
                    std::cout << "userPath: " << userPath << std::endl;
                    int counter = 0;

                    //counts all messages for the user
                    //d_name = filename
                    //npos = if string not found
                    if ((dir = opendir(userPath.c_str())) != nullptr) {
                        while ((ent = readdir(dir)) != nullptr) {
                            file = ent->d_name;
                            if (file.find("Subject") != std::string::npos) {
                                counter++;
                            }
                        } closedir(dir);
                    }
                    //only continues if msg id exists
                    if ((dir = opendir(userPath.c_str())) != nullptr) {
                        if (desiredMsg <= counter) {
                            int tempCount = 0;
                            while ((ent = readdir(dir)) != nullptr) {
                                file = ent->d_name;

                                if (file.find("Subject") != std::string::npos) {
                                    tempCount++;

                                    if (tempCount == desiredMsg) {
                                        std::string filepath;
                                        filepath.append(userPath);
                                        filepath.append("/");
                                        filepath.append(file);

                                        std::ifstream myFile(filepath);
                                        //check if file open
                                        if (!myFile) {
                                            std::cerr << "Unable to open file";
                                            exit(1);   // call system to stop
                                        }
                                        if (myFile.is_open()) {
                                            stream << myFile.rdbuf();
                                            myFile.close();
                                        }
                                        break;
                                    }
                                }
                            }
                        } else {
                            printf("Message not found\n");
                            strcpy(buffer,"Message not found\n");
                            send(new_socket, buffer, strlen(buffer),0);
                            exit(1);
                        }

                        //store stream into buffer again
                        sendStream(buffer, stream);

                        //send message to client & close dir again
                        send(new_socket, buffer, BUF, 0);
                        closedir(dir);
                    }
                }
                else {
                    strcpy(buffer,"ERR");
                    send(new_socket, buffer, strlen(buffer),0);
                }

                endOfCommand();
                //END of READ command
                //START of DEL command
            } else if (strncmp(buffer, "del", 3) == 0) {
                strcpy(buffer,"Message received: del\n");
                send(new_socket, buffer, strlen(buffer),0);

                char user[9] ="";
                std::string temp;
                std::string username;

                strcpy(buffer,"Please enter a User (max. 8 symbols): ");
                send(new_socket, buffer, strlen(buffer),0);
                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0 && size <= 9) {
                    buffer[size] = '\0';
                    for (int i = 0; i < 8; i++) {
                        if (buffer[i] == '\n') {
                            break;
                        }
                        user[i] = buffer[i];
                    }
                    username = user;
                } else wrongInput(new_socket, buffer);

                strcpy(buffer,"Please enter a Number: ");
                send(new_socket, buffer, strlen(buffer),0);

                size = recv (new_socket, buffer, BUF-1, 0);
                if( size > 0) {
                    buffer[size] = '\0';
                    temp = buffer;
                }

                int desiredMsg = std::stoi(temp);
                if (username.length() == 8 && desiredMsg > 0) {
                    strcpy(buffer,"OK\n");
                    send(new_socket, buffer, strlen(buffer),0);

                    DIR* dir;
                    struct dirent* ent;
                    std::string file;
                    std::string userPath = mailDir + username;
                    std::cout << "userPath: " << userPath << std::endl;
                    int counter = 0;

                    //counts all messages for the user
                    counter = countMsgInDir(dir, ent, file, userPath, counter);

                    //open directory
                    if ((dir = opendir(userPath.c_str())) != nullptr) {
                        //only continues if msg id exists
                        if (desiredMsg <= counter) {
                            int tempCount = 0;
                            while ((ent = readdir(dir)) != nullptr) {
                                file = ent->d_name;

                                if (file.find("Subject") != std::string::npos) {
                                    tempCount++;
                                    //checks if msg id == user input
                                    if (tempCount == desiredMsg) {
                                        std::string filepath;
                                        filepath.append(userPath);
                                        filepath.append("/");
                                        filepath.append(file);

                                        //remove file
                                        if (remove(filepath.c_str()) == 0)
                                        {
                                            stream << "File deleted.\n";
                                        } else {
                                            std::cout << "Error - File deletion not possibru!" << std::endl;
                                            stream << "Error - File deletion not possibru!\n";
                                        }
                                        break;
                                    }
                                }
                            }
                            //no message found
                        } else {
                            printf("Message not found\n");
                            strcpy(buffer,"Message not found\n");
                            send(new_socket, buffer, strlen(buffer),0);
                            exit(1);
                        }

                        //store stream into buffer again
                        sendStream(buffer, stream);

                        //send message to client & close dir again
                        send(new_socket, buffer, BUF, 0);
                        closedir(dir);
                    }
                }
                else {
                    strcpy(buffer,"ERR");
                    send(new_socket, buffer, strlen(buffer),0);
                }
                endOfCommand();
            }
            //END of DEL command
        }
        else if (size == 0) //no connection
        {
            printf("Client closed remote socket\n");
            break;
        }
        else
        {
            perror("recv error");
            exit(1);
        }

    } while (strncmp (buffer, "quit", 4)  != 0);
    printf("Goodbye!:) \n");
    strcpy(buffer,"Goodbye!:) \n");
    send(new_socket, buffer, strlen(buffer),0);
    close (new_socket);
}

int main (int argc, char **argv) {

    char *serverPort;
    char *mailSpoolDir;
    //storing cmd arguments into variable if provided
    if (argc == 3) {
        serverPort = argv[1];
        mailSpoolDir = argv[2];
        printf("Port: %s", serverPort);
        printf("\n");
        printf("Directory: %s", mailSpoolDir);
        printf("\n \n");

    } else {
        printf("Not enough Arguments: %i", argc);
        printf("\n \n");
        printf("\nUsage: 1.Argument: Port 2.Argument: Directory Path\n\n");
        return EXIT_FAILURE;
    }

    int create_socket, new_socket;
    socklen_t addrlen;
    struct sockaddr_in address{}, cliaddress{};
    auto port = (unsigned short) strtoul(serverPort, nullptr, 0); //auto instead of unsigned short to avoid duplicates

    if ((create_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {//create socket
        perror("Socket error");
        return EXIT_FAILURE;
    }
    //Network byte order -> big endian
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    std::vector<int> sockets;

    if (bind(create_socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
        perror("bind error");
        return EXIT_FAILURE;
    }
    sockets.push_back(create_socket);
    listen(create_socket, 5); //bind listening socket to address

    addrlen = sizeof(struct sockaddr_in);

    //endless loop
    while (true) {
        printf("Waiting for connections...\n");
        new_socket = accept(create_socket, (struct sockaddr *) &cliaddress, &addrlen);
        if (new_socket > 0) {
            printf("Client connected from %s:%d...\n", inet_ntoa(cliaddress.sin_addr), ntohs(cliaddress.sin_port));
            sockets.push_back(new_socket);
            std::thread(newClient, new_socket, mailSpoolDir).detach();
        }
    }

    close(create_socket);
    return EXIT_SUCCESS;
}