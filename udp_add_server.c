#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

#define port 9002
#define BUF 256

int main()
{
	int server_socket;
	struct sockaddr_in server_addr,client_addr;
	int num1,num2,sum;
	server_socket=socket(AF_INET,SOCK_DGRAM,0);
	if(server_socket<0)
		perror("ERROR opening socket");
	
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=INADDR_ANY;
	
	if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
		perror("ERROR binding to server");
		
	int addr_size=sizeof(client_addr);
	int bytes_recieved=recvfrom(server_socket,&num1,sizeof(num1),0,(struct sockaddr*)&client_addr,&addr_size);//recieve first no. from client
	printf("Recieved number: %d\n",num1);
	int bytes_recieved2=recvfrom(server_socket,&num2,sizeof(num2),0,(struct sockaddr*)&client_addr,&addr_size);
	printf("Recieved number: %d\n",num2);
	sum=num1+num2;   //calcutate sum
	printf("Sum: %d\n",sum);
	sendto(server_socket,&sum,sizeof(sum),0,(struct sockaddr*)&client_addr,addr_size);  //return sum
	
	close(server_socket);
	
	return 0;

}
	
