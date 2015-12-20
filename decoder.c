/*
 * Author: Kyler Witting
 * Class: CS3411
 * Semester: Fall
 * Due Date: 11/19/15
 * Program 3: Compressing Files
 * decoder.c 
 */


#include <stdio.h>

//reads the next bit, if it is the end of a char
//it reads the next char and gets that bit
//returns the int of the bit found
int read_bit( unsigned char *c ) {   
    static int wCount = 0;
    int binaryResult;
    
    if( wCount == 8 ) {
        read( 0, c, 1 );
        wCount = 0;
    }
    
    binaryResult = (*c&128) == 128;
    *c = (*c) << 1;
    
    wCount++;
    
    return binaryResult;
}

int main( ) {
    //dictionary holds the dict ints, binary is used to store the bits read
    int dictionary[15] = {0}, binary[8];

    //i, j, k,  are loop and temp vars
	//loop is used to go through the while loop until the EOF
    int i, j, k, bin, loop = 1;

	//Used for sprintf preparing 
    char buffer[256];

	//used for the read calls 
    unsigned char readChar;

	//Gets the first 15 chars, and puts the int into dictionary
    for( i = 0; i < 15; i++ ) {
        read( 0, &readChar, 1 );
        dictionary[i] = ( int ) readChar;
    }
	
	//If there is no std input then it exits the program
    if (read( 0, &readChar, 1 ) != 1 )
        return -1;

    while( loop ) {
        int binary[8] = {0};
        bin = read_bit( &readChar );
        //Freq Characters
        if ( bin == 0 ) {
            bin = read_bit( &readChar );
            //Non-Repeating Freq Characters
            if ( bin == 0 ){
                for( i = 0; i < 4; i++ )
                    binary[i] = read_bit( &readChar );
                if( binary[0] == 1 && binary[1] == 1 && binary[2] == 1 && binary[3] == 1 )
                    return 0;

                i = (binary[0]*8)+(binary[1]*4)+(binary[2]*2)+(binary[3]*1);
                sprintf( buffer, "%c", dictionary[i] );
                write( 1, buffer, 1 );       
            }
            //Repeating Freq Characters 
            else {
                for( i = 0; i < 3; i++ ) 
                    binary[i] = read_bit( &readChar );
                k = (binary[0]*4)+(binary[1]*2)+(binary[2]*1)+2;

                for( i = 0; i < 4; i++ ) 
                    binary[i] = read_bit( &readChar );
                j = (binary[0]*8)+(binary[1]*4)+(binary[2]*2)+(binary[3]*1);

                for ( i = 0; i < k; i++ ) {
                    sprintf( buffer, "%c", dictionary[j] );
                    write( 1, buffer, 1 );
                }
            }
        }
        //Non Freq Characters
        else {
            for( i = 0; i < 8; i++ )
                binary[i] = read_bit( &readChar );
            i = (binary[0]*128)+(binary[1]*64)+(binary[2]*32)+(binary[3]*16)+(binary[4]*8)+(binary[5]*4)+(binary[6]*2)+(binary[7]*1);

            sprintf( buffer, "%c", i );
            write( 1, buffer, 1 );
        }
    }
    return;
}
