#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

int WORDS_COUNT = 0;
int WORDS_MAX_LENGTH = 0;

#define MIN_VALUE           0
#define MAX_VALUE           9

#define true (char)         1
#define false (char)        0

#define error (char)		1
#define success (char)      0

#define undefined (char)   -1

//#define DEBUG_show_solution
#define DEBUG_show_time

char alth_table[256];

typedef char bool;
bool used[10];

//char table[WORDS_COUNT][WORDS_MAX_LENGTH] = {
//	"BIG", "CAT"
//};
//char result[WORDS_MAX_LENGTH] = "LION";
//char overflow[WORDS_MAX_LENGTH];
char** table;
char* result;
char* overflow;

int replaced_count = 0;

struct step_t {
	char min;
	char max;
	char start;
	char value;
};

struct words {
	char* word;
	int lenth;
}words[8];



#define init_step(STEP, MAX, MIN) (STEP).min = MIN; (STEP).max = MAX; (STEP).start = (STEP).min; (STEP).value = (STEP).start;

int valid_characters(char r)
{
	if (isupper(r))
		return 5;
	else
		switch (r)
		{
		case '+':
			return 4;
			break;
		case '=':
			return 3;
			break;
		case ' ':
			return 2;
			break;
		case '\n':
			return 1;
			break;
		default:
			return 0;
			break;
		}
}
inline adding_word(char* flags, char* buffer, int* length)
{
	flags[8] = 0;// cлово закончилось
	flags[0] = 1;//появилось слово слева
	flags[5] = 1;//слева слово есть
	flags[1] = 0;//идет вначале слово
	words[WORDS_COUNT].lenth = *length;
	words[WORDS_COUNT].word = (char*)calloc(sizeof(char), *length + 1);
	memcpy(words[WORDS_COUNT].word, buffer, *length);
	memset(buffer, 0, *length);
	*length = 0;
	WORDS_COUNT++;
}
char check_input(char* str)
{

	int length = 0;
	char buffer[100] = { 0 };
	unsigned char flags[] = { 0,1,1,1,0,0,0,0,0 };
	//flags[0] = есть значения слева =
	//flags[1] = вначале идет слово, а не символ
	//flags[2] = вначале идет плюс, а не равно
	//flags[3] = окончание цикла
	//flags[4] = код окончания
	//flags[5] = слева слова от плюса
	//flags[6] = был плюс
	//flags[7] = было =
	//flags[8] = появилось слово
	while (flags[3])
	{
		switch (valid_characters(*str))
		{
		case 5:
			if (WORDS_COUNT <= 7)
			{


				flags[8] = 1;//появилось слово
				flags[6] = 0;//снимаем что был плюс
				buffer[length] = *str;
				length++;
			}
			else//если не правильно
			{
				flags[3] = 0;
				flags[4] = 0;
			}
			break;
		case 4://+
			if (flags[8] == 1)//шла запись слова
			{
				//добавление слова в структуру
				adding_word(flags, buffer, &length);
				flags[8] = 0;
			}
			if (flags[1] == 0 && flags[5] == 1 && flags[7] == 0 && flags[6] == 0)// если в начале было слово и слева от символа было слово и плюса не было
			{

				flags[2] = 0;//плюс появился раньше равно, снимаем флаг
				flags[6] = 1;// ставим флаг что был плюс
				flags[5] = 0;
			}
			else//если не правильно
			{
				flags[3] = 0;
				flags[4] = 0;
			}
			break;
		case 3://=
			if (flags[8] == 1)// если было заполнение слова
			{
				//добавление слова в структуру
				adding_word(flags, buffer, &length);
				flags[8] = 0;
			}
			if (flags[0] && (flags[2] == 0) && (flags[5] == 1))//проверяем что слева были значения и то что = появилось позже +
			{

				flags[7] = 1;//появилось равно
			}
			else//если не правильно
			{
				flags[3] = 0;
				flags[4] = 1;
			}
			break;
		case 2://' '
			if (flags[8] == 1)
			{
				//добавление слова в структуру
				adding_word(flags, buffer, &length);
				flags[8] = 0;
			}
			break;
		case 1://'/0'
			if (flags[8] == 1)
			{
				//добавление слова в структуру
				adding_word(flags, buffer, &length);
				flags[8] = 0;
			}
			flags[3] = 0;
			flags[4] = 1;
			break;
		case 0:
			flags[3] = 0;
			flags[4] = 0;
			break;
		}
		str++;
	}
	return flags[4];
}

