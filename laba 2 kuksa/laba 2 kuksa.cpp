#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <string.h>
#include <stdio.h>

const int max = 1000;
char stack[max];
int tos = 0;

void push(int i)
{
	if (tos >= max)
		printf("Stack full\n");
	else
		stack[tos++] = i;
}

char pop(void)
{
	tos--;
	if (tos < 0)
		printf("Stack empty\n");
	else
		return stack[tos];
	return 0;
}

int priorCheck(char t)
{
	if (t == '(') return 0;
	else if (t == ')') return 1;
	else if (t == '+' || t == '-' || t == '|' || t == '^' || t == '=') return 2;
	else if (t == '!') return 3;
	else if (t == '*' || t == '/' || t == '&') return 4;
	else return 0;
}

void RPN(FILE* f, char* text)
{
	int pR, pS, count = 0;
	char buf;
	char result[max], finalResult[max];
	for (int i = 0; i <= strlen(text); i++)
	{
		if (text[i] == '\n') {
			while (tos > 0) {
				result[count++] = ' ';
				result[count++] = pop();
			}
			result[count++] = '\n';
		}
		else if (isdigit(text[i]) || isalpha(text[i]) || text[i] == ' ')
			result[count++] = text[i];
		else if (tos == 0)
			push(text[i]);
		else {
			pS = priorCheck(text[tos - 1]);
			pR = priorCheck(text[i]);
			if (text[i] == '(')
				push(text[i]);
			else if (text[i] == ')')
				while ((buf = pop()) != '(') {
					result[count++] = ' ';
					result[count++] = buf;
				}
			else if (pS > pR)
				push(text[i]);
			else {
				while (pR <= priorCheck(stack[tos - 1]) && tos != 0) {
					result[count++] = ' ';
					result[count++] = pop();
				}
				push(text[i]);
			}
		}
	}
	result[count] = '\0';
	char* ptr = strtok(result, " ");
	strcpy(finalResult, ptr);
	while (ptr = strtok(NULL, " ")) {
		strcat(finalResult, " ");
		strcat(finalResult, ptr);
	}
	fprintf(f, "%s", finalResult);
	tos = 0;
}

int main()
{
	FILE* f1 = fopen("input.txt", "r"), * f2 = fopen("output.txt", "w");
	char text[max];
	if (f1 == NULL || f2 == NULL) {
		printf("File error");
		return 0;
	}
	while (fgets(text, max, f1))
		RPN(f2, text);
	fclose(f1);
	fclose(f2);
	return 0;
}