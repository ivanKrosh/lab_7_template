/*
    Лабораторна робота №7, крок 2. Режими відкриття та помилки.

    Запуск:
        Step2_modes.exe table               таблиця: 5 випадків x 4 режими
        Step2_modes.exe open <файл> <режим> відкрити один файл і закрити
        Step2_modes.exe three               номери дескрипторів трьох потоків

    Програма лише відкриває й закриває файли - нічого в них не пише.
    Для таблиці вона створює теку modes_test у поточному каталозі.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <io.h>
#include <sys/stat.h>
#include <windows.h>

/* Розмір файлу в байтах або -1, якщо файлу немає (службова функція) */
static long long file_size(const char *path)
{
    struct _stat64 st;
    if (_stat64(path, &st) != 0) {
        return -1;
    }
    return st.st_size;
}

/* Відкрити й одразу закрити; у cell записати результат для таблиці */
static void try_open(const char *path, const char *mode, char *cell, size_t size)
{
    FILE *f = fopen(path, mode);
    if (f == NULL) {
        int e = errno;
        snprintf(cell, size, "NULL %d", e);
        return;
    }
    fclose(f);
    snprintf(cell, size, "ok %lld", file_size(path));
}

/* Підготувати випадок перед кожною спробою: вміст файлу з 15 байтів */
static void make_file(const char *path)
{
    SetFileAttributesA(path, FILE_ATTRIBUTE_NORMAL);
    FILE *f = fopen(path, "wb");
    if (f != NULL) {
        fputs("one two three\r\n", f);
        fclose(f);
    }
}

static int table(void)
{
    const char *modes[] = { "r", "w", "a", "r+" };
    const char *cases[] = { "exists", "missing", "read-only", "directory", "no folder" };
    const char *paths[] = {
        "modes_test\\exists.txt",
        "modes_test\\missing.txt",
        "modes_test\\readonly.txt",
        "modes_test\\folder",
        "modes_test\\nodir\\x.txt"
    };

    CreateDirectoryA("modes_test", NULL);
    CreateDirectoryA("modes_test\\folder", NULL);

    printf("%-12s", "case");
    for (int m = 0; m < 4; m++) {
        printf("%-12s", modes[m]);
    }
    printf("\n");

    for (int c = 0; c < 5; c++) {
        printf("%-12s", cases[c]);
        for (int m = 0; m < 4; m++) {
            if (c == 0) {
                make_file(paths[0]);
            }
            if (c == 1) {
                remove(paths[1]);
            }
            if (c == 2) {
                make_file(paths[2]);
                SetFileAttributesA(paths[2], FILE_ATTRIBUTE_READONLY);
            }
            char cell[16];
            try_open(paths[c], modes[m], cell, sizeof cell);
            printf("%-12s", cell);
        }
        printf("\n");
    }
    SetFileAttributesA(paths[2], FILE_ATTRIBUTE_NORMAL);
    printf("\nok N   - fopen succeeded, file size after fclose is N bytes\n");
    printf("NULL E - fopen returned NULL, errno = E\n");
    printf("errno  2: %s\n", strerror(2));
    printf("errno 13: %s\n", strerror(13));
    return 0;
}

static int open_one(const char *path, const char *mode)
{
    printf("size before: %lld\n", file_size(path));
    FILE *f = fopen(path, mode);
    if (f == NULL) {
        int e = errno;
        perror(path);
        fprintf(stderr, "  errno = %d\n", e);
        return 1;
    }
    printf("fopen(\"%s\", \"%s\") = %p, descriptor %d\n", path, mode, (void *)f, _fileno(f));
    fclose(f);
    printf("size after : %lld\n", file_size(path));
    return 0;
}

static int three(void)
{
    FILE *a = fopen("three_a.txt", "w");
    FILE *b = fopen("three_b.txt", "w");
    FILE *c = fopen("three_c.txt", "w");
    if (a == NULL || b == NULL || c == NULL) {
        perror("three");
        return 1;
    }
    printf("stdin %d, stdout %d, stderr %d\n", _fileno(stdin), _fileno(stdout), _fileno(stderr));
    printf("a %d, b %d, c %d\n", _fileno(a), _fileno(b), _fileno(c));
    fclose(b);
    FILE *d = fopen("three_d.txt", "w");
    if (d == NULL) {
        perror("three_d.txt");
        return 1;
    }
    printf("after fclose(b): d %d\n", _fileno(d));
    fclose(a);
    fclose(c);
    fclose(d);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "table") == 0) {
        return table();
    }
    if (argc == 4 && strcmp(argv[1], "open") == 0) {
        return open_one(argv[2], argv[3]);
    }
    if (argc == 2 && strcmp(argv[1], "three") == 0) {
        return three();
    }
    fprintf(stderr, "usage: %s table | open <file> <mode> | three\n", argv[0]);
    return 2;
}
