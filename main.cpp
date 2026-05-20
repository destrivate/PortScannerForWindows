#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

using namespace std;


bool checkPort(string ip,int port,int protocol){
    SOCKET clientSocket = socket(AF_INET,SOCK_STREAM,protocol);
    sockaddr_in sAdr;
    sAdr.sin_family = AF_INET;
    sAdr.sin_port = htons(port);
    inet_pton(AF_INET,ip.c_str(),&sAdr.sin_addr);
    int connectStatus = connect(clientSocket,(sockaddr*)&sAdr,sizeof(sAdr));
    closesocket(clientSocket);
    if(connectStatus == 0){
        return true;
    }
    return false;
}

namespace data{
    int protocols[] = {IPPROTO_TCP,IPPROTO_UDP};
    string ip = "";
    int Sport = 0;
    int Kport = 0;
    int protocolType = 0;
    void inputData(){
        cout << "Target IP: " << endl;
        cin >> ip;
        cout << "Port range from: " << endl;
        cin >> Sport;
        cout << "Port range up to: " << endl;
        cin >> Kport;
        cout << "Protocol - " << "1.TCP" << " " << "2.UDP" << endl;
        int protocolNumber = 0;
        cin >> protocolNumber;
        if(protocolNumber > 2 || protocolNumber == 0){
            system("cls");
            inputData();
        }
        protocolType = protocols[protocolNumber-1];
        cout << "__________________" << endl;
        if(Kport == 0){
            system("cls");
            inputData();
        }
    }
}

int main() {
    data::inputData();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return 1;

    const int total_ports = data::Kport - data::Sport;
    thread threads[total_ports];

    for (int i = data::Sport; i < data::Kport; i++) {
        int index = i - data::Sport; 
        threads[index] = thread([i]() {
            if (checkPort(data::ip, i, data::protocolType)) {
                printf("Open: %d\n", i);
            }
        });
    }

    for (int i = 0; i < total_ports; i++) {
        threads[i].join();
    }

    WSACleanup();
    return 0;
}
