/*
    Лабораторна робота №7, крок 4. Пошук помилок у роботі з файлами.

    П'ять функцій, у кожній - одна помилка. Компілятор з /W4 жодної з них
    не помічає. Номер функції задається аргументом командного рядка:
        Step4_bugs.exe 1   ...   Step4_bugs.exe 5

    Перед кожним запуском функція prepare() заново створює в поточному каталозі
    файли bug_marks.txt, bug_numbers.txt і теку reports, тож дані можна псувати
    скільки завгодно.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <windows.h>

#define MAX_REC 16

/* ------------------------- підготовка даних (без помилок) ------------------------- */

static void write_text(const char *path, const char *text)
{
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        perror(path);
        exit(3);
    }
    fputs(text, f);
    fclose(f);
}

static void prepare(void)
{
    write_text("bug_marks.txt",
               "Bondar 74\nHnatiuk 58\nKovalenko 91\nLysenko 66\n"
               "Melnyk 45\nPetrenko 83\nSavchuk 60\nTkachenko 97\n");
    write_text("bug_numbers.txt", "12 7\n30 x 5\n");
    CreateDirectoryA("reports", NULL);
}

/* ------------------------------- функції з помилками ------------------------------- */

/* 1. Кількість студентів і середня оцінка у файлі path */
static void average(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror(path);
        return;
    }
    char name[32];
    int mark, sum = 0, count = 0;
    while (!feof(f)) {
        fscanf(f, "%31s %d", name, &mark);
        sum += mark;
        count++;
    }
    fclose(f);
    printf("count = %d, average = %.2f\n", count, (double)sum / count);
}

/* 2. Найдовше прізвище у файлі path */
static void longest_name(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror(path);
        return;
    }
    char name[8], best[32] = "";
    int mark;
    while (fscanf(f, "%s %d", name, &mark) == 2) {
        if (strlen(name) > strlen(best)) {
            strcpy(best, name);
        }
    }
    fclose(f);
    printf("longest name: %s (%zu letters)\n", best, strlen(best));
}

/* 3. Сума цілих чисел у файлі path; слова, що не є числами, пропускаються */
static void sum_numbers(const char *path)
{
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        perror(path);
        return;
    }
    int x, sum = 0, count = 0, r;
    while ((r = fscanf(f, "%d", &x)) != EOF) {
        if (r == 1) {
            sum += x;
            count++;
        }
    }
    fclose(f);
    printf("count = %d, sum = %d\n", count, sum);
}

/* 4. Записати прізвища у файлі path великими літерами (файл змінюється на місці) */
static void upper_names(const char *path)
{
    FILE *in = fopen(path, "r");
    FILE *out = fopen(path, "w");
    if (in == NULL || out == NULL) {
        perror(path);
        return;
    }
    char name[32];
    int mark, n = 0;
    while (fscanf(in, "%31s %d", name, &mark) == 2) {
        for (int i = 0; name[i] != '\0'; i++) {
            name[i] = (char)toupper((unsigned char)name[i]);
        }
        fprintf(out, "%s %d\n", name, mark);
        n++;
    }
    fclose(in);
    fclose(out);
    printf("%d names converted\n", n);
}

/* 5. Зберегти копію файлу path у теці reports під іменем new_marks.txt */
static void save_copy(const char *path)
{
    FILE *in = fopen(path, "r");
    if (in == NULL) {
        perror(path);
        return;
    }
    const char *target = "reports\new_marks.txt";
    FILE *out = fopen(target, "w");
    if (out == NULL) {
        perror(target);
        fclose(in);
        return;
    }
    char line[64];
    int n = 0;
    while (fgets(line, sizeof line, in) != NULL) {
        fputs(line, out);
        n++;
    }
    fclose(in);
    fclose(out);
    printf("%d lines copied to %s\n", n, target);
}

int main(int argc, char *argv[])
{
    int k = (argc > 1) ? atoi(argv[1]) : 0;
    if (k < 1 || k > 5) {
        fprintf(stderr, "usage: %s 1..5\n", argv[0]);
        return 2;
    }
    prepare();
    switch (k) {
    case 1:
        average("bug_marks.txt");
        break;
    case 2:
        longest_name("bug_marks.txt");
        break;
    case 3:
        sum_numbers("bug_numbers.txt");
        break;
    case 4:
        upper_names("bug_marks.txt");
        break;
    case 5:
        save_copy("bug_marks.txt");
        break;
    }
    return 0;
}
