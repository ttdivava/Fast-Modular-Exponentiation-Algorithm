/****************************************************
*
*    Basic minimal socket client program for use
*    in CSc 487 final projects.  You will have to
*    enhance this for your projects!!
*
*                                  RSF    11/14/20
*
****************************************************/
#include<stdio.h>      // used printf/scanf for demo (puts/getchar would be leaner)
#include<string.h>	
#include<sys/socket.h>
#include<arpa/inet.h>  // for inet_addr and sockaddr_in structs
#include<stdlib.h>

char* decToBin(int decimal) {
	printf("Converting to binary now\n");
	// hold the value of the binary string after convertion to be returned 
	char binary[100];
	int i = 0;
	// do this while n is positive, until the remainder is 0
	while (decimal > 0) {
		// get the remainder of n divided by 2
		binary[i] = (decimal % 2) + '0';
		// get the new result of n
		decimal = decimal / 2;
		i++;
	}
	return binary;
}

int fastModExpAlg(char* binary, int a, int n) {
	int c = 0,
		f = 1;
	// Print
	printf("i\t\tb\t\tc\t\tf\t\t\n");
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
	char * binary ;
	printf("Enter a --> ");
	scanf("%d", &a);
	printf("\nEnter b --> ");
	scanf("%d", &b);
	printf("\nEnter n --> ");
	scanf("%d", &n);
	
	// convert b to binary then assign to binary string  
	//strcpy(binary,decTobin(b));
	 binary = decToBin(b);
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
	int socket_desc;    // file descripter returned by socket command
	int read_size;
	int keyReceived, pKb, g, q, gPKb, comKey, gPKa;
	struct sockaddr_in server;    // in arpa/inet.h
	char  server_reply[100], client_message[100], space =' ';   // will need to be bigger
	char* found, convert[15];
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	printf("Trying to create socket\n");
	if (socket_desc == -1)
	{
		printf("Unable to create socket\n");
	}
		
// *********** This is the line you need to edit ****************
	server.sin_addr.s_addr = inet_addr("169.254.204.239");  // doesn't like localhost?
	server.sin_family = AF_INET;
	server.sin_port = htons( 8421 );    // random "high"  port number

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf(" connect error");
		return 1;
	}
	

	//Get data from keyboard and send  to server
	printf("What do you want to send to the server. (b for bye)\n");
	printf("Press -1 with a space sending g and q or just enter message\n");
	while(strncmp(client_message,"b",1))      // quit on "b" for "bye"
	{
		
				memset(client_message,'\0',100);
				scanf("%s",client_message);
				// if the user input is -1 then set the g prime and q  
				if (strcmp(client_message,"-1") == 0) {
					
					printf("\nEnter g --> ");
					scanf("%d", &g);

					printf("\nEnter q --> ");
					scanf("%d", &q);
					strcpy(client_message, "-1");
		
					// append the integer g to client message containing -1 already
					// after converting the integer to character g
					printf(" %s ", client_message);
					strcat(client_message, " ");
					
					sprintf(convert, "%d", g);
printf("Conversion for g is %s", convert);					
					strcat(client_message, convert);
					// q
					strcat(client_message, " ");
					sprintf(convert, "%d", q);
					strcat(client_message, convert);
				}
				found = strtok(client_message, " ");
				printf("Message after concatenating is %s \n", client_message);
				// check that g and q exist before processing and sending the message
				if (!(g > 0 && q > 0))
				{
					printf("No g prime or q set. Error, set them before to continue. Press -1 no enter them\n");
				}
				else{
					// Check whether g and q is being sent by checking if the first string is 1
					// If not -1 then no g and q is being sent process this
					if (strcmp(found,"-1") != 0) {
						// Send the client message  
						// convert string of character containing the message into integer for calculation
						printf("Now sending %s ", client_message);

						pKb = atoi(client_message);
						// generate a key using your private key and g ^ pkb mod q
						gPKb = fastModExpAlg(decToBin(pKb), g, q);
						// Display private key and generated private key for debugging
						printf("Your Private Key is %d and your Generated Key is %d", pKb, gPKb);
						// convert the key to a character then send it to server
						sprintf(convert, "%d", gPKb);
						strcat(client_message, convert);
					}		
					// Send to server
					if (send(socket_desc, &client_message, strlen(client_message), 0) < 0)
					{
						printf("Send failed");
						return 1;
					}

					printf("\nSending Message: %.*s\n", (int)strlen(client_message), client_message);

					//Receive a reply from the server
					if ((read_size = recv(socket_desc, server_reply, 100, 0)) < 0)
					{
						printf("recv failed");
					}
					found = strtok(server_reply, " ");
					// if key k is in front of the message then generated key from the server
					if(strcmp(found,"k") == 0) {
						found = strtok(NULL, " ");
						// Get the string after the space then convert that into integer
						gPKa = atoi(found);
						printf("Server  Replies: %d.  Generated %d \n\n", read_size, gPKa);
						// Find common key using the server key received 
						comKey = fastModExpAlg(decToBin(gPKa), g, q);
						// display common key
						printf("Common key is %d", comKey);
					}
					else
						printf("Server  Replies: %.*s\n\n", read_size, client_message);
				}
				
	
	}

	
	return 0;
}
