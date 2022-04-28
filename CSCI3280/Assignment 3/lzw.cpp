/*
* CSCI3280 Introduction to Multimedia Systems *
* --- Declaration --- *
* I declare that the assignment here submitted is original except for source
* material explicitly acknowledged. I also acknowledge that I am aware of
* University policy and regulations on honesty in academic work, and of the
* disciplinary guidelines and procedures applicable to breaches of such policy
* and regulations, as contained in the website
* http://www.cuhk.edu.hk/policy/academichonesty/ *
* Assignment 3
* Name : Lai Man Hin
* Student ID : 1155136167
* Email Addr : 1155136167@link.cuhk.edu.hk
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

#define CODE_SIZE  12
#define TRUE 1
#define FALSE 0
int lib_index = 256;

/* function prototypes */
unsigned int read_code(FILE*, unsigned int); 
void write_code(FILE*, unsigned int, unsigned int); 
void writefileheader(FILE *,char**,int);
void readfileheader(FILE *,char**,int *);
void compress(FILE*, FILE*);
void decompress(FILE*, FILE*);

unordered_map<string, int> umap;
unordered_map<int, string> umap1;
int main(int argc, char **argv)
{
    int printusage = 0;
    int	no_of_file;
    char **input_file_names;    
	char *output_file_names;
    FILE *lzw_file;

    if (argc >= 3)
    {
		if ( strcmp(argv[1],"-c") == 0)
		{		
			/* compression */
			lzw_file = fopen(argv[2] ,"wb");
        
			/* write the file header */
			input_file_names = argv + 3;
			no_of_file = argc - 3;
			writefileheader(lzw_file,input_file_names,no_of_file);
        	        	
			/* ADD CODES HERE */
        	for (int i = 0; i < no_of_file; i++)
        	{
        		printf("Adding: %s\n",input_file_names[i]);
        		FILE *input_file = fopen(input_file_names[i], "rb");
				compress(input_file, lzw_file);
        		write_code(lzw_file, 4095, CODE_SIZE); // write end of file HERE, not compress()
				fclose(input_file);
			}			
			
			write_code(lzw_file, 0, CODE_SIZE);
			fclose(lzw_file);        	
		} else
		if ( strcmp(argv[1],"-d") == 0)
		{	
			/* decompress */
			lzw_file = fopen(argv[2] ,"rb");
			
			/* read the file header */
			no_of_file = 0;			
			readfileheader(lzw_file,&output_file_names,&no_of_file);
			
			/* ADD CODES HERE */
			int file_name_index = 0;
			for (int i = 0; i < no_of_file; i++)
        	{
        		string real_output_file_names;
        		while (output_file_names[file_name_index] != '\n' && output_file_names[file_name_index] != '\0')
        		{
        			real_output_file_names.push_back(output_file_names[file_name_index]);
        			file_name_index++;
				}
				file_name_index++;
        		printf("Deflating: %s\n",real_output_file_names.c_str());
        		FILE* output_file = fopen(real_output_file_names.c_str(), "wb");
				decompress(lzw_file, output_file);
				fclose(output_file);
			}
			fclose(lzw_file);		
			free(output_file_names);
		}else
			printusage = 1;
    }else
		printusage = 1;

	if (printusage)
		printf("Usage: %s -<c/d> <lzw filename> <list of files>\n",argv[0]);
 	
	return 0;
}

/*****************************************************************
 *
 * writefileheader() -  write the lzw file header to support multiple files
 *
 ****************************************************************/
void writefileheader(FILE *lzw_file,char** input_file_names,int no_of_files)
{
	int i;
	/* write the file header */
	for ( i = 0 ; i < no_of_files; i++) 
	{
		fprintf(lzw_file,"%s\n",input_file_names[i]);	
			
	}
	fputc('\n',lzw_file);

}

/*****************************************************************
 *
 * readfileheader() - read the fileheader from the lzw file
 *
 ****************************************************************/
void readfileheader(FILE *lzw_file,char** output_filenames,int * no_of_files)
{
	int noofchar;
	char c,lastc;

	noofchar = 0;
	lastc = 0;
	*no_of_files=0;
	/* find where is the end of double newline */
	while((c = fgetc(lzw_file)) != EOF)
	{
		noofchar++;
		if (c =='\n')
		{
			if (lastc == c )
				/* found double newline */
				break;
			(*no_of_files)++;
		}
		lastc = c;
	}

	if (c == EOF)
	{
		/* problem .... file may have corrupted*/
		*no_of_files = 0;
		return;
	
	}
	/* allocate memeory for the filenames */
	*output_filenames = (char *) malloc(sizeof(char)*noofchar);
	/* roll back to start */
	fseek(lzw_file,0,SEEK_SET);

	fread((*output_filenames),1,(size_t)noofchar,lzw_file);
	
	return;
}

/*****************************************************************
 *
 * read_code() - reads a specific-size code from the code file
 *
 ****************************************************************/
unsigned int read_code(FILE *input, unsigned int code_size)
{
    unsigned int return_value;
    static int input_bit_count = 0;
    static unsigned long input_bit_buffer = 0L;

    /* The code file is treated as an input bit-stream. Each     */
    /*   character read is stored in input_bit_buffer, which     */
    /*   is 32-bit wide.                                         */

    /* input_bit_count stores the no. of bits left in the buffer */

    while (input_bit_count <= 24) {
        input_bit_buffer |= (unsigned long) getc(input) << (24-input_bit_count);
        input_bit_count += 8;
    }
    
    return_value = input_bit_buffer >> (32 - code_size);
    input_bit_buffer <<= code_size;
    input_bit_count -= code_size;
    
    return(return_value);
}


