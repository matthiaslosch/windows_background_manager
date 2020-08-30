#include <windows.h>
#include <stdio.h>
#include <signal.h>

#define STB_DEFINE
#include "stb.h"

#define MS_IN_DAY 86400000

#define HOURS_TO_MS(hour) (hour * 60 * 60 * 1000)
#define MIN_TO_MS(min) (min * 60 * 1000)
#define SEC_TO_MS(sec) (sec * 1000)

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

long interval_offset(long interval, int number_of_images, int *current_image)
{
    SYSTEMTIME local_time;
    GetLocalTime(&local_time);
    int current_time_in_ms = HOURS_TO_MS(local_time.wHour) + MIN_TO_MS(local_time.wMinute) + SEC_TO_MS(local_time.wSecond) + local_time.wMilliseconds;
    int offset = interval - (current_time_in_ms % interval);
    *current_image = (current_time_in_ms / interval) % number_of_images;

    return offset;
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

    char **files = stb_readdir_files(argv[1]);
    int number_of_images = stb_arr_len(files);
    long interval = argc == 3 ? strtol(argv[2], NULL, 10) : MS_IN_DAY / number_of_images;

    int i;

    int offset = interval_offset(interval, number_of_images, &i);

    for (;;) {
        for (; i <= number_of_images; ++i) {
            if (i == number_of_images)
                i = 0;
            char *file = files[i];
            set_wallpaper(file);

            if (offset) {
                Sleep(offset);
                offset = 0;
            } else {
                Sleep(interval);
            }
        }
    }

    return 0;
}
