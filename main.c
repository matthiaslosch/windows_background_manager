#include <windows.h>
#include <stdio.h>
#include <signal.h>

#define STB_DEFINE
#include "stb.h"

#define MS_IN_DAY 86400000

static char previous_wallpaper[MAX_PATH];

void set_wallpaper(char *path)
{
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE);
}

void get_current_wallpaper(char *path)
{
    SystemParametersInfoA(SPI_GETDESKWALLPAPER, 100, path, 0);
}

void handle_sigint(int signal)
{
    set_wallpaper(previous_wallpaper);
}

// argv[1]: directory path
// argv[2]: interval in ms
int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "arg1: directory path, [arg2: interval in ms]\n");
        return 1;
    }

    get_current_wallpaper(previous_wallpaper);
    signal(SIGINT, handle_sigint);

    printf("%s\n", argv[1]);
    char **files = stb_readdir_files(argv[1]);
    int number_of_images = stb_arr_len(files);
    long interval = argc == 3 ? strtol(argv[2], NULL, 10) : MS_IN_DAY / number_of_images;

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