/*****************************************************************
 *
 * write_code() - write a code (of specific length) to the file 
 *
 ****************************************************************/
void write_code(FILE *output, unsigned int code, unsigned int code_size)
{
    static int output_bit_count = 0;
    static unsigned long output_bit_buffer = 0L;

    /* Each output code is first stored in output_bit_buffer,    */
    /*   which is 32-bit wide. Content in output_bit_buffer is   */
    /*   written to the output file in bytes.                    */

    /* output_bit_count stores the no. of bits left              */    

    output_bit_buffer |= (unsigned long) code << (32-code_size-output_bit_count);
    output_bit_count += code_size;

    while (output_bit_count >= 8) {
        putc(output_bit_buffer >> 24, output);
        output_bit_buffer <<= 8;
        output_bit_count -= 8;
    }


    /* only < 8 bits left in the buffer                          */    

}

/*****************************************************************
 *
 * compress() - compress the source file and output the coded text
 *
 ****************************************************************/
void compress(FILE *input, FILE *output)
{
	
	/* ADD CODES HERE */
	int not_end = 1, read = 0;
	unsigned char nextchar, nowchar;
	unsigned int myprint;
	if (fscanf(input, "%c", &nextchar) == 1)
	{
		read++;
		nowchar = nextchar;
	}
	else
		not_end = EOF;
	while(not_end != EOF)
	{
		myprint = nowchar;
		string mystring;
		mystring.push_back(nowchar);
		not_end = fscanf(input, "%c", &nextchar);	// Getting nextchar
		read++;
		if (not_end == EOF)
			break;
		mystring.push_back(nextchar);	// mystring, contains of nowchar and nextchar, will passed to find at the library (umap)
		auto search_dict = umap.find(mystring);
		while (search_dict != umap.end() && not_end != EOF) // Getting longest string by looping, automatically exit when mystring is not found at umap library or EOF
		{
			myprint = search_dict -> second;	// Get the value in dictionary
			not_end = fscanf(input, "%c", &nextchar);
			read++;
			if (not_end != EOF)
			{
				mystring.push_back(nextchar);	// Get the nextchar and longer the string to find whether it exist in dictionary again
				search_dict = umap.find(mystring);
			}
		}
		if(search_dict == umap.end())
		{
			if (lib_index == 4095)		//EXCEED LIBRARY SIZE -> RESET
			{
				printf("* Read %d, compressed %d, next entry at 256: code word changed to 12\n", read, read-1);
				read = 0;
				umap.clear();
				lib_index = 255;
			}
			else
				umap[mystring] = lib_index;	//mystring is not at library, let me put that into it
			lib_index++;	// points to next library index, 256 -> 257, but 4095 (-> 255, with clearing) -> 256 
		}
		write_code(output, myprint, CODE_SIZE);	
		nowchar = nextchar;	
	}
	if (myprint < 256)
	{
		write_code(output, nowchar, CODE_SIZE); // Note that the final character may not be processed well if it is not a component in library
	}	// Note that the end of one text file (4095) is set at main, NOT Here 
}


/*****************************************************************
 *
 * decompress() - decompress a compressed file to the orig. file
 *
 ****************************************************************/
void decompress(FILE *input, FILE *output)
{	
	/* ADD CODES HERE */
	int not_end = 1, read = 0;
	string temp, back_string;
	unsigned int nowchar, prevchar;
	nowchar = read_code(input, CODE_SIZE);
	if (nowchar != 4095)
	{
		if (nowchar >= 256)
		{
			fprintf(output, "%s", (umap1.find(nowchar)->second).c_str());
		}
		else
		{
			fprintf(output, "%c", (char)nowchar);
		}
		prevchar = nowchar;
		if (prevchar >= 256)
			temp = (umap1.find(prevchar)->second)[0];
		else
			temp.push_back((char)prevchar);
	}
	while (nowchar != 4095)
	{
		nowchar = read_code(input, CODE_SIZE);
		if (nowchar == 4095)
			continue;
		back_string.clear();
		if (nowchar >= lib_index)
		{
			if (prevchar >= 256)
				back_string = umap1.find(prevchar)->second;
			else
			{
				back_string.push_back((char)prevchar);
			}
			back_string += temp;
		}
		else
		{
			if (nowchar >= 256)
				back_string = umap1.find(nowchar)->second;
			else
			{
				back_string.push_back((char)nowchar);
			}
		}
		fprintf(output, "%s", back_string.c_str());
		temp.clear();
		if (prevchar >= 256)
			temp = umap1.find(prevchar)->second;
		else
			temp.push_back((char)prevchar);
		temp.push_back(back_string[0]);
		if (lib_index == 4095)		//EXCEED LIBRARY SIZE -> RESET
		{
			umap1.clear();
			lib_index = 255;	
		}
		else
			umap1[lib_index] = temp;
		lib_index++;	// if it is 255 after reset, it will be set into 256 here
		temp.clear();
		temp.push_back(back_string[0]);
		prevchar = nowchar;
	}
}
