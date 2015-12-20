#include<stdio.h>
#include <fcntl.h>

int main( int argc , char *argv[] ) {  
	int outputFile, inputFile;	
	ssize_t retIn, retOut;
	char ch, buffer[256];

	//local
	outputFile = open(argv[2], O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if ( outputFile < 0 ) {
		sprintf( buffer, "The file to copy to could not be opened.\n" );
        write( 2, buffer, 256 );
		return -1;
	}
	
	//remote
	inputFile = open(argv[1], O_RDONLY);
	if ( inputFile < 0 ) {
		sprintf( buffer, "The file to copy from could not be opened.\n" );
        write( 2, buffer, 256 );
		return -1;
	}

	if ( lseek( inputFile, 10, SEEK_SET ) < 0 ) {
		sprintf( buffer, "Seeking to position 10 has failed. \n" );
       	write( 2, buffer, 256 );
		return -1;
	} 

	while( ( retIn = read( inputFile, &ch, 1 ) ) > 0 ) {
		retOut = write( outputFile, &ch, (ssize_t) retIn );
		if ( retOut != retIn ) {
			sprintf( buffer, "Writing to the outputFile failed. \n" );
        	write( 2, buffer, 256 );
			return -1;
		} 
	}
	close( inputFile );
	close( outputFile );
	return 1;
}
