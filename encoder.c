/*
 * Author: Kyler Witting
 * Class: CS3411
 * Semester: Fall
 * Due Date: 11/19/15
 * Program 3: Compressing Files
 *  Compresses the standard input to the standard output
 *  Uses a modified run-length encoding algorithm 
 */

#include <stdio.h>

//writes the bit given into the unsigned char c
//if the char is full (8 bits) it writes it to std out and count is 0 again
//it returns the wCount for the last line bit number checking 
int write_bit( int n ) {
    static unsigned char c = 0;   
	static int wCount = 0;

    c = ( c << 1 )|n;
    wCount++;
    if( wCount >= 8 ) {
        write( 1, &c, 1 );
        wCount = 0;
    }
    return wCount;
}

int main( ) {
    //buffer is used for sprintf buffering
    char buffer[256];
    
    //freqCount counts number of each ascii character, binary holds the binary until printing
    //dictBinary is used for the dictionary printing, dictionary is the number value of the frequent characters
    int freqCount[256] = {0}, dictBinary[8], binary[9], dictionary[15];

    //i and j are for loops and temp vars, highLoc = highestLocation for dictionary find
    //check is the highest count and a temp var
    //loopBool is the loop checking for reading the file
    int i, j,  highLoc, check, count, loopBool = 1;

    //unsigned char readChar is used for my read calls 
    unsigned char readChar;

    //Writes the count of each character into freqCount
    while ( read( 0, &readChar, 1) == 1 ){
        i = ( int ) readChar;
        freqCount[i]+=1;
    }

    //Finds the most used character 
    //Goes through for the top 15
    for( j = 0; j < 15; j++ ) {

        //resets the highest count and sets new binary to 00000000
        check = 0;
        int dictBinary[8] = {0};

        //goes through and finds the highest count of chars and then gets the location of it
        for( i = 0; i < 256; i++ ) {
            if( freqCount[i] > check ) {
                check = freqCount[i];
                highLoc = i;
            }
        }

        //check is temp var, i is 1 to start going through the bits
        i = 1;
        check = highLoc;
        dictionary[j] = highLoc;
        //Finds the binary of the highest location and stores it in binary[]
        while( check != 0 ) {
            dictBinary[8-i] = ( check % 2 );
            check = check / 2;
            i++;
        }

        //Prints the dictionarty 
        for ( i = 0; i < 8; i++ ) {
			write_bit(dictBinary[i]);
        }
        //puts the count to 0 so it doesnt get repeated in dictionary 
        freqCount[highLoc] = 0;
    }	
    
    //sets the pointer back to the beginning of the file
    //It will print error if it cannot do that
    //and then end the program
    if ( lseek(0,0,SEEK_SET) == -1 ) {
        sprintf( buffer, "The input file could not be seeked to the beginning.\n" );
        write( 2, buffer, 256 );
		return -1;
    }


    //Goes through each character of the file, from the start
    if ( read( 0, &readChar, 1 ) != 1 )
		return -1;    
	while ( loopBool  ) {
        //Sets everything to 0 for each character 
        loopBool = 0;
        int binary[9] = {0};
        binary[0] = 1; 
        check = ( int ) readChar;
        j = 0;
        count = 0;

        //checks if the char is in the dictionary, if it is not, it goes to the if statement after
        while( j < 15 && binary[0] == 1 ) { 
            if( dictionary[j] == check ) {
                loopBool = 1;
                if ( read( 0, &readChar, 1 ) == 1 ){
                    while ( ( (int) readChar ) == check && count < 8 ) {
                        count ++;
                        read( 0, &readChar, 1 );
                    }
                }
                else
                    loopBool = 0;
                
                //if the char is frequent but doesn't repeat
                if ( count == 0 ) {
                    i = 1;
                    binary[0] = 0;
                    binary[1] = 0; 
                    while( j != 0 ) {
                        binary[6-i] = ( j % 2 );
                        j = j / 2;
                        i++;
                    }

                    for ( i = 0; i < 6; i++ ) 
						write_bit( binary[i] );
                 }

                //if the char is freqent and repeats, it prints "count" times
                else {
                    binary[0] = 0;
                    binary[1] = 1;
                    i = 1;
                    count = count -1;
                    while( count != 0 ) {
                        binary[5-i] = ( count % 2 );
                        count = count / 2;
                        i++;
                    }
                    i = 1;
                    while( j != 0 ) {
                        binary[9-i] = ( j % 2 );
                        j = j / 2;
                        i++;
                    }

                    for ( i = 0; i < 9; i++ )
                       write_bit( binary[i] );
                }
            }
            j++;

        }

        //if char wasn't in dict, the first spot of the binary stays 0 and
        //then it prints out 1 + binary of the char
        if (binary[0] == 1 ) {
            i = 1;
            while( check != 0 ) {
                binary[9-i] = ( check % 2 );
                check = check / 2;
                i++;
            }
            i = 1;
            for ( i = 0; i < 9; i++ )
                write_bit( binary[i] );
        }
        
        //If the read up top wasn't read then it reads then next char before going through the while loop again
        if ( loopBool == 0 ){
            //if the read fails, it prints out the EOF for the compressed file
            if (read( 0, &readChar, 1 ) == 1 )
                loopBool = 1;
            else{
				write_bit( 0 );
				write_bit( 0 );
				write_bit( 1 );
				write_bit( 1 );
				write_bit( 1 );
				j = write_bit( 1 );
				while( j != 0 )
					j = write_bit( 0 );     
				return 0;
            } 
        }
    }


}
