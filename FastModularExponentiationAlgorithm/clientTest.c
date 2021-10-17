#include<stdio.h>		// used printf/scanf for demo (puts/getchar would be leaner)
#include<string.h>
#include<stdlib.h>

void printFastModTable (int i, char bt, int c, int f)
{
  printf ("%d\t\t %c\t\t %d\t\t %d\t\t\n", i, bt, c, f);

}

int fastModExpAlg (char *binary, int a, int n)
{
  int c = 0, f = 1;
  // Print
  printf ("i\t\tb\t\tc\t\tf\t\t\n");
  for (int i = strlen (binary) - 1; i >= 0; i--)
    {
      // 
      c = 2 * c;
      f = (f * f) % n;
      // Check that the binary digit at position i is 1 to perform ...
      if (binary[i] == '1')
	{
	  c = c + 1;
	  f = (f * a) % n;
	}
      printFastModTable (i, binary[i], c, f);
    }
  return f;
}

char * decToBin (int decimal)
{
  printf ("Converting to binary now\n");
  // hold the value of the binary string after convertion to be returned 
  char *binary = malloc (25);
  int i = 0;
  // do this while n is positive, until the remainder is 0
  while (decimal > 0)
    {
      // get the remainder of n divided by 2
      binary[i] = (decimal % 2) + '0';
      // get the new result of n
      decimal = decimal / 2;
      i++;
    }
  return binary;
}

int genPrivateKey ()
{
  // a ^ b mod n 
  int a, b, n, result;
  char *binary;			//= (char *) malloc(100);
  printf ("Enter a --> ");
  scanf ("%d", &a);
  printf ("\nEnter b --> ");
  scanf ("%d", &b);
  printf ("\nEnter n --> ");
  scanf ("%d", &n);

  // convert b to binary then assign to binary string  
  //strcpy(binary,decTobin(b));
  binary = decToBin (b);
  printf ("Binary is now %s", binary);
  // send binary string, a and n to calculate the fast modular of a to the power of b modular n
  // by using the binary string,  the integer a and the modular number
  // return the result 
  result = fastModExpAlg (binary, a, n);
  printf ("\n %d   ^   %d   mod   %d   =   %d\n", a, b, n, result);
  //cout << endl << a << " ^ " << b << " mod " << n << " = " << result << endl;
  return result;
}



int main(int argc , char *argv[])
{
  // Client Server Data
  int socket_desc;		                                        // file descripter returned by socket command
  int read_size;
  int keyReceived, pKb, g, q, gPKb, comKey, gPKa;
  struct sockaddr_in server;                                    // in arpa/inet.h
  char server_reply[100], client_message[100], space = ' ';	    // will need to be bigger
  char *found = " ", convert[15];

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
  printf ("What do you want to send to the server. (b for bye)\n");
  printf ("Press A with a space sending g and q or just enter message\n");

  while (strncmp (client_message, "b", 1))	// quit on "b" for "bye"
    {
      memset (client_message, '\0', 100);
      scanf ("%s", client_message);
      // if client message is -1 prompt user for G and Q then send to server
      // Server reply with a confirmation message that G and Q are set
      if (strcmp (client_message, "-1") == 0){
    	  printf ("\nEnter g --> ");
    	  scanf ("%d", &g);
    
    	  printf ("\nEnter q --> ");
    	  scanf ("%d", &q);
    
    	  // append the integer g and q to client message containing -1 already
    	  // after converting the integer to character g
    	  strcat (client_message, " ");
    	  sprintf (convert, "%d", g);
    	  strcat (client_message, convert);
    	  // q
    	  strcat (client_message, " ");
    	  sprintf (convert, "%d", q);
    	  strcat (client_message, convert);
    	}
      // Before Sending the message make sure(Check) that g and q are set
      if (!(g > 0 && q > 0))
        printf("No g prime or q set. Error, set them before to continue. Press -1 no enter them\n");
      else  // Otherwise if they are set process the message
    	{		
    	    found = (char *)malloc(strlen(client_message)+1);
    	    strcpy(found, client_message);
    	    // Get the first string in the client message
            found = strtok(found, " ");
    	    
    	    //*** Case 1 message containning private key
    	        // If first string is -1 then g and q is in the message
				if (strcmp(found,"-1")) {
					// Send the client message  
					// convert string of character containing the message into integer for calculation
					printf("Now sending %s ", client_message);
					pKb = atoi(client_message);
					// generate a key using your private key and g ^ pkb mod q
					gPKb = fastModExpAlg(decToBin(pKb), g, q);
					// Display private key and generated private key for debugging
					printf("Your Private Key is %d and your Generated Key is %d", pKb, gPKb);
					// convert the key to a character
					sprintf(convert, "%d", gPKb);
					// copy the key generated to be sent over to the server
					strcpy(client_message, convert);
				}
			//*** Case 2 message containing g and q
			// Just send that to the server
		    
		    // Send to server
        	if (send(socket_desc, &client_message, strlen(client_message), 0) < 0)
        	{
        		printf("Send failed");
        		return 1;
        	}
        			
			// Print message client is sending to the client 
			printf("\nSending Message: %.*s\n", (int)strlen(client_message), client_message);
			
			//Receive a reply from the server
        	if ((read_size = recv(socket_desc, server_reply, 100, 0)) < 0)
        	{
        		printf("recv failed");
        	}
        			
        	strcpy(server_reply,client_message);
			// Get the first string from the server reply
			found = strtok(server_reply, " ");
		
    	    // If server reply's first string is k
            // Process server generated private key
			if(strcmp(found,"k") == 0) {
				found = strtok(NULL, " ");
				// Get the string after the space then convert that into integer
				gPKa = atoi(found);
				printf("Server  Replies: %d.  Generated %d \n\n", read_size, gPKa);
				// Find common key using the server key received 
				comKey = fastModExpAlg(decToBin(gPKa), g, q);
				// display common key
				printf("Common key is %d \n", comKey);
			}
			// Otherwise If first string equal to -1 print the message from server 
			// then loop again
			else if(strcmp(found, "-1") == 0)
				printf("Server  Replies: %.*s\n\n", read_size, server_reply);
    	    else
    	        printf("Error Unknown Command. \n");
    	}
    }
  return 0;
}
