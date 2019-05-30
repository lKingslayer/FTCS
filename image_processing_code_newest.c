/**
* @file image_blur_gray.c
* @brief C program to blur the grayscale image, using convolution
* @author Priya Shah
* @version v1
* @date 2018-01-10
*/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>


const int bytesPerPixel = 3; /// red, green, blue
const int fileHeaderSize = 14;
const int infoHeaderSize = 40;
const int height = 101;
const int width = 201;

static FILE* fpRead;


/*
 * int 4 byte 
 * unsighed short 1 byte = char 1 byte, 0-255=2^8
 * long long 8 byte ???
 */

void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int width, int paddingSize);
unsigned char* createBitmapInfoHeader(int height, int width);



void generateBitmapImage(unsigned char *image, int height, int width, char* imageFileName){
    
    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width*bytesPerPixel) % 4) % 4;
    
    unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    
    FILE* imageFile = fopen(imageFileName, "wb");
    
    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);
    
    int i;
    for(i=0; i<height; i++){
        fwrite(image+(i*width*bytesPerPixel), bytesPerPixel, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }
    
    fclose(imageFile);
}

unsigned char* createBitmapFileHeader(int height, int width, int paddingSize){
    int fileSize = fileHeaderSize + infoHeaderSize + (bytesPerPixel*width+paddingSize) * height;
    
    static unsigned char fileHeader[] = {
        0,0, /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };
    
    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);
    
    return fileHeader;
}

unsigned char* createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0, /// number of color planes
        0,0, /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };
    
    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytesPerPixel*8);
    
    return infoHeader;
}

int get_max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

void generate_image(char* imageFileName)
{
    unsigned char image[height][width][bytesPerPixel];
    int m, n;
    for(m = 0; m < height; m++){
        for(n = 0; n < width; n++){
            double r,g,b;
            //            double temp = (n * 100) / (double)width_prime;
            double temp;
            char str[10];
            double max = 100, min = 0;
            fgets(str,15,fpRead);
            str[strlen(str)-1]='\0';
            //            printf("%s\n",str);
            temp = atof(str);
            if (m == 0 && n == 0)
                printf("%lf\n",temp);
//            printf("%lf\n",temp);
            double ratio = 2 * (temp - min) / (max - min);
            b = get_max(0, 255 * (1 - ratio));
            r = get_max(0, 255 * (ratio - 1));
            g = 255 - b - r;
            image[m][n][2] = (unsigned char)(r); ///red
            image[m][n][1] = (unsigned char)(g); ///green
            image[m][n][0] = (unsigned char)(b); ///blue
        }
    }
    generateBitmapImage((unsigned char *)image, height, width
                        , imageFileName);
}




void temp2rgb(double value, double r, double g, double b) {
    double max = 100, min = 0;
    double ratio = 2 * (value - min) / (max - min);
    b = get_max(0, 255 * (1 - ratio));
    r = get_max(0, 255 * (ratio - 1));
    g = 255 - b - r;
    return;
}

int main()
{
	clock_t start, stop;

	start = clock();											// Note the start time for profiling purposes.
//    unsigned char image[height][width][bytesPerPixel];
    char* imageFileName_0 = "./images/bitmapImage_0.bmp";
    char* imageFileName_1 = "./images/bitmapImage_1.bmp";
    char* imageFileName_2 = "./images/bitmapImage_2.bmp";
    char* imageFileName_3 = "./images/bitmapImage_3.bmp";
    char* imageFileName_4 = "./images/bitmapImage_4.bmp";
    char* imageFileName_5 = "./images/bitmapImage_5.bmp";
    char* imageFileName_6 = "./images/bitmapImage_6.bmp";
    char* imageFileName_7 = "./images/bitmapImage_7.bmp";
    char* imageFileName_8 = "./images/bitmapImage_8.bmp";
    char* imageFileName_9 = "./images/bitmapImage_9.bmp";
    char* imageFileName_10 = "./images/bitmapImage_10.bmp";
    printf("width: %d\n",width);
    printf("height: %d\n",height);
    fpRead = fopen("snapshot_60000.txt","r");
    if (fpRead == NULL)
    {
        puts("cannot open");
        exit(0);
    }
    char string_time[20];
    fgets(string_time,15,fpRead);
    generate_image(imageFileName_0);
    generate_image(imageFileName_1);
    generate_image(imageFileName_2);
    generate_image(imageFileName_3);
    generate_image(imageFileName_4);
    generate_image(imageFileName_5);
    generate_image(imageFileName_6);
    generate_image(imageFileName_7);
    generate_image(imageFileName_8);
    generate_image(imageFileName_9);
    generate_image(imageFileName_10);
    
    
    fclose(fpRead);
    printf("Image generated!!");
    
    
//    FILE *fIn = fopen("./images/bitmapImage.bmp","r");                    //Input File name
//    FILE *fOut = fopen("./images/photo_prime.bmp","w+");                    //Output File name
//    int i,j,y;
//    unsigned char byte[54];
//
//    if(fIn==NULL)                                                // check if the input file has not been opened succesfully.
//    {
//        printf("File does not exist.\n");
//    }
//
//    for(i=0;i<54;i++)                                            //read the 54 byte header from fIn
//    {
//        byte[i] = getc(fIn);
//    }
//
//    fwrite(byte,sizeof(unsigned char),54,fOut);                    //write the header back
//
//    // extract image height, width and bitDepth from imageHeader
//    int height = *(int*)&byte[18];
//    int width = *(int*)&byte[22];
//    int bitDepth = *(int*)&byte[28];
//
//    printf("width: %d\n",width);
//    printf("height: %d\n",height );
//
//    int size = height*width;                                    //calculate the image size
//
//    unsigned char buffer[size][3];                                //store the input image data
//    unsigned char out[size][3];                                    //store the output image data
//
//    for(i=0;i<size;i++)                                            //read image data character by character
//    {
//        buffer[i][2]=getc(fIn);                                    //blue
//        buffer[i][1]=getc(fIn);                                    //green
//        buffer[i][0]=getc(fIn);                                    //red
//    }
//
//
//    float v = 1.0/ 9.0;
//    float kernel[3][3]={{v,v,v},
//                        {v,v,v},
//                        {v,v,v}};
//
//    for(int x=1;x<height-1;x++)
//    {
//        for(y=1;y<width-1;y++)
//        {
//            /*
//            float sum0= 0.0;
//            float sum1= 0.0;
//            float sum2= 0.0;
//            for(i=-1;i<=1;++i)
//            {
//                for(j=-1;j<=1;++j)
//                {                                //matrix multiplication with kernel with every color plane
//                    sum0=sum0+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][0];
//                    sum1=sum1+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][1];
//                    sum2=sum2+(float)kernel[i+1][j+1]*buffer[(x+i)*width+(y+j)][2];
//                }
//            }
//            */
//            short r, g, b;
//            int temp = y * 100 / (float)width;
//            temp2rgb(temp, &r, &g, &b);
//            out[(x)*width+(y)][0]=r;
//            out[(x)*width+(y)][1]=g;
//            out[(x)*width+(y)][2]=b;
//        }
//    }
//
//    for(i=0;i<size;i++)                                            //write image data back to the file
//    {
//        putc(out[i][2],fOut);
//        putc(out[i][1],fOut);
//        putc(out[i][0],fOut);
//    }
//
//    fclose(fOut);
//    fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); //1000000
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
