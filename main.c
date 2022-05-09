#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/day01.h"
#include "./include/day02.h"
#include "./include/day03.h"
#include "./include/day04.h"
#include "./include/day05.h"
#include "./include/day06.h"
#include "./include/day07.h"
#include "./include/day08.h"
#include "./include/day09.h"
#include "./include/day10.h"
#include "./include/day11.h"
#include "./include/day12.h"
#include "./include/day13.h"
#include "./include/day14.h"
#include "./include/day15.h"
#include "./include/day16.h"
#include "./include/day17.h"
#include "./include/day18.h"
#include "./include/day19.h"
#include "./include/day20.h"
#include "./include/day21.h"
#include "./include/day22.h"
#include "./include/day23.h"
#include "./include/day24.h"
#include "./include/day25.h"

#define BUF_SIZE 1024

int read_input(char **content) {
    int size = 0;
    *content = calloc(sizeof(char), BUF_SIZE + 1);
    if (*content == NULL) {
        return -1;
    }

    char buffer[BUF_SIZE];
    while (fgets(buffer, BUF_SIZE, stdin) > 0) {
        size += strlen(buffer);
        char *tmp = realloc(*content, size + 1);

        if (tmp == NULL) {
            free(*content);
            return -1;
        }

        *content = tmp;
        strcat(*content, buffer);
    }

    return size;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Usage: ./main <day_number>\n");
        return 1;
    }

    int day = atoi(argv[1]);

    char *content;
    int size = read_input(&content);

    if (size < 0) {
        printf("Failed to read input\n");
        return 1;
    }

    const char *result = NULL;
    switch (day) {
        case 1:
            result = day01_solve(content);
            break;
        case 2:
            result = day02_solve(content);
            break;
        case 3:
            result = day03_solve(content);
            break;
        case 4:
            result = day04_solve(content);
            break;
        case 5:
            result = day05_solve(content);
            break;
        case 6:
            result = day06_solve(content);
            break;
        case 7:
            result = day07_solve(content);
            break;
        case 8:
            result = day08_solve(content);
            break;
        case 9:
            result = day09_solve(content);
            break;
        case 10:
            result = day10_solve(content);
            break;
        case 11:
            result = day11_solve(content);
            break;
        case 12:
            result = day12_solve(content);
            break;
        case 13:
            result = day13_solve(content);
            break;
        case 14:
            result = day14_solve(content);
            break;
        case 15:
            result = day15_solve(content);
            break;
        case 16:
            result = day16_solve(content);
            break;
        case 17:
            result = day17_solve(content);
            break;
        case 18:
            result = day18_solve(content);
            break;
        case 19:
            result = day19_solve(content);
            break;
        case 20:
            result = day20_solve(content);
            break;
        case 21:
            result = day21_solve(content);
            break;
        case 22:
            result = day22_solve(content);
            break;
        case 23:
            result = day23_solve(content);
            break;
        case 24:
            result = day24_solve(content);
            break;
        case 25:
            result = day25_solve(content);
            break;
        default:
            result = "Unknown day";
    }
    printf("%s\n", result);

    free(content);
    return 0;
}
