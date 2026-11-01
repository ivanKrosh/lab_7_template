/*
    Лабораторна робота №7, крок 1. Де лежить файл і що в ньому.

    Програма показує поточний каталог процесу і те, яким абсолютним шляхом
    стає відносне ім'я файлу. Потім читає файл двічі - у текстовому режимі "r"
    і у двійковому "rb" - і рахує, скільки символів отримала програма.
    Наприкінці створює файл where_out.txt і друкує, де він опинився.

    Запуск:  Step1_where.exe [ім'я файлу]     типово marks.txt
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <direct.h>
#include <locale.h>
#include <windows.h>

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);        /* кирилиця в шляхах - заняття 7 */
    setlocale(LC_ALL, ".UTF8");

    const char *name = (argc > 1) ? argv[1] : "marks.txt";
    char cwd[MAX_PATH], full[MAX_PATH];

    _getcwd(cwd, sizeof cwd);
    _fullpath(full, name, sizeof full);
    printf("current dir : %s\n", cwd);
    printf("%-12s: %s\n", name, full);

    FILE *f = fopen(name, "r");
    if (f == NULL) {
        perror(name);
        return 1;
    }
    long chars = 0;
    int lines = 0, c;
    while ((c = fgetc(f)) != EOF) {
        chars++;
        if (c == '\n') {
            lines++;
        }
    }
    fclose(f);

    f = fopen(name, "rb");
    if (f == NULL) {
        perror(name);
        return 1;
    }
    long bytes = 0;
    int cr = 0;
    while ((c = fgetc(f)) != EOF) {
        bytes++;
        if (c == '\r') {
            cr++;
        }
    }
    fclose(f);

    printf("mode \"r\"    : %ld characters, %d of them '\\n'\n", chars, lines);
    printf("mode \"rb\"   : %ld bytes, %d of them 0D\n", bytes, cr);

    FILE *out = fopen("where_out.txt", "w");
    if (out == NULL) {
        perror("where_out.txt");
        return 1;
    }
    fprintf(out, "written from %s\n", cwd);
    fclose(out);
    _fullpath(full, "where_out.txt", sizeof full);
    printf("created     : %s\n", full);
    return 0;
}
