#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART

//Setting Up The UART

void delay(unsigned int n)
{
unsigned int i,j;
for(i=0;i<=n;i++)
for(j=0;j<=1275;j++);
}


// Input: state - the state used to generate the keystream
//        key - Key to use to initialize the state
//        len - length of key in bytes
void state_permutation(unsigned char state[], unsigned char key[], int len)
{
   int i,j=0,t;
   unsigned char T[256];
   for (i=0; i < 256;i++){            //state initialization
     	 state[i] = i;
		T[i]=key[i % len];
		// printf("normal=%d\n",state[i]);
	    }

//Initial Permutation of state
   for (i=0; i < 256; ++i) {
      j = (j + state[i] +T[i])%256;
		  t = state[i];
       state[i] = state[j];
      state[j] = t;
   }

for(i=0;i<256;++i)
{
	//printf("random=%d\n",state[i]);
	}
}

// Pseudo-Random Generator Algorithm
// Input: state - the state used to generate the keystream
//        out - Must be of at least "len" lengt
//        len - number of bytes to generate

void pseudo_key_gem(unsigned char state[], unsigned char out[], int len)
{
   int i=0,j=0,x,t;

   for (x=0; x < len; ++x)  {
      i = (i + 1) % 256;
      j = (j + state[i]) % 256; 
      t = state[i];
      state[i] = state[j]; 
      state[j] = t;
      out[x] = state[(state[i] + state[j]) % 256];
   }
}


int main()
{
	int uart0_filestream = -1;
	uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode

	if (uart0_filestream == -1)
	{
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}

	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

//----- CHECK FOR ANY RX BYTES -----

printf("Waiting for the receprion\n");

while (1)
{
unsigned char plaintext[1024], cipher[1024];
unsigned char state[256],key[1024],stream[1024];
int len=0, x, i=0;
//delay(500); 
	if (uart0_filestream != -1)
	{
		// Read up to 255 characters from the port if they are there
		unsigned char rx_buffer[256], p[256], q[256];

		int rx_length = read(uart0_filestream, (void*)rx_buffer, 255);		//Filestream, buffer to store in, number of bytes to read (max)

		//printf("Waiting for the receprion\n");

		if (rx_length < 0)
		{
			//An error occured (will occur if there are no bytes)
		}
		else if (rx_length == 0)
		{

		//No data waiting
		}
		
	else
		{
		printf("\nReceived Encrypted Message:: ");
		rx_buffer[rx_length] = '\0';
		printf("%s\n", rx_buffer);

		printf("Enter the same Key as used at sender\n");
		scanf("%s",key);

		state_permutation(state,key,rx_length);
   		pseudo_key_gem(state,stream,rx_length);

		//Decryption

		printf("\nRECEIVED PLAIN  MESSAGE\n");
		unsigned char papa[1024];
		for(i=0;i<rx_length;i++)
		{
		plaintext[i]=rx_buffer[i]^stream[i];
		printf("%c ",plaintext[i]);
    		}
		}
}
}
//----- CLOSE THE UART -----
	close(uart0_filestream);
}
