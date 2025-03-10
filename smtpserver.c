#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
void check(char str[],int value){
	if(value<0)
		printf("%s FAILED\n",str);
}
int main(){
	
	int sid,sbind,slisten,saccept,srec,ssend,size,i,n=5;
	char recbuff[100],sendbuff[100],fromaddress[500],toaddress[500],mailbody[1000];
	sid=socket(AF_INET,SOCK_STREAM,0);
	check("SOCKET CREATON",sid);
	struct sockaddr_in saddr;
	saddr.sin_family=AF_INET;
	saddr.sin_port=htons(8082);
	saddr.sin_addr.s_addr=INADDR_ANY;
	size=sizeof(saddr);
	sbind=bind(sid,(struct sockaddr *)&saddr,size);
	check("BIND",sbind);
	slisten=listen(sid,5);
	check("LISTEN",slisten);
	saccept=accept(sid,(struct sockaddr *)&saddr,&size);
	check("ACCEPT",saccept);
	printf("......................................\n");
	srec=recv(saccept,recbuff,500,0);
	check("RECEIVE",srec);
	printf("MESSAGE FROM CLIENT :%s\n",recbuff);
	printf("SENDING RESPONSE FOR HI\n");
	strcpy(sendbuff,"220 192.168.9.138");
	ssend=send(saccept,sendbuff,500,0);
	check("SEND",ssend);
	printf("WAITING FOR RESPONSE FROM CLIENT\n");
	srec=recv(saccept,recbuff,500,0);
	check("RECEIVE",srec);
	if(strncmp(recbuff,"HELLO",5))
		printf("ERROR\n");
	else
		printf("MESSAGE FROM CLIENT : %s\n",recbuff);
	printf("SENDING RESPONSE FOR HELLO\n");
	strcpy(sendbuff,"250 OK");
	ssend=send(saccept,sendbuff,500,0);
	check("SEND",ssend);
	printf("WAITING FOR FROM ADDRESS\n");
	srec=recv(saccept,fromaddress,500,0);
	check("RECEIVE",srec);
	if(strncmp(fromaddress,"MAIL FROM:",10))
		printf("From address error\n");
	else
		printf("FROM ADDRESS: %s\n",fromaddress);
	printf("SENDING OK TO FROM ADDRESS\n");
	strcpy(sendbuff,"250 OK");
	ssend=send(saccept,sendbuff,500,0);
	check("SEND",ssend);
	printf("WAITING FOR TO ADDRESS\n");
	srec=recv(saccept,toaddress,500,0);
	check("RECEIVE",srec);
	if(strncmp(toaddress,"MAIL TO:",8))
		printf("To address error\n");
	else
		printf("TO ADDRESS: %s\n",toaddress);
	printf("SENDING OK TO TO ADDRESS\n");
	strcpy(sendbuff,"250 OK");
	ssend=send(saccept,sendbuff,500,0);
	check("SEND",ssend);
	printf("WAITING FOR DATA\n");
	srec=recv(saccept,fromaddress,500,0);
	check("RECEIVE",srec);
	if(strncmp(recbuff,"DATA",4))
		printf("DATA NOT RECEIVED\n");
	else
		printf("MESSAGE FROM CLIENT%s\n",recbuff);
	printf("SENDING OK TO CLIENT\n");
	strcpy(sendbuff,"354 GO AHEAD");
	ssend=send(saccept,sendbuff,500,0);
	check("SENDING",ssend);
	printf("MAIL BODY:\n");
	for(i=0;;i++){
		srec=recv(saccept,mailbody,1000,0);
		check("MAIL BODY RECEIVE",srec);
		if(strncmp(mailbody,"$",1)==0)
			break;
		else
			printf("%s",mailbody);
	}
	strcpy(sendbuff,"221 OK");
	ssend=send(saccept,sendbuff,500,0);
	printf("\nSENDING OK TO CLIENT\n");
	check("SENDING",ssend);
	srec=recv(saccept,mailbody,100,0);
	if(strncmp(mailbody,"QUIT",4)==0){
		strcpy(sendbuff,"221 OK");
		ssend=send(saccept,sendbuff,500,0);
		printf("Sending 221 ok...");
	}
	printf("CONNECTION CLOSED\n");
	close(saccept);
	close(sid);
	return 0;
}
