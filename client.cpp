// Client side C/C++ program to demonstrate Socket programming 
#include <bits/stdc++.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
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


int main() 
{ 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    char hello[] = "9:3:"; 
    char type[] = "1:"; 

    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
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
	if(atoi(buffer)!=1){
		cout << "error" << endl;
	}else{
	cout << "yes"<<endl;	
	}
	send(sock , type , strlen(type) , 0 ); 
	read( sock , buffer, 1024); 
	printf("%s\n",buffer ); 
    return 0; 
} 
