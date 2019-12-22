// Client side C/C++ program to demonstrate Socket programming
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

using namespace std;
#define all(v) (v).begin(), (v).end()
#define lp(i, n) for (int i = 0; i < (n) ; ++i)
#define lpp(i, n) for (int i = 1; i <= (n) ; ++i)
#define lpi(i, j, n)    for(int i=(j);i<(int)(n);++i)
#define lpd(i, j, n)    for(int i=(j);i>=(int)(n);--i)
#define sz(v) (int)(v).size()
#define clr(v, d)       memset(v, d, sizeof(v))
#define mod 100000007
#define PORT 8000

int cID , msgID;
int makeRequest();

 
void lock(){
	char type[] = "1:";
	char buffer[1024] = {0};
	int sock = makeRequest();
	int ready = 0 ; 
	while(ready==0){
	send(sock , type , strlen(type) , 0 );
    read( sock , buffer, 1024);
	ready=atoi(buffer);
	}
	cout << "Locked Successfully "<<endl;
}

void unlock(){
		char type[] = "2:";
	char buffer[1024] = {0};
	int sock = makeRequest();
	int ready = 0 ; 
	while(ready==0){
	send(sock , type , strlen(type) , 0 );
    read( sock , buffer, 1024);
	ready=atoi(buffer);
	}
	cout << "Unlocked Successfully "<<endl;
}

void read(){
	char type[] = "3:";
	char buffer[1024];
	buffer[0]='0';
	int sock = makeRequest();
	int ready = 0 ; 
	send(sock , type , strlen(type) , 0 );
    read( sock , buffer, 1024);
	ready=atoi(buffer);
	if(buffer[0]=='0'){
		cout << "You must lock before read "<< endl;
		return ;
		}
	cout << buffer << endl;
	cout << "Read Successfully "<<endl;
	
}

void write(){
	char type[] = "4:";
	char buffer[1024];
	buffer[0]='0';
	int sock = makeRequest();
	int ready = 0 ; 
	send(sock , type , strlen(type) , 0 );
    read( sock , buffer, 1024);
	ready=atoi(buffer);
	if(buffer[0]=='0'){
		cout << "You must lock before read "<< endl;
		return ;
		}
	cout << "Print the msg you want to sent "<<endl;
	string x ; 
	cin >> x ; 
	lp(i,x.size()){buffer[i]=x[i];buffer[i+1]='\0';}
	send(sock , buffer , strlen(buffer) , 0 );
    read( sock , buffer, 1024);
    if(buffer[0]=='1'){
	cout << "written Successfully "<<endl;
		}else{
			cout << "Failed "<<endl;
			}

	
	
	
	
	
	
}




int makeRequest() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char hello[1100];
	string helloShakehand = std::to_string(cID)+":"+std::to_string(msgID)+":";
	lp(i,helloShakehand.size()){hello[i]=helloShakehand[i];hello[i+1]='\0';}
    cout << helloShakehand<<endl;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    read( sock , buffer, 1024);
    if (atoi(buffer) != 1) {
        cout << "error in connection" << endl;
        return 0 ; 
    } else {
        cout << "Connected ..." << endl;
        return sock ; 
    }
    return sock ;
}



int main()
{
    while (1) {
        readId :
        cout << "Please Enter Client ID : ";
        cin >> cID ;
        readMID:
        cout << endl << "Please Enter sheard Memory you want to access : ";
        cin >> msgID ;
        while (1) {
            cout << endl << "Enter type of you Request : " << endl;
            cout << "1- Lock " << endl;
            cout << "2- unLock " << endl;
            cout << "3- Read " << endl;
            cout << "4- Write " << endl;
            cout << "5- Change msg id " << endl;
            cout << "6- Change Client session " << endl;
            int type ; cin >> type ;
            if (type == 1) {
				lock();
            } else if (type == 2 ) {
				unlock();
            } else if (type == 3) {
				read();
            } else if (type == 4 ) {
				write();
            } else if(type == 5){
				goto readMID;
            }else if (type == 6 ){
				goto readId;
			}else{
				 cout << "These option not implemeted yet ." << endl;
				}
        }
    }
    return 0;
}
