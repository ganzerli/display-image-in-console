#include <stdlib.h>
#include <stdio.h>

typedef unsigned char byte;
typedef unsigned int uint;

#define SCREENSIZE  1400

// change this first lines to apply the software to your hardware or preferences
#define filename "lena_color.bmp"
// HOW MANY CHARACTERS ARE OR A FEW LESS TO FILL ONE LINE IN YOUR CONSOLE WINDOW AT FULL SCREEN 
#define NUMBER_OF_CHARS_IN_ONE_LINE 160

void main(int argc, char const *argv[]){
    byte img_header[54];
    FILE* fp = fopen(filename, "rb");               //read the file
    fread(img_header, sizeof(unsigned char), 54, fp);       // read the 54-byte from fp to imageHeader

    // get info from butmap
    uint width = *(uint*)&img_header[18];
    uint height = *(uint*)&img_header[22];
    uint bitDepth = *(uint*)&img_header[28];

    // read COLOR TABLE if present
    byte* color_table[1024];

    if(bitDepth <= 8)fread(color_table, sizeof(unsigned char), 1024, fp);       // read the 1024-byte from fp to colorTable

    // output file..
    FILE *fo = fopen("greyscale.bmp", "wb");

    fwrite(img_header, sizeof(unsigned char), 54, fo); // write the header back.
    if(bitDepth <= 8) fwrite(color_table, sizeof(unsigned char), 1024, fo);     // write the color table back

    byte y = 0;
    uint size = width * height;
    byte buffer[size][3];
    // *
    byte rgb_img[size];

    for( uint i = 0 ; i < size ; i++ ){
        // grab with feeling the bits containing r , g and also b
        buffer[i][2] = getc(fp);    //blue
        buffer[i][1] = getc(fp);    //green
        buffer[i][0] = getc(fp);    //red
        //conversion formula of rgb to gray, obviously related to human perception of colors
        y = (buffer[i][0]*0.3) + (buffer[i][1]*0.59) + (buffer[i][2]*0.11); 
        putc(y,fo);    // new blue
        putc(y,fo);    // new green
        putc(y,fo);    // new red
        // stuff for img to chars
        rgb_img[i] = y;
    }
    fclose(fp);
    fclose(fo);


    // Until that was taken from a tutorial in internet found at https://abhijitnathwani.github.io/blog/

    // Now there are few problems to convert a greyscale image in charachters image...

    // Problem 1. Getting a char scale to replace the greyscale
    char gradient[12] = { ' ','.',',','-',':','+','*','o','$','%','&','@' };

    // Needed is a way to convert the darkness in the char scale
    // Darkest rgb is 0 , and darkest gradient is also at index 0.
    // Lightest rgb is 255 and lightest char is at index 12,

    // If rgb was 100 the lightest and the char scale was 10, then with about 100/10 is found the index
    // If rgb was 200 then 10 is appearing 20 times in 200.. so 200/10 = 20 , 20/10 = 2
    // A formula found could be 200 / 10 = 20 , 20 / 10 = 2. So 10*2 is 20, then to find the index is 200 / (200/indexes in gradient: 10)
    // But in the reality is 255 , and the indexes, now are 12..
    // So 255 / (255/12) = 21.25 , 255 / 21.25 = 12 !  Oh no.. a float!!!
    // and 21.24 / (256/12) = 0.9almost1
    // Then even better is 256. Continues...

    // One guy at the bar sayed he does so in university
    float indexes = sizeof gradient / sizeof (char);
    float rgb = 255.0;
    const float RGB = 256;

    printf("\n255 / (256 / 12) = %f\n", rgb / ( RGB/indexes ) );
    // 11.953125 ... almost 12!! 12 was better... or one index is never used.

    int rgbtochi(byte rgbvalue){
        float rgv = (float)rgbvalue;                                                // having a float
        float fid = rgv / ( RGB / indexes );                                        // a float index
        // from rgb 0 to 21.25 , we need to have indx 0..
        // from 21.25 to 42.50 index 1.. and so on..

        // not having any matematician collegue at GLS storage nor building foritures xor at home, to center the probailities the most lazy solution is to add 0.5 to the result and cast int.
        // or add 21.24 / 2
        // but trying a lot of rgbtochi(10) to (255) the best solution was fid + fid/21.24 
        float fi = fid + fid / 21.25 ;                                              // that was me.. and the math behind that is 255 / (255/12) = 21.25 , 255 / 21.25 = 12
        //printf("%f\n" ,fi);        
        uint index = (int)fi;
        //printf("%u\n" , index );
        return index;
    }

    // this is the test
    rgbtochi(10);
    rgbtochi(20);
    rgbtochi(30);
    rgbtochi(40);
    rgbtochi(50);
    rgbtochi(60);
    rgbtochi(70);
    rgbtochi(80);
    rgbtochi(90);
    rgbtochi(100);
    rgbtochi(120);
    rgbtochi(160);
    rgbtochi(190);
    rgbtochi(200);
    rgbtochi(220);
    rgbtochi(240);
    rgbtochi(250);
    rgbtochi(255);

    // Now there is another problem, and is to solve not to bring to the disco manipulate it and speak it out. Ops.. i did it again...
    // And is to standardize the output size of the image in a world made with different image sizes, and the bmp format made from windows, and also other things.
    // Until now the project works only in my 1520 px screen.
    // So if a char is 16px can be that for y a line takes about 20px
    // if the image width was 1520px and not 512 was 1520 / 512 times bigger: 2.96875 oh no a float!
    // Height will be scrolled

    // But not everyone has a 1520px screen so that can be 1400 or as SCREENSIZE
    float screen = SCREENSIZE;
    float image_ratio = screen / width;                             // width is from the bitmap
    printf("%f\n" , width * image_ratio);                           // 1400.0

    // I could not find an equation for my screen and pixels, so i have counted 170 chars fitting int the terminal wikndow @14 or 16px
    float char_in_widht = NUMBER_OF_CHARS_IN_ONE_LINE;

    // If the image was 1400px wide and 170 chars are filling the width then 1400/170 ,to do is take a pixel to translate every 8.23px
    float px_distance_in_screen = ( screen / char_in_widht) ;
    printf("%f\n" , px_distance_in_screen);                         // 8.23

    // NEEDED IS: TAKE FROM 0 TO width RGB OF EVERY 8.23 / image_ratio px
    printf("%f\n" ,px_distance_in_screen * char_in_widht);          // 1400

    // and apply the relation to the size of the image choosen
    float px_distance_in_img = px_distance_in_screen / image_ratio; // 3.2 in this case
    printf("translating rgb every %f pixels\n", px_distance_in_img );

    // it may be endianity or something strange.. but in my pc the img_buffer needs to be turned
    byte rgb_img_[size];
    uint i_ = 0;
    // resolving endianity , if your computer prints the image upside down comment the 4 lines underneath
    for (uint i = size; i > 0; i--){
        rgb_img_[i_] = rgb_img[i];
        i_++;
    }
    
    // some variables
    uint idx = 0;
    byte value = 0;

    //  . . . fatality
    char c_to_print (byte rgb_img_i){
        uint id = rgbtochi(rgb_img_i);
        return gradient[id];
    }
    for(float i = 0; i < (float)height; i+= px_distance_in_img+2.5 ){
        for (float j = 0; j < (float) width; j+= px_distance_in_img){
            idx = (int) i*width + j;
            value = rgb_img_[idx];
            printf("%c" , c_to_print(value) );        
        }
        printf("\n");
    }

}
