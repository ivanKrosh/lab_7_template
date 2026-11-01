/*
    Лабораторна робота №7, крок 3. Функції читання й запису.

    У верхній частині - п'ять функцій із заготовками, які треба написати за контрактами
    в коментарях. У нижній - перевірки: їх змінювати не можна.
    Перевірки створюють тестові файли t_*.txt у поточному каталозі.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define NAME_LEN 32     /* прізвище: до 31 символу і '\0' */
#define LINE_LEN 64     /* рядок файлу: до 62 символів, '\n' і '\0' */

/* ======================= ФУНКЦІЇ, ЯКІ ТРЕБА НАПИСАТИ ======================= */

/*
    1. Кількість рядків у файлі f, відкритому для читання.
       Рядок - це послідовність символів, що закінчується '\n', а також останній
       рядок без '\n', якщо він не порожній. Порожній файл - 0 рядків.
       Довжина рядка не обмежена.
*/
int count_lines(FILE *f)
{
    (void)f;
    return 0;
}

/*
    2. Прочитати з f записи «прізвище оцінка», по одному в рядку, у масиви names і marks.
       - Рядок читається fgets і розбирається sscanf за форматом "%31s %d".
       - Рядок, у якому немає прізвища й цілого числа, - зіпсований: він не записується,
         bad[0] збільшується на 1. Рядок, довший за LINE_LEN - 2 символи, - теж зіпсований
         (один, а не кілька).
       - Порожній рядок або рядок із самих пробілів пропускається і зіпсованим не вважається.
       - У масиви записується не більше cap записів; решта лише рахується.
       Повертає кількість правильних записів у файлі - вона може бути більшою за cap.
       bad - масив з одного елемента: у bad[0] функція повертає кількість зіпсованих
       рядків (друге значення результату). bad[0] на початку обнуляє сама функція.
*/
int load_marks(FILE *f, char names[][NAME_LEN], int marks[], int cap, int bad[1])
{
    (void)f; (void)names; (void)marks; (void)cap;
    bad[0] = 0;
    return 0;
}

/*
    3. Записати в out звіт про n студентів:
           кожен рядок - fprintf "%-10s %3d\n";
           далі рядок "--------------";
           далі середнє - fprintf "%-10s %5.1f\n" з текстом "average".
       Для n == 0 - лише рядок "no data".
*/
void save_report(FILE *out, char names[][NAME_LEN], const int marks[], int n)
{
    (void)out; (void)names; (void)marks; (void)n;
}

/*
    4. Дописати рядок text і '\n' у кінець файлу path (файлу немає - створити).
       Файл відкривається й закривається всередині функції.
       Повертає 0 - успіх, -1 - файл не відкрився.
*/
int append_line(const char *path, const char *text)
{
    (void)path; (void)text;
    return 0;
}

/*
    5. Переписати з in в out записи з оцінкою не нижче pass у вигляді "%s %d\n".
       Правила читання - ті самі, що в load_marks; зіпсовані рядки пропускаються.
       Повертає кількість записаних записів.
*/
int filter_marks(FILE *in, FILE *out, int pass)
{
    (void)in; (void)out; (void)pass;
    return 0;
}

/* ================= ПЕРЕВІРКИ: НЕ ЗМІНЮЙТЕ НІЧОГО НИЖЧЕ ================= */

static int passed = 0, failed = 0;