int max_len = 0;

char calculate_overflow_max_value(int pos) { // НЕПРАВИЛЬНО
	char over = 0;
	if (pos < max_len) {
		for (int a = 0; a < WORDS_COUNT; a++) {
			if (table[a][pos + 1]) over++;
		}
	}
	over *= 9; over /= 10;
	if (over < 9) over++;
	return over;
}

char calculate_max_value(int pos) {
	char maxvalue = MIN_VALUE;
	for (int a = 0; a < WORDS_COUNT; a++)
	{
		if (table[a][pos]) return MAX_VALUE;
		if (table[a][pos + 1]) maxvalue++;
	}
	return maxvalue;
}

char calculate_min_value(int pos, char * s) {
	if (pos == 0 ||
		!s[pos - 1]) return 1;
	return MIN_VALUE;
}


void display_line(char *s) {
	printf("\n");
	for (int a = 0; a < max_len; a++) {
		if (s[a]) {
			if (alth_table[s[a]] != undefined) printf(" %d", alth_table[s[a]]);
			else printf(" %c", s[a]);
		}
		else printf(" 0");
	}
}

void display_current_state() {
	printf("\ncurrent state:\n");
	for (int a = 0; a < max_len; a++) {
		printf(" %d", overflow[a]);
	}
	printf(" : overflow\n-----------");
	for (int a = 0; a < WORDS_COUNT; a++) {
		display_line(table[a]);
	}
	printf("\n-----------");
	display_line(result);
}
bool is_invalid_value(char value) {
	return value == undefined;
}

char get_next_step(struct step_t *s) {
	char beg = s->value;
	if (s->value >= s->start) {
		for (char a = beg; a <= s->max; a++) {
			if (!used[a]) return a;
		}
		beg = s->min;
	}
	for (char a = beg; a < s->start; a++) { // extra loop if value < start :C
		if (!used[a]) return a;
	}
	return undefined;
}

char set_replacement(char a, char value) {
	if (alth_table[a] != undefined || used[value] || is_invalid_value(value)) return error;
	alth_table[a] = value;
	used[value] = true;
	return 0;
}

int free_replacement(char a) {
	used[alth_table[a]] = false;
	alth_table[a] = undefined;
	return 0;
}

char get_expected_overflow(int pos) {
	if (!pos) return 0;
	return overflow[pos - 1];
}

int calculate_column(int pos) {
	int value = 0, cell = 0;
	for (int a = 0; a < WORDS_COUNT; a++) {
		if (!table[a][pos]) continue;
		cell = (int)alth_table[table[a][pos]];
		if (cell == undefined) {
			printf("\nUNDEFINED VALUE IN COLUMN!");
			exit(EXIT_FAILURE);
		}
		value += cell;
	}
	value += overflow[pos];
	return value;
}

bool is_column_invalid(int pos, int value) {
	if (alth_table[result[pos]] != value % 10) return true;
	if (get_expected_overflow(pos) != value / 10) return true;
	return false;
}

int define_base(int);
int solve(int, int);

char calculate_new_overflow(int value, char max_overflow, char prev_overflow, char result_value) {
	char new_overflow;
	for (new_overflow = 1; new_overflow <= max_overflow; new_overflow++) {
		if ((value + new_overflow) / 10 == prev_overflow &&
			(value + new_overflow) % 10 == result_value) return new_overflow;
	}
	return 0;
}

int try_with_overflow(int pos, int value) {
	char max_overflow = calculate_overflow_max_value(pos), new_overflow = 0,
		prev_overflow = get_expected_overflow(pos);
	int res = error;
	if ((max_overflow + value) / 10 >= prev_overflow) {
		new_overflow = calculate_new_overflow(value, max_overflow, prev_overflow, alth_table[result[pos]]);
		if (new_overflow) {
			overflow[pos] = new_overflow;
#ifdef DEBUG_show_solution
			printf("\ntry with overflow: %d;", new_overflow);
#endif
			res = define_base(pos + 1);
			overflow[pos] = 0;
		}
	}
	return res;
}

