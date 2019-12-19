// Server side C/C++ program to demonstrate Socket programming
//#include <bits/stdc++.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#define all(v) (v).begin(), (v).end()
#define lp(i, n) for (int i = 0; i < (n) ; ++i)
#define lpp(i, n) for (int i = 1; i <= (n) ; ++i)
#define lpi(i, j, n)    for(int i=(j);i<(int)(n);++i)
#define lpd(i, j, n)    for(int i=(j);i>=(int)(n);--i)
#define sz(v) (int)(v).size()
#define clr(v, d)       memset(v, d, sizeof(v))
#define mod 100000007

using namespace std;
#define PORT 8000


// DataStucture for Sheard Memoreis

typedef struct {
    // to store the contant as string
    string memo ;
    // to know client make lock
    int lockedBy ;
    // dynamic memory to store clients sheards this memory
    set<int> sheardBy;
} memory ;
// dynamic array [ key - > value ] to store the memores
map<int, memory>memories ;

// End




void requestHandler(void* data) {
    int new_socket = *((int*)data) ;
    char buffer[1024] = {0};
	char success[] ="1:";
	char failed[] ="0:";
	
    // Get User Id And Sheard Memory Id ..
    read( new_socket , buffer, 1024);
    int id = atoi(buffer);
    int  i  ;
    for ( i = 0; i < (int)strlen(buffer); ++i) {
        if (buffer[i] == ':') {
            i++;
            break;
        }
    }
    int memoId = atoi(buffer + i);
    cout << id << " ID " << memoId << " request type " << endl;
	send(new_socket , success , strlen(success) , 0 ); 
	
	/*
     * The client sends the shared memory key (int) to server, if memory already exist
    the client will be added to that memory list of clients, otherwise the memory is
    allocated first
     * */
    if (memories.count(memoId) == 0) { // Memory Does Not Exist
        memory a  ;
        a.memo = "Empty";
        a.lockedBy = -1 ;
        a.sheardBy.insert(id);
        memories[memoId]=a;
    } else {
        memories[memoId].sheardBy.insert(id) ;
    }
	
    //Get Msg Type { 1 , 2  ,3,  4 ,5 , 6  } .
    read( new_socket , buffer, 1024);
    int msgType = atoi(buffer);
    cout << "msg Type is " << msgType << endl;
	//send(new_socket , success , strlen(success) , 0 ); 
	
    if (msgType == 1) { // LOCK REQUEST 
		
		if(memories[memoId].lockedBy == -1||memories[memoId].lockedBy == id){
			cout << "Locked by " << id << endl;
			memories[memoId].lockedBy = id ; 
			send(new_socket , success , strlen(success) , 0 ); 
		}else{
			cout << "Fail Locked " << id << endl;
			send(new_socket , failed , strlen(failed) , 0 ); 
		}
		return ;
    } else if (msgType == 2) {
		if(memories[memoId].lockedBy == id){
			cout << "Success UNLOCKED " << id << endl;
			memories[memoId].lockedBy = -1 ; 
			send(new_socket , success , strlen(success) , 0 ); 
		}else{
			cout << "Fail Unlocked " << id << endl;
			send(new_socket , failed , strlen(failed) , 0 ); 
		}
		return ;
		
    } else if (msgType == 3) { // Read  
		
		if(memories[memoId].lockedBy == id){
			char res[1000] ; 
			lp(i,memories[memoId].memo.size()){res[i]=memories[memoId].memo[i];res[i+1]='\0';}
			send(new_socket , res , strlen(res) , 0 ); 
		}else{
			cout << "Fail Read by " << id << endl;
			send(new_socket , failed , strlen(failed) , 0 ); 
		}
		return ;
		
    } else if (msgType == 4) { // Write 
		
		if(memories[memoId].lockedBy == id){
			send(new_socket , success , strlen(success) , 0 );    
			//read( new_socket , buffer, 1024);
			char res[1024] ; 
			read( new_socket , res, 1024);
			string s(res);
			memories[memoId].memo=s;
			cout << s << endl ;
			send(new_socket , success , strlen(success) , 0 ); 
		}else{
			cout << "Fail Write by " << id << endl;
			send(new_socket , failed , strlen(failed) , 0 ); 
		}
		return ;
    } else if (msgType == 5) {
    } else if (msgType == 6) {
    }


   


}

int main()
{
    int server_fd, new_socket ;
    struct sockaddr_in address, peer_addr;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)

    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&peer_addr,
                                 (socklen_t*)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // Get IP Address for client
        //
        pthread_t  p_thread;
        pthread_create(&p_thread, NULL, reinterpret_cast<void *(*)(void *)>(requestHandler), (void*)&new_socket);
    }
    return 0;
}


