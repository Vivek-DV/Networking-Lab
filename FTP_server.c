#include<stdio.h>
#include<sys/types.h> 
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

int main(int argc , char * argv[]){
	int serv_sockfd,portno,client,newsockfd,n;
	char command[10]={0} , fileName[10]={0} , buffer[1024]={0};
	portno=atoi(argv[1]);
	struct sockaddr_in serv_addr,client_addr;
	serv_sockfd =socket(AF_INET, SOCK_STREAM, 0);
	
	if(serv_sockfd<0)
		printf("Error\n");
		
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port =htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	bind(serv_sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	listen(serv_sockfd,5);
	client=sizeof(client_addr);
	newsockfd=accept(serv_sockfd,(struct sockaddr*)&client_addr,&client);
	n=read(newsockfd,command,sizeof(command));
	if(n < 0)
		printf("Error\n");
	n=read(newsockfd,fileName,sizeof(fileName));
	if(n < 0)
		printf("Error\n");
	
	FILE *fp; 
	if(!strcmp(command,"get")){ 
		fp=fopen(fileName,"r");
		fgets(buffer, sizeof(buffer),fp);
		n=write(newsockfd,buffer,1023);
		if(n < 0)
			printf("Error\n");
		fclose(fp);
		printf("Put file is written into newFileGet.txt\n");
	}else if(!strcmp(command,"put")){
		fp=fopen("newFilePut.txt","w");
		n=read(newsockfd,buffer,1023);
		if(n < 0)
			printf("Error\n");
		fprintf(fp,"%s",buffer);
		fclose(fp);
		printf("Put file is written into newFilePut.txt\n");
	}else{
		printf("Wrong command!\n");
		exit(0);	
	}
}
