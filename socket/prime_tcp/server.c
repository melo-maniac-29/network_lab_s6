
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netdb.h>
#include<unistd.h>

int isPrime(int n);


int main()
{
    int serversocket,clientsocket,port; 
    struct sockaddr_in serveraddr,clientaddr; 
    socklen_t len; 
    char message[50]; 
    struct serveraddr; 

    int numrecieved;

    serversocket=socket(AF_INET,SOCK_STREAM,0); 
    bzero((char*)&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;

    printf("Enter the port number ");
    scanf("%d",&port);

    serveraddr.sin_port=htons(port); 
    serveraddr.sin_addr.s_addr=INADDR_ANY; 
    bind(serversocket,(struct sockaddr*)&serveraddr,sizeof(serveraddr)); 
    bzero((char*)&clientaddr,sizeof(clientaddr)); 
    len=sizeof(clientaddr); 
    listen(serversocket,5); 

    printf("\nWaiting for client connection\n"); 
    printf("\n connection verification successfully done");
    clientsocket=accept(serversocket,(struct sockaddr*)&clientaddr,&len);

    printf("\nClient connectivity received.\n");
    

    write(clientsocket,"Enter an integer:",sizeof("Enter an integer:")); 
   
    read(clientsocket,&numrecieved,sizeof(numrecieved));
   
    printf("\nNumber send from server : %d \n",numrecieved);  //receiving data 

    
    if(isPrime(numrecieved))
    {
        write(clientsocket,"Number is prime \n",sizeof("Number is prime"));
    }
    else
    {
        write(clientsocket,"Number is not prime \n",sizeof("Number is not prime"));
    }

    close(clientsocket);
    


}


int isPrime(int n)
{
    int i;
    for(i=2;i<=n/2;i++)
    {
        if(n%i==0)
        {
            return 0;
        }
    }
    return 1;
}

