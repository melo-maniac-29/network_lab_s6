#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
void check(char str[],int value){
	if(value<0)
		printf("%s FAILED",str);
}
int main(){

	int cid,con,crec,csend,size,i;
	char recbuff[500],sendbuff[500],fromaddress[500],toaddress[500],mailbody[1000];
	struct sockaddr_in caddr;
	cid=socket(AF_INET,SOCK_STREAM,0);
	check("SOCKET CREATION",cid);
	caddr.sin_family=AF_INET;
	caddr.sin_port=htons(8082);
	caddr.sin_addr.s_addr=INADDR_ANY;
	size=sizeof(caddr);
	con=connect(cid,(struct sockaddr*)&caddr,size);
	check("CONNECTION",con);
	printf(".....................................\n");
	printf("SENDING HI TO SERVER\n");
	strcpy(sendbuff,"HI");
	csend=send(cid,sendbuff,500,0);
	check("SEND",csend);
	printf("WAITING FOR RESPONSE FROM SERVER\n");
	crec=recv(cid,recbuff,500,0);
	check("RECEIVE",crec);
	printf("MESSAGE FROM SERVER :%s\n",recbuff);
	printf("SENDING HELLO TO SERVER\n");
	strcpy(sendbuff,"HELLO");
	csend=send(cid,sendbuff,500,0);
	check("SEND",csend);
	printf("WAITING FOR OK MESSAGE\n");
	crec=recv(cid,recbuff,500,0);
	check("RECEIVE",crec);
	if(strncmp(recbuff,"250",3))
		printf("OK NOT RECEIVED\n");
	else
		printf("MESSAGE FROM SERVER:%s\n",recbuff);
	//Sending From Address
	printf("ENTER FROM ADDRESS:");
	scanf("%s",fromaddress);
	strcpy(sendbuff,"MAIL FROM:");
	strcat(sendbuff,fromaddress);
	csend=send(cid,sendbuff,500,0);
	check("SEND",csend);
	printf("WAITING FOR OK FROM SERVER\n");
	crec=recv(cid,recbuff,500,0);
	check("RECEIVE",crec);
	if(strncmp(recbuff,"250",3))
		printf("OK NOT RECEIVED\n");
	else
		printf("MESSAGE FROM SERVER:%s\n",recbuff);
	//Sending To Address	
	printf("ENTER TO ADDRESS:");
	scanf("%s",toaddress);
	strcpy(sendbuff,"MAIL TO:");
	strcat(sendbuff,toaddress);
	csend=send(cid,sendbuff,500,0);
	check("SEND",csend);
	printf("WAITING FOR OK FROM SERVER\n");
	crec=recv(cid,recbuff,500,0);
	check("RECEIVE",crec);
	if(strncmp(recbuff,"250",3))
		printf("OK NOT RECEIVED\n");
	else
		printf("MESSAGE FROM SERVER:%s\n",recbuff);
	//Sending Data
	printf("SENDING DATA TO THE SERVER\n");
	strcpy(sendbuff,"DATA");
	csend=send(cid,sendbuff,500,0);
	check("SEND",csend);
	printf("WAITING FOR OK FROM SERVER\n");
	crec=recv(cid,recbuff,500,0);
	check("RECEIVE",crec);
	if(strncmp(recbuff,"354",3))
		printf("OK NOT RECEIVED\n");
	else
		printf("MESSAGE FROM SERVER:%s\n",recbuff);
	printf("Enter mail body\n");
	for(i=0;;i++){
		fgets(mailbody,sizeof(mailbody),stdin);
		csend=send(cid,mailbody,1000,0);
		if(strncmp(mailbody,"$",1)==0)
			break;
		check("SENDING",csend);
	}
	printf("SENDING MAILBODY TO SERBER\n");
	printf("WAITING OK FROM SERVER\n");
	crec=recv(cid,recbuff,500,0);
	check("RECEIVE",crec);
	if(strncmp(recbuff,"221",3))
		printf("OK NOT RECEIVED\n");
	else
		printf("MESSAGE FROM SERVER:%s\n",recbuff);
	strcpy(sendbuff,"QUIT");
	csend=send(cid,sendbuff,1000,0);
	printf("SENDING %s....\n",sendbuff);
	strcpy(recbuff,"");
	crec=recv(cid,recbuff,500,0);
	if(strncmp(recbuff,"221 OK",6)==0){
		printf("Existing...");
	}
	printf("CONNECTION CLOSED\n");
	close(cid);
	return 0;
}