int define_base(int pos) {
#ifdef DEBUG_show_solution
	printf("\n\t!\t!base: %d;", pos);
#endif
	if (alth_table[result[pos]] != undefined) {
		return solve(pos, 0);
	}
	else {
		struct step_t step;
		init_step(step, calculate_max_value(pos), calculate_min_value(pos, result))
			step.value = get_next_step(&step);
		while (!is_invalid_value(step.value)) {
			set_replacement(result[pos], step.value);
			if (solve(pos, 0)) { // start the magic
				step.value = get_next_step(&step); // CORRECT ORDER
				free_replacement(result[pos]);
			}
			else return success;
		}
	}
	return error;
}

int solve(int pos, int l) {
#ifdef DEBUG_show_solution
	printf("\n\ncell: l: %d; p: %d;", l, pos);
	display_current_state();
#endif 
	if (l >= WORDS_COUNT) { // ЕСЛИ СОПАСТАВИЛИ ВСЕ БУКВЫ В СТОЛБЦЕ
		int column_value = calculate_column(pos);
		if (is_column_invalid(pos, column_value)) {
			// HERE: check with overflow
			return try_with_overflow(pos, column_value);
			//return error; // возврат ((
		}
		else { // погнали дальше if конец то EXIT
			if (pos >= max_len - 1) {
				printf("\n\t\t\tDONE!!");
				display_current_state();
				return success;
			}
			return define_base(pos + 1);
		}
	}
	if (alth_table[table[l][pos]] != undefined || !table[l][pos]) { // определено или 0 // OPT
		return solve(pos, l + 1);
	}
	else {
		struct step_t step;
		init_step(step, calculate_max_value(pos), calculate_min_value(pos, table[l]))
			step.value = get_next_step(&step);
		while (!is_invalid_value(step.value)) {
			set_replacement(table[l][pos], step.value);
			if (solve(pos, l + 1)) {
				step.value = get_next_step(&step);
				free_replacement(table[l][pos]);
			}
			else return success;
		}
	}
	return error;
}

int start_solution() {
	int pos = 0;
	return define_base(pos);
}

int main() {
	clock_t cur_time = 0;
	char str[100] = { 0 };
	printf("Enter your rebus:\n\n");
	fgets(str, 100, stdin);
	if (!check_input(str))
	{
		printf("\nincorrect example");
		return -1;
	}
	cur_time = clock();
	WORDS_COUNT--;
	max_len = words[WORDS_COUNT].lenth;
	result = (char*)calloc(max_len, sizeof(char));
	memcpy(result, words[WORDS_COUNT].word, max_len);

	overflow = (char*)calloc(max_len, sizeof(char));
	memset(alth_table, undefined, 256);
	memset(used, false, 10);

	table = (char**)calloc(WORDS_COUNT, sizeof(char*));
	for (int i = 0; i < WORDS_COUNT; i++) {
		table[i] = (char*)calloc(max_len, sizeof(char));
		unsigned int margin = max_len - words[i].lenth;
		memcpy(table[i] + margin, words[i].word, words[i].lenth);
	}



	start_solution();
	cur_time = clock() - cur_time;
	printf("\n\nAnswer:\n");

	for (int i = 0; i < WORDS_COUNT; i++)
	{
		for (int j = 0; j < words[i].lenth; j++)
		{
			printf("%d", alth_table[words[i].word[j]]);
		}
		if (i + 1 < WORDS_COUNT)
			printf(" + ");

	}
	printf(" = ");
	for (int j = 0; j < words[WORDS_COUNT].lenth; j++)
	{
		printf("%d", alth_table[words[WORDS_COUNT].word[j]]);
	}
#ifdef DEBUG_show_time
	printf("\n");
	printf("\nProcess time in ticks: %ld", cur_time);
	printf("\nProcess time in seconds: %f", (double)cur_time / CLOCKS_PER_SEC);
#endif
	printf("\n");
}