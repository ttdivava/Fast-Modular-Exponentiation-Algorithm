/****************************************************
*
*    Basic minimal socket server program for use
*    in CSc 487 final projects.  You will have to
*    enhance this for your projects!!
*
*                                  RSF    11/14/20
*
****************************************************/
#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<stdlib.h>	// for system & others

char * decToBin(int decimal) {
	// hold the value of the binary string after convertion to be returned 
	char binary[100];
	int i = 0;
	// do this while n is positive, until the remainder is 0
	while (n > 0) {
		// get the remainder of n divided by 2
		binary[i]= to_string(n % 2);
		// get the new result of n
		n = n / 2;
		i++;
	}
	return binary;
}

int fastModExpAlg(char * binary, int a, int n) {
	int c = 0,
		f = 1;
	// Print
	cout << "i\t\t" << "b\t\t" << "c\t\t" << "f\t\t" << endl;
	for (int i = strlen(binary) - 1; i >= 0; i--) {
		// 
		c = 2 * c;
		f = (f * f) % n;
		// Check that the binary digit at position i is 1 to perform ...
		if (binary[i] == '1') {
			c = c + 1;
			f = (f * a) % n;
		}
		//printFastModTable(i, binary[i], c, f);
	}
	return f;
}

int genPrivateKey() {
	// a ^ b mod n 
	int a, b, n, result;
	char binary [100];
	printf("Enter a --> ");
	scanf("%d", a);
	printf("\nEnter b --> ");
	scanf("%d", b);
	printf("\nEnter n --> ");
	scanf("%d", n);
	// convert b to binary then assign to binary string  
	binary = decTobin(b);
	// send binary string, a and n to calculate the fast modular of a to the power of b modular n
	// by using the binary string,  the integer a and the modular number
	// return the result 
	result = fastModExpAlg(binary, a, n);
	printf("\n %d   ^   %d   mod   %d   =   %d\n", a, b, n, result);
	//cout << endl << a << " ^ " << b << " mod " << n << " = " << result << endl;
	return result;
}

int main(int argc , char *argv[])
{
	int socket_desc , new_socket , c, read_size, i, pKa, gPKa, gPKb, keyReceived;
	struct sockaddr_in server , client;
	char *message, client_message[100];

	char *list;	
	list = "ls -l\n";

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8421 );                 // Random high (assumed unused) port
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf(" unable to bind\n");
		return 1;
	}
	printf(" socket bound, ready for and waiting on a client\n");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept incoming connection
	printf(" Waiting for incoming connections... \n");
	
	
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}
	
	printf("Connection accepted\n");

	
	//Reply to the client
	message = "You have located Server X at our undisclosed location.  What would you like to say?\n";
	//write(new_socket , message , strlen(message));
	
	//Receive a message from client
	// OLD -- while( (read_size = recv(new_socket , client_message , 100 , 0)) > 0 )
	while ((read_size = recv(new_socket, client_message, 100, 0)) > 0)
	{

		printf("\n Client sent %2i byte message:  %.*s\n",read_size, read_size ,client_message);

		if(!strncmp(client_message,"showMe",6)) 
		{
			printf("\nFiles in this directory: \n");
			system(list);
			printf("\n\n");
		}
		// Ask user to enter their key 
		printf("\nEnter your key --> ");
		scanf("%d", pKa);
		
		// Generate a key using private key and g ^ pk mod q		
		// convert to binary && mod private key
		gPKa = fastModExpAlg(decTobin(pKa), g, q);

		
		// Find key generated from key received 
		gPKb = fastModExpAlg(decToBin(client_message, g, q));

		printf("Your Private Key is %d and your Generated Key is %d", pKa, gPKa);
		client_message = gPKb + '0';

		// send private key produced 
		write(new_socket, client_message, read_size);
		// Receive private key from client 
		
		//Send the result back to client if they match

		//for(i=0;i< read_size;i++)
		//{
		//	if ( i%2)
		//		client_message[i] = 'z';
		//}

       //        	printf(" Sending back Z'd up message:  %.*s \n", read_size ,client_message);

		//write(new_socket, client_message , strlen(client_message));
		//write(new_socket, client_message , read_size);
	}
	
	if(read_size == 0)
	{
		printf("client disconnected\n");
		fflush(stdout);
	}
	else if(read_size == -1)
	{
		perror("receive failed");
	}
		
	//Free the socket pointer
	close(socket_desc);
	return 0;
}
