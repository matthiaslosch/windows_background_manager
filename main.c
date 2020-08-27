#include <windows.h>
#include <stdio.h>
#include <string.h>

#define STB_DEFINE
#include "stb.h"

void set_wallpaper(char *path)
{
    SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, path, SPIF_UPDATEINIFILE);
}

// argv[1]: directory path
// argv[2]: interval
int main(int argc, char **argv)
{
    if (argc < 3) {
        fprintf(stderr, "arg1: directory path, arg2: interval in ms");
        return 1;
    }

    long interval = strtol(argv[2], NULL, 10);

    printf("%s\n", argv[1]);
    char **files = stb_readdir_files(argv[1]);

    int i;
    size_t n;

    for (;;) {
        for (i = 0; i <= stb_arr_len(files); ++i) {
            if (i == stb_arr_len(files))
                i = 0;
            char *file = files[i];
            printf("%s\n", file);
            set_wallpaper(file);
            Sleep(interval);
        }
    }

    return 0;
}
