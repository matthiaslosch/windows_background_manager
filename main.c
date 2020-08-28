#include <windows.h>
#include <stdio.h>

#define STB_DEFINE
#include "stb.h"

#define MS_IN_DAY 86400000

void set_wallpaper(char *path)
{
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE);
}

// argv[1]: directory path
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "arg1: directory path");
        return 1;
    }

    printf("%s\n", argv[1]);
    char **files = stb_readdir_files(argv[1]);
    int number_of_images = stb_arr_len(files);

    long interval = MS_IN_DAY / number_of_images;

    int i;
    size_t n;

    for (;;) {
        for (i = 0; i <= number_of_images; ++i) {
            if (i == number_of_images)
                i = 0;
            char *file = files[i];
            printf("%s\n", file);
            set_wallpaper(file);
            Sleep(interval);
        }
    }

    return 0;
}
