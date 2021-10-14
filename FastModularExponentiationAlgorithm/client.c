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
char* decToBin(int decimal) {
	// hold the value of the binary string after convertion to be returned 
	char binary[100];
	int i = 0;
	// do this while n is positive, until the remainder is 0
	while (n > 0) {
		// get the remainder of n divided by 2
		binary[i] = to_string(n % 2);
		// get the new result of n
		n = n / 2;
		i++;
	}
	return binary;
}

int fastModExpAlg(char* binary, int a, int n) {
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
	char binary[100];
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
	int socket_desc;    // file descripter returned by socket command
	int read_size;
	int keyReceived, pKb, g, q, gPKb;
	struct sockaddr_in server;    // in arpa/inet.h
	char  server_reply[100], client_message[100], user_input;   // will need to be bigger
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	printf("Trying to create socket\n");
	if (socket_desc == -1)
	{
		printf("Unable to create socket\n");
	}
		
// *********** This is the line you need to edit ****************
	server.sin_addr.s_addr = inet_addr("0.0.0.0");  // doesn't like localhost?
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
	printf("Press -1 with a space sending g and q or just enter message")
	while(strncmp(client_message,"b",1))      // quit on "b" for "bye"
	{
		
				memset(client_message,'\0',100);
				scanf("%s",client_message);
				// if the user input is -1 then set the g prime and q  
				if (client_message == "-1") {
					printf("\nEnter g --> ");
					scanf("%d", g);
					printf("\nEnter q --> ");
					scanf("%d", q);

					// append the integer g to client message containing -1 already
					// after converting the integer to character
					strcat(client_message, (g + '0'));
					strcat(client_message, ' ');
					strcat(client_message, (q + '0'));
				}
				// check that g and q exist before processing and sending the message
				if (!(g > 0 && q > 0))
				{
					printf("No g prime or q set. Error, set them before to continue\n");
				}
				else{
					// Send the client message  
					// Before sending key, Generate a new one by moding your key
					pKb = itoa(client_message);
					// generate a key using your private key and g ^ pk mod q
					gPKb = fastModExpAlg(decToBin(pKb), g, q);
					
					printf("Your Private Key is %d and your Generated Key is %d", pKb, gpKb);
					// convert the key to a character then send it to server
					client_message = gpKb + '0';

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
					printf("Server  Replies: %.*s\n\n", read_size, server_reply);
				}
				
	
	}

	
	return 0;
}
