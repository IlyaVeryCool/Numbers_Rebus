#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<conio.h>

//#define DEBAG 0
typedef char bool;
#define true (char)               1
#define false (char)              0
#define no_simvol (char)         -2
#define there_is_simvol (char)   -1
#define no_return (int)          -1
#ifdef DEBUG
int count_check = 0;
#endif
struct words {
	char* word;
	int lenth;
}words[8];

struct str_rebus
{
	char simvols[256];
	bool digits[10];
	int number_of_different_simvols;
	int digit;
	int digit_return;
	char simvol;
}rebus;
int count_word = 0;

bool check_solution()
{
	int SUM = 0;;
	int skoliko_word = count_word - 1;
	for (int i = 0; i < skoliko_word; i++)
	{
		int buffer_sum = 0;
		for (int j = 0; j < words[i].lenth; j++)
		{
			buffer_sum *= 10;
			buffer_sum += rebus.simvols[words[i].word[j]];
		}
		SUM += buffer_sum;
	}
	int result_sum = 0;
	for (int j = 0; j < words[count_word - 1].lenth; j++)
	{
		result_sum *= 10;
		result_sum += rebus.simvols[words[count_word - 1].word[j]];
	}
	if (SUM != result_sum)
		return true;
	else
		return false;
}

void selection_digit(bool boolean, int* count_selected_simvols, bool* flag_search_simvol)
{

	char beg = rebus.digit;
	if (boolean)
	{
		if (rebus.digit >= 0) {
			for (char a = beg; a < 10; a++) {
				if (!rebus.digits[a]) {
					rebus.digits[rebus.digit] = false;
					rebus.digit = a; return;
				}
			}
			beg = 0;
		}
		/*	 for (char a = beg; a < rebus.digit; a++) {
				 if (!rebus.digits[a]) {
					 rebus.digits[rebus.digit] = false;
					 rebus.digit = a; return;
				 }
			 }*/
		rebus.digits[rebus.digit] = false;
		int buf = rebus.digit;
		if (rebus.digit_return != no_return)
			buf = rebus.digit_return;
		if (buf < 9)
			buf++;
		else
			buf = 0;
		rebus.digit_return = buf;
		rebus.digits[buf] = false;
		for (char c = 'A'; c <= 'Z'; c++)
		{
			if (rebus.simvols[c] == buf)
			{
				rebus.simvols[c] = there_is_simvol;
				*count_selected_simvols--;
				rebus.simvol = c;
				break;
			}
		}
		rebus.digit = buf;

	}
	else {
		*flag_search_simvol = true;
		rebus.simvol = 'A';
		if (rebus.digit >= 0) {
			for (char a = beg; a < 10; a++) {
				if (!rebus.digits[a]) {
					rebus.digit = a; return;
				}
			}
			beg = 0;
		}
		for (char a = beg; a < rebus.digit; a++) {
			if (!rebus.digits[a]) { rebus.digit = a; return; }
		}
	}
}
bool finding_a_solution()
{
	rebus.digit = 0;
	rebus.digit_return = no_return;
	rebus.simvol = 'A';
	bool flag_search = true;
	bool flag_search_simvol = true;
	int count_selected_simvols = 0;
	//bool flag_search_simvol = 1;
	while (flag_search)
	{

		if (flag_search_simvol)
			while ((rebus.simvol <= 'Z') && rebus.simvols[rebus.simvol] != there_is_simvol)
				rebus.simvol++;
		/*for (; digit < 10 && rebus.digits[digit] != false;)
			digit++;*/
		rebus.simvols[rebus.simvol] = rebus.digit;
		count_selected_simvols++;
		rebus.digits[rebus.digit] = true;
		if (count_selected_simvols == rebus.number_of_different_simvols)
		{
			flag_search = check_solution();
			if (flag_search)
			{
				flag_search_simvol = false;
				rebus.simvols[rebus.simvol] = there_is_simvol;
				count_selected_simvols--;
				selection_digit(1, &count_selected_simvols, &flag_search_simvol);
			}
		}
		else
		{
			selection_digit(0, &count_selected_simvols, &flag_search_simvol);
		}
	}
}

int valid_characters(char r)
{
	switch (r)
	{
	case 'A'://vможно попробовать изменить на isupper
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
	case 'G':
	case 'H':
	case 'I':
	case 'J':
	case 'K':
	case 'L':
	case 'M':
	case 'N':
	case 'O':
	case 'P':
	case 'Q':
	case 'R':
	case 'S':
	case 'T':
	case 'U':
	case 'V':
	case 'W':
	case 'X':
	case 'Y':
	case 'Z':
		return 5;
		break;
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
	words[count_word].lenth = *length;
	words[count_word].word = (char*)calloc(sizeof(char), *length + 1);
	memcpy(words[count_word].word, buffer, *length);
	memset(buffer, 0, *length);
	*length = 0;
	count_word++;
}
bool check_input(char* str)
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
			if (count_word <= 7)
			{
				if (rebus.simvols[*str] == no_simvol)
				{
					rebus.simvols[*str] = there_is_simvol;
					rebus.number_of_different_simvols++;
				}


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
int main()
{
	char str[100] = { 0 };
	printf("Enter your rebus:\n");
	fgets(str, 100, stdin);
	//#ifdef DEBAG
	//	printf("\nCode exit: %i\n", check_input(str));
	//	for (int i = 0; i < count_word; i++)
	//		printf("Word %d: %s Length: %d\n", i + 1, words[i].word, words[i].lenth);
	//#endif
	memset(rebus.digits, false, 10);
	memset(rebus.simvols, no_simvol, 256);
	rebus.number_of_different_simvols = 0;

	check_input(str);
	finding_a_solution();
	printf("\n");
	for (int i = 0; i < count_word - 1; i++)
	{
		for (int j = 0; j < words[i].lenth; j++)
		{
			printf("%c", rebus.simvols[words[i].word[j]]);
		}
		printf(" + ");
	}
	printf(" = ");
	for (int j = 0; j < words[count_word - 1].lenth; j++)
	{
		printf("%c", rebus.simvols[words[count_word - 1].word[j]]);
	}
}