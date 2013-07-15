/*
 * radiotrim -
 * pieced together by Jostein Kjønigsen for his own purposes and those only.
 * this is meant to work, not to be pretty or idiomatic c.
 */

#include <stdlib.h>
#include <stdio.h>

char *buffer;
char* source_name;
char* output_name;

unsigned long fileLen;

FILE *source;
FILE *output;

int lastNonZero;
int i;
int result;

int main( int argc, char *argv[] )
{
	printf("\nSamsung Galaxy Nexus I9250 radio-image trimmer.\n");

    if (argc != 3)
    {
		printf("Written by Jostein Kjonigsen for his own use only.\n");
		printf("This tool comes with NO warranties provided.\n\n");
		printf("usage: %s input.img output.img\n", argv[0] );
        exit(1);
    }

    source_name = argv[1];


    //Open file
    source = fopen(source_name, "rb");
    if (!source)
    {
        fprintf(stderr, "Unable to open file %s\n", source_name);
        exit(1);
    }

    fprintf(stdout, "%s opened successfully.\n", source_name);

    //Get file length
    fseek(source, 0, SEEK_END);
    fileLen=ftell(source);
    fseek(source, 0, SEEK_SET);

    fprintf(stdout, "File length: %li bytes.\n", fileLen);

    //Allocate memory
    buffer=(char *)malloc(fileLen+1);
    if (!buffer)
    {
        fprintf(stderr, "Memory error!");
        fclose(source);
        exit(1);
    }

    //Read file contents into buffer
    fread(buffer, fileLen, 1, source);
    fclose(source);

    // determine last non-zero byte
    lastNonZero = 0;
    for (i=0; i < fileLen; i++)
    {
        if (buffer[i] != 0)
        {
            lastNonZero = i;
        }
    }
    fprintf(stdout, "Last non-zero byte offset: %i\n", lastNonZero);

    // do the trim
    output_name = argv[2];
    fprintf(stdout, "Creating trimmed copy %s.\n", output_name);

    output = fopen(output_name, "w+");
    if (!output)
    {
        fprintf(stderr, "Unable to open file %s\n", output_name);
        exit(1);
    }

	// pad with 0 zero, just for politeness ;)
	if (lastNonZero < fileLen)
	{
		lastNonZero++;
	}

    for (i=0; i < lastNonZero; i++)
        {
        result = fputc(buffer[i], output);
        if (result == EOF)
        {
            fclose(output);
            fprintf(stderr,"Error writing file!\n");
            free(buffer);
            exit(1);
        }
        }
    fclose(output);

    free(buffer);

    return 0;
}