#define CHECK(expr)                                                     \
    do {                                                                \
        if (expr) {                                                     \
            passed++;                                                   \
        } else {                                                        \
            failed++;                                                   \
            printf("FAIL line %d: %s\n", __LINE__, #expr);              \
        }                                                               \
    } while (0)

/* Створити тестовий файл з точно заданими байтами */
static void make_file(const char *name, const char *bytes)
{
    FILE *f = fopen(name, "wb");
    if (f == NULL) {
        perror(name);
        return;
    }
    fputs(bytes, f);
    fclose(f);
}

/* Прочитати весь файл у текстовому режимі в рядок buf */
static const char *read_all(const char *name, char *buf, int size)
{
    int n = 0, c;
    buf[0] = '\0';
    FILE *f = fopen(name, "r");
    if (f == NULL) {
        return buf;
    }
    while (n < size - 1 && (c = fgetc(f)) != EOF) {
        buf[n++] = (char)c;
    }
    buf[n] = '\0';
    fclose(f);
    return buf;
}

static int lines_of(const char *bytes)
{
    make_file("t_lines.txt", bytes);
    FILE *f = fopen("t_lines.txt", "r");
    if (f == NULL) {
        return -100;
    }
    int n = count_lines(f);
    fclose(f);
    return n;
}

static char names[8][NAME_LEN];
static int marks[8];

static int load_of(const char *bytes, int cap, int bad[1])
{
    for (int i = 0; i < 8; i++) {
        strcpy(names[i], "-");
        marks[i] = -1;
    }
    make_file("t_marks.txt", bytes);
    FILE *f = fopen("t_marks.txt", "r");
    if (f == NULL) {
        return -100;
    }
    int n = load_marks(f, names, marks, cap, bad);
    fclose(f);
    return n;
}

static const char *report_of(int n, char *buf, int size)
{
    FILE *f = fopen("t_report.txt", "w");
    if (f == NULL) {
        buf[0] = '\0';
        return buf;
    }
    save_report(f, names, marks, n);
    fclose(f);
    return read_all("t_report.txt", buf, size);
}

int main(void)
{
    char buf[512];
    int bad[1] = { -1 }, n;

    /* 1. count_lines */
    CHECK(lines_of("") == 0);
    CHECK(lines_of("Bondar 74\r\n") == 1);
    CHECK(lines_of("Bondar 74\r\nHnatiuk 58\r\n") == 2);
    CHECK(lines_of("Bondar 74\r\nHnatiuk 58") == 2);            /* без '\n' в кінці */
    CHECK(lines_of("\r\n\r\n") == 2);                            /* два порожні рядки */
    CHECK(lines_of("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
                   "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\r\n") == 1);

    /* 2. load_marks */
    n = load_of("Bondar 74\r\nHnatiuk 58\r\nKovalenko 91\r\n", 8, bad);
    CHECK(n == 3);
    CHECK(bad[0] == 0);
    CHECK(strcmp(names[0], "Bondar") == 0 && marks[0] == 74);
    CHECK(strcmp(names[2], "Kovalenko") == 0 && marks[2] == 91);
    CHECK(strcmp(names[3], "-") == 0);                           /* далі нічого не записано */

    n = load_of("Bondar 74\r\nMelnyk abc\r\nKovalenko 91", 8, bad);
    CHECK(n == 2);                                               /* зіпсований рядок посередині */
    CHECK(bad[0] == 1);
    CHECK(strcmp(names[1], "Kovalenko") == 0 && marks[1] == 91);

    n = load_of("\r\nBondar 74\r\n   \r\n\r\nHnatiuk 58\r\n\r\n", 8, bad);
    CHECK(n == 2);                                               /* порожні рядки - не помилка */
    CHECK(bad[0] == 0);

    n = load_of("  Bondar    74  \r\n", 8, bad);
    CHECK(n == 1 && strcmp(names[0], "Bondar") == 0 && marks[0] == 74);

    n = load_of("Bondar 74\r\n"
                "Ooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo 77\r\n"
                "Hnatiuk 58\r\n", 8, bad);
    CHECK(n == 2);                                               /* довгий рядок - один зіпсований */
    CHECK(bad[0] == 1);
    CHECK(strcmp(names[1], "Hnatiuk") == 0 && marks[1] == 58);

    n = load_of("Aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa 77\r\nHnatiuk 58\r\n", 8, bad);
    CHECK(n == 1);                                               /* прізвище довше за 31 символ */
    CHECK(bad[0] == 1);
    CHECK(strcmp(names[0], "Hnatiuk") == 0);

    n = load_of("A 1\r\nB 2\r\nC 3\r\nD 4\r\n", 2, bad);
    CHECK(n == 4);                                               /* записів більше, ніж місця */
    CHECK(strcmp(names[1], "B") == 0 && marks[1] == 2);
    CHECK(strcmp(names[2], "-") == 0 && marks[2] == -1);         /* за межу cap не записано */

    n = load_of("", 8, bad);
    CHECK(n == 0 && bad[0] == 0);

    /* 3. save_report */
    load_of("Bondar 74\r\nHnatiuk 58\r\n", 8, bad);
    CHECK(strcmp(report_of(2, buf, sizeof buf),
                 "Bondar      74\n"
                 "Hnatiuk     58\n"
                 "--------------\n"
                 "average     66.0\n") == 0);
    CHECK(strcmp(report_of(0, buf, sizeof buf), "no data\n") == 0);

    /* 4. append_line */
    remove("t_log.txt");
    CHECK(append_line("t_log.txt", "one") == 0);
    CHECK(append_line("t_log.txt", "two") == 0);
    CHECK(strcmp(read_all("t_log.txt", buf, sizeof buf), "one\ntwo\n") == 0);
    CHECK(append_line("nodir\\t_log.txt", "three") == -1);

    /* 5. filter_marks */
    make_file("t_in.txt", "Bondar 74\r\nHnatiuk 58\r\nMelnyk abc\r\nSavchuk 60\r\n");
    FILE *in = fopen("t_in.txt", "r");
    FILE *out = fopen("t_out.txt", "w");
    if (in != NULL && out != NULL) {
        n = filter_marks(in, out, 60);
        fclose(in);
        fclose(out);
        CHECK(n == 2);
        CHECK(strcmp(read_all("t_out.txt", buf, sizeof buf), "Bondar 74\nSavchuk 60\n") == 0);
    } else {
        failed++;
        printf("FAIL: t_in.txt / t_out.txt not opened\n");
    }

    printf("passed %d, failed %d\n", passed, failed);
    return failed;
}
