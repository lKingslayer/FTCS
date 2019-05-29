/**
* @file image_blur_gray.c
* @brief C program to blur the grayscale image, using convolution
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <time.h>

/*
 * int 4 byte 
 * unsighed short 1 byte = char 1 byte, 0-255=2^8
 * long long 8 byte ???
 */

int get_max(int a, int b) {
    if (a > b) 
        return a;
    return b;
}

void temp2rgb(float value, short *r, short *g, short *b) {
    float max = 100, min = 0;
    float ratio = 2 * (value - min) / (max - min); 
    *b = (short)(get_max(0, 255 * (1 - ratio)));
    *r = (short)(get_max(0, 255 * (ratio - 1)));
    *g = (short)255 - *b - *r; 
}

int main()
{
	clock_t start, stop;

	start = clock();											// Note the start time for profiling purposes.

	FILE *fIn = fopen("./images/lena_color.bmp","r");					//Input File name
	FILE *fOut = fopen("./images/lena_blur_color.bmp","w+");		    		//Output File name
	int i,j,y;
	unsigned char byte[54];
	
	if(fIn==NULL)												// check if the input file has not been opened succesfully.
	{											
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)											//read the 54 byte header from fIn
	{
        byte[i] = getc(fIn);
        printf("%c",byte[i]);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);					//write the header back

	// extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width;									//calculate the image size

	unsigned char buffer[size][3];								//store the input image data
	unsigned char out[size][3];									//store the output image data

	for(i=0;i<size;i++)											//read image data character by character
	{
		buffer[i][2]=getc(fIn);									//blue
		buffer[i][1]=getc(fIn);									//green
		buffer[i][0]=getc(fIn);									//red
	}

    
	float v = 1.0/ 9.0;
    float kernel[3][3]={{v,v,v},
                        {v,v,v},
                        {v,v,v}};

	for(int x=1;x<height-1;x++)
	{					
		for(y=1;y<width-1;y++)
		{
            /*
			float sum0= 0.0;
			float sum1= 0.0;
			float sum2= 0.0;
			for(i=-1;i<=1;++i)
			{
				for(j=-1;j<=1;++j)
				{								//matrix multiplication with kernel with every color plane
					sum0=sum0+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
					sum1=sum1+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
					sum2=sum2+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];
				}
			}
            */
            short r, g, b; 
            int temp = y * 100 / (float)width;
            temp2rgb(temp, &r, &g, &b); 
			out[(x)*width+(y)][0]=r;
			out[(x)*width+(y)][1]=g;
			out[(x)*width+(y)][2]=b;
		}
	}

	for(i=0;i<size;i++)											//write image data back to the file
	{
		putc(out[i][2],fOut);
		putc(out[i][1],fOut);
		putc(out[i][0],fOut);
	}
		
	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); //1000000
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
