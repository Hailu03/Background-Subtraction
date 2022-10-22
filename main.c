#include <stdio.h>
#include<math.h>
#include<stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

unsigned char *uc_arrayNew_1d(int _size) {
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

unsigned char* absdiff(unsigned char *background,unsigned char *foreground,int width,int height,int channel) {
    unsigned char *diffImg = uc_arrayNew_1d(height*width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int sum = 0;
            for (int k = 0; k < channel; k++) {
                sum += abs(background[i*width*channel+j*channel+ k] - foreground[i*width*channel+j*channel+k]);
            }
            if((sum/3) < 55) {
                diffImg[i*width + j] = 0;
            } else {
                diffImg[i*width + j] =255;
            }
            // diffImg[i*width + j] = sum/3;
        }
    }
    return diffImg;
}

unsigned char* imgCombination(unsigned char *diffImg,unsigned char *weather_forecast,unsigned char *foreground,int width,int height,int channel) {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if(diffImg[i*width + j]  == 255) {
                for (int k = 0; k < channel; k++) {
                    weather_forecast[i*width*channel+j*channel+ k] = foreground[i*width*channel+j*channel+k];
                }
            }
        }
    }
}

int main() {
    int width, height, channel;

    char path_backgroundImg[] = "./images/background.png";
    char path_foregroundImg[] = "./images/foreground.png";

    char save_path_difference[] = "./images/difference.png";
    char save_path_binary_difference[] = "./images/binary_difference.png";

    unsigned char * background = stbi_load(path_backgroundImg , &width , &height , & channel, 0);
    unsigned char * foreground = stbi_load(path_foregroundImg , &width , &height , & channel, 0);

    if(background == NULL || foreground == NULL) {
        printf ("\nError in loading the image\n");
        exit(1);
    }

    printf("Width = %d\nHeight = %d\nChannel = %d\n", width , height , channel);


    // compute difference between two images
    unsigned char *diffImg = absdiff(background, foreground, width, height, channel);
    // save image
    stbi_write_png(save_path_binary_difference, width, height,1, diffImg, width);
    printf("New image saved to %s\n", save_path_binary_difference);

    char path_bgforecastImg[] = "./images/weather_forecast-resized.jpg";
    char save_path_output[] = "./images/output.png";
    
    unsigned char * weather_forecast = stbi_load(path_bgforecastImg , &width , &height , &channel, 0);

    if(weather_forecast == NULL) {
        printf ("\nError in loading the image\n");
        exit(1);
    }

    printf (" Width = %d\nHeight = %d\nChannel = %d\n", width , height , channel);

    imgCombination(diffImg,weather_forecast,foreground,width,height,channel);

    // save image
    stbi_write_png(save_path_output, width, height,channel, weather_forecast, width*channel);
    printf("New image saved to %s\n", save_path_output);
}