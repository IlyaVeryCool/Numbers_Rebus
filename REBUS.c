#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<conio.h>


struct words {
	char* word;
	int lenth;
}words[8];

int count_word = 0;


int valid_characters(char r)
{
	switch (r)
	{
	case 'A'://v����� ����������� �������� �� isupper
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
	flags[8] = 0;// c���� �����������
	flags[0] = 1;//��������� ����� �����
	flags[5] = 1;//����� ����� ����
	flags[1] = 0;//���� ������� �����
	words[count_word].lenth = *length;
	words[count_word].word = (char*)calloc(sizeof(char), *length + 1);
	memcpy(words[count_word].word, buffer, *length);
	memset(buffer, 0, *length);
	*length = 0;
	count_word++;
}
char check_input(char* str)
{

	int length = 0;
	char buffer[100] = { 0 };
	unsigned char flags[] = { 0,1,1,1,0,0,0,0,0 };
	//flags[0] = ���� �������� ����� =
	//flags[1] = ������� ���� �����, � �� ������
	//flags[2] = ������� ���� ����, � �� �����
	//flags[3] = ��������� �����
	//flags[4] = ��� ���������
	//flags[5] = ����� ����� �� �����
	//flags[6] = ��� ����
	//flags[7] = ���� =
	//flags[8] = ��������� �����
	while (flags[3])
	{
		switch (valid_characters(*str))
		{
		case 5:
			if (count_word <= 7)
			{


				flags[8] = 1;//��������� �����
				flags[6] = 0;//������� ��� ��� ����
				buffer[length] = *str;
				length++;
			}
			else//���� �� ���������
			{
				flags[3] = 0;
				flags[4] = 0;
			}
			break;
		case 4://+
			if (flags[8] == 1)//��� ������ �����
			{
				//���������� ����� � ���������
				adding_word(flags, buffer, &length);
				flags[8] = 0;
			}
			if (flags[1] == 0 && flags[5] == 1 && flags[7] == 0 && flags[6] == 0)// ���� � ������ ���� ����� � ����� �� ������� ���� ����� � ����� �� ����
			{

				flags[2] = 0;//���� �������� ������ �����, ������� ����
				flags[6] = 1;// ������ ���� ��� ��� ����
				flags[5] = 0;
			}
			else//���� �� ���������
			{
				flags[3] = 0;
				flags[4] = 0;
			}
			break;
		case 3://=
			if (flags[8] == 1)// ���� ���� ���������� �����
			{
				//���������� ����� � ���������
				adding_word(flags, buffer, &length);
				flags[8] = 0;
			}
			if (flags[0] && (flags[2] == 0) && (flags[5] == 1))//��������� ��� ����� ���� �������� � �� ��� = ��������� ����� +
			{

				flags[7] = 1;//��������� �����
			}
			else//���� �� ���������
			{
				flags[3] = 0;
				flags[4] = 1;
			}
			break;
		case 2://' '
			if (flags[8] == 1)
			{
				//���������� ����� � ���������
				adding_word(flags, buffer, &length);
				flags[8] = 0;
			}
			break;
		case 1://'/0'
			if (flags[8] == 1)
			{
				//���������� ����� � ���������
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
	printf("Enter your rebus:\n\n");
	fgets(str, 100, stdin);
	printf("\n%i\n", check_input(str));
	for (int i = 0; i < count_word; i++)
		printf("Word %d: %s Length: %d\n", i + 1, words[i].word, words[i].lenth);
}