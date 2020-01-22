/* final project.
 * it's to implement the Secure Hash Algorithm
 * SHA-1 in C.
 * @Author Tianhui Zhou
 * @Since 03/20/2019
 */

#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 1048576

//function prototype
unsigned int readFile(unsigned char []);
unsigned int calculateBlocks(unsigned int);
void convertCharArrayToIntArray(unsigned char [], unsigned int [], unsigned int);
void addBitCountToLastBlock(unsigned int [], unsigned int, unsigned int);
void computeMessageDigest(unsigned int [], unsigned int);
unsigned int f(unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int K(unsigned int);
unsigned int S(unsigned int, unsigned int);
unsigned int W(unsigned int, unsigned int [], unsigned int);


int main(){
    unsigned char buffer[MAX_SIZE]={'\0'};
    unsigned int message[MAX_SIZE] = {0x00000000};
    unsigned int length = 0;
    unsigned int blockCount = 0;
    //call function
    length = readFile(buffer);
    blockCount = calculateBlocks(length);
    convertCharArrayToIntArray(buffer, message, length);
    addBitCountToLastBlock(message, length, blockCount);
    
    computeMessageDigest(message, blockCount);
    return 1;
}//close main method

/* readFile function.
 * the file will be read into and stored in the buffer[] array
 * return the number of characters
 */
unsigned int readFile(unsigned char buffer[]){
    unsigned int length = 0;
    int letter = 0;
    int i = 0;//buffer index
    letter = getchar();
    while(EOF != letter){
        buffer[i] = letter;
        i++;
        length++;
        letter = getchar();
    }//close while
    buffer[i] = 0x80;
    return length;
}//close readFile

/* calculateBlocks function.
 * calculate the block count
 * return the number of blocks
 */
unsigned int calculateBlocks(unsigned int sizeOfFileInBytes){
    unsigned int blockCount = 0;
    int a = 0;
    //calculate the block
    blockCount = (((8 * sizeOfFileInBytes) + 1) / 512) + 1;
    //if an extra block needs to add or not
    if((((8 * sizeOfFileInBytes) + 1) % 512) > (512 - 64)){
        blockCount++;
        sizeOfFileInBytes--;
    }//close if
    return blockCount;
}//close calculateBlocks

/* convertCharArrayToIntArray function
 * packed 4 characters into 1 integer
 * every 16 int is a block
 */
void convertCharArrayToIntArray(unsigned char buffer[], unsigned int message[], unsigned int sizeOfFileInBytes){
    unsigned int mask = 0x00000000;
    int iBuffer = 0;//buffer index
    int iMessage = 0;// message index
    int shifter = 24;
    int temp = 0;
    unsigned int number;
    int a = 0;
    /* each 4 char will be 1 int.
     * when loop 4 times, the mask will be set into message[i]
     */
    while(iBuffer < (sizeOfFileInBytes+1)){
      //convert it to unsigned int
      number = (unsigned int)buffer[iBuffer];
      number = number << (shifter - a);
      //pack the four char into one integer
      mask = mask | number;
      a = a + 8;
        if(a>24){// already loop 4 times, set the mask to int array
            a = 0;//reset it to 0
            message[iMessage] = mask | message[iMessage];
            iMessage++;//raise the index number
            mask = 0x00000000;//reset to default
        }//close if
        iBuffer++;
    }//close while
    /* if there have 6 chars, the first 4 are already added to int[1]
     * but remain 2 chars, which have not been added.
     * mask still hold the value of the 2 chars
     */
    //printf("%08x\n", message[0]);
    message[iMessage] = mask | message[iMessage];
    return;
}//close convertCharArrayToIntArray

/* addBitCountToLastBlock function
 * based on the length of the file in bits
 * convert it to hex, and add it to the last block
 */
void addBitCountToLastBlock(unsigned int message[], unsigned int sizeOfFileInBytes, unsigned int blockCount){
    int position = 0;
    //calculate the number in bits
    unsigned int addNumber = sizeOfFileInBytes*8;
    //find the last index
    position = blockCount*16 - 1;
    //add it to int array
    message[position] = message[position] | addNumber;
    return;
}//close addBitCountToLastBlock

/* computeMessageDigest function
 * compute the message digest.
 */
void computeMessageDigest(unsigned int message[], unsigned int blockCount){
    unsigned int H0 = 0x67452301;
    unsigned int H1 = 0xEFCDAB89;
    unsigned int H2 = 0x98BADCFE;
    unsigned int H3 = 0x10325476;
    unsigned int H4 = 0xC3D2E1F0;
    unsigned int A;
    unsigned int B;
    unsigned int C;
    unsigned int D;
    unsigned int E;
    unsigned int temp;
    unsigned int times = 0;
    //
    while(times<blockCount){
        A = H0;
        B = H1;
        C = H2;
        D = H3;
        E = H4;
      for(unsigned int t=0; t<80; t++){
          temp = S(5,A) + f(t,B,C,D) + E + W(t, message, times) + K(t);
          E=D;
          D=C;
          C=S(30,B);
          B=A;
          A=temp;
      }//close for
      H0 = H0 + A;
      H1 = H1 + B;
      H2 = H2 + C;
      H3 = H3 + D;
      H4 = H4 + E;
      times++;
    }//close while
   printf("%08X %08X %08X %08X %08X\n", H0, H1, H2, H3, H4);
}//close computeMessageDigest

/* f helper function
 * using it for final step.
 */
unsigned int f(unsigned int t, unsigned int B, unsigned int C, unsigned int D){
    unsigned int result = 0;;
    if(0<=t && t<=19){
        result = (B&C)|((~B)&D);
    }//0~19
    else if(20<=t && t<=39){
        result = B^C^D;
    }//20~39
    else if(40<=t && t<=59){
        result = (B&C)|(B&D)|(C&D);
    }//40~59
    else if(60<=t && t<=79){
        result = B^C^D;
    }
    return result;
}//close f

/* K helper function
 * using it for final step
 */
unsigned int K(unsigned int t){
    unsigned int result = 0;
    if(0<=t && t<=19){
        result = 0x5A827999;
    }//0~19
    else if(20<=t && t<=39){
        result = 0x6ED9EBA1;
    }//20~39
    else if(40<=t && t<=59){
        result = 0x8F1BBCDC;
    }//40~59
    else if(60<=t && t<=79){
        result = 0xCA62C1D6;
    }//60~79
    return result;
}//close K

/* circleLeft
 * performs a circular left shift of bits.
 */
unsigned int S(unsigned int a, unsigned int b){
    //the bit of 'b' rotate to the left 'a' times
    unsigned int leftShift = b << a;
    unsigned int rightShift = b >> (32 - a);
    unsigned int result = leftShift | rightShift;
    
    return result;
}//close S

/* W helper function
 * t=0~15, W = int array[t]
 * otherwise, it's different
 */
unsigned int W(unsigned int t, unsigned int message[], unsigned int times){
    unsigned int result = 0;
    unsigned int i = times*16;
    if(t<=15){
        result = message[t+i];
    }//close if
    else{
       result = S(1,(W(t-3, message, times)^W(t-8, message, times)^W(t-14, message, times)^W(t-16, message, times)));
    }//close else
    return result;
}
