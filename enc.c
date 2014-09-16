/*
 * serialTest.c:
 *	Very simple program to test the serial port. Expects
 *	the port to be looped back to itself
 *
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

int fd,i;
char data[20];


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
 }

// Pseudo-Random Generator Algorithm
// Input: state - the state used to generate the keystream
//        out - Must be of at least "len" length
//        len - number of bytes to generate

void pseudo_key_gem(unsigned char state[], unsigned char out[], int len)
{
   int i=0,j=0,x,t;
   for (x=0; x < len; ++x)
	{
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
	unsigned char state[256],key[1024],stream[1024];
	int len=0, x,i=0;
	char cipher[1024];
	char plaintext[1024],*p;




if ((fd = serialOpen ("/dev/ttyAMA0", 9600 )) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
    return 1 ;
  }

if (wiringPiSetup () == -1)
        {
        fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
        return 1 ;
        }


printf("\n\nRC4 ENCRYPTION-DECRYPTION BETWEEN TWO BCM2835 USING UART\n\n");
// Input: state - the state used to generate the keystream
//        key - Key to use to initialize the state
//        len - length of key in bytes


	printf("enter the Key\n");
	scanf("%s",key);
	printf("please enter the plaintext\n");
	scanf("%s",plaintext);
	//gets(plaintext);
	p=plaintext;
	while(*p!='\0')
	{
	len++;
		p++;
	}
   state_permutation(state,key,len);
   pseudo_key_gem(state,stream,len);

//Encryption
	printf("\n\tCIPHER TEXT\n");

    for(i=0;i<len;i++)
	{
	cipher[i]=plaintext[i]^stream[i];
	printf("%c",cipher[i]);
    }

	printf("\nSending Data.");
char j=0;
for(i=0;i<strlen(cipher);i++)
{
printf(".");
j=cipher[i];
serialPutchar(fd, j);
}

printf("\n \n Encrypted Data Sent \n\n");
serialClose(fd);

return 0;
}
