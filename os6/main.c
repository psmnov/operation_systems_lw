#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
int menu() {
	puts("1 - ������ ������ ����� �������");
	puts("2 - ������ ������ ����� ����");
	puts("�������� ������ ��� ����� ����");
	int n;
	do {
		scanf_s("%d", &n);
	} while (n < 1 || n>2);
	return n;
}
typedef struct {
	int id;
	int time;
	int num_of_prev;
	int* PreviousProc;
	int early_start;
	int late_end;
}p;
void InitProcesses(FILE * stream, p* mass, int n) {
	if (stream == stdin) printf("������� �������� �� ������� ��������� �������: 1 - ����� ��������, 2 - ���������� �������������� ���������(���� ���, �� 0), 3 - id ��������������, ���� ����\n");
	for (int i = 0; i < n; i++) {
		(mass + i)->id = i + 1;
		fscanf(stream, "%d", &(mass + i)->time);
		fscanf(stream, "%d", &(mass + i)->num_of_prev);
		(mass + i)->early_start = 0;
		(mass + i)->late_end = -1;
		(mass + i)->PreviousProc = (int*)malloc(sizeof(int) * ((mass + i)->num_of_prev));
		for (int j = 0; j < (mass + i)->num_of_prev; j++) fscanf(stream, "%d", &(mass + i)->PreviousProc[j]);
	}
}
void CalculateEarlyStart(p* mass, int id) {
	mass[id].early_start = 0;
	for (int i = 0; i < mass[id].num_of_prev; i++) {
		if (mass[mass[id].PreviousProc[i] - 1].early_start + mass[mass[id].PreviousProc[i] - 1].time >= mass[id].early_start) {
			mass[id].early_start = mass[mass[id].PreviousProc[i] - 1].early_start + mass[mass[id].PreviousProc[i] - 1].time;
		} 
	}
}
void CalculateLateEnd(p* mass, int id){
	for (int i = 0; i < (mass + id)->num_of_prev; i++) {//���� ������� ��������� �������� �� ������� ������� ��� ���������� ������, ��� ������� ��������� �������� �� ��������
		//�� �������, ���� ���� ������� ��������� ������ �� �������������� ����� -1
		if (((mass + id)->late_end - (mass + id)->time < (mass + ((mass + id)->PreviousProc[i]) - 1)->late_end) || (mass + ((mass + id)->PreviousProc[i]) - 1)->late_end == -1) {
			(mass + ((mass + id)->PreviousProc[i]) - 1)->late_end = (mass + id)->late_end - (mass + id)->time;
		}
	}
}
void CalculateAllTimes(p* mass, int n) {
	for (int i = 0; i < n; i++) {
		CalculateEarlyStart(mass, i);
	}
	for (int i = n-1; i >= 0; i--) {
		if (i == n - 1) mass[i].late_end = mass[i].early_start;
		CalculateLateEnd(mass, i);
	}
}
void PrintTable(p* mass, int n) {
	printf("\n+-------------------------------------------------------------+\n");
	printf("|j |Tj |K(j)       |t ��|t ��|t ��|t ��|������|���������� ����|\n");
	printf("+-------------------------------------------------------------+\n");

	for (int i = 0; i < n-1; i++) {
		printf("|%-2d|%-3d|", mass[i].id, mass[i].time);
		for (int j = 0; j < mass[i].num_of_prev; j++) printf("%d ", mass[i].PreviousProc[j]);
		int CountNumOfTooMuchSpaces = 0;
		for (int j = 0; j < mass[i].num_of_prev; j++) if (mass[i].PreviousProc[j] / 10 != 0) CountNumOfTooMuchSpaces++;
		for (int j = 0; j < 11 - mass[i].num_of_prev - mass[i].num_of_prev - CountNumOfTooMuchSpaces; j++) printf(" ");
		int t_rk = mass[i].early_start + mass[i].time;
		int t_pn = mass[i].late_end - mass[i].time;
		int t_pk = mass[i].late_end;
		int reserve = t_pk - t_rk;
		printf("|%-4d|%-4d|%-4d|%-4d|%-6d|%-15s|", mass[i].early_start, t_rk, t_pn, t_pk, reserve, (reserve == 0) ? "*" : " ");
		printf("\n");
		printf("+-------------------------------------------------------------+\n");
	}
	printf("�����: ����� ������������ ���� = %d\n", mass[n - 1].late_end);
	printf("����: ");
	int previousProcess = 0;
	for (int i = 0; i < n; i++) {
		int t_rk = mass[i].early_start + mass[i].time;
		int t_pk = mass[i].late_end;
		int reserve = t_pk - t_rk;
		if (!reserve && i == 0) {
			previousProcess = i;
			printf("%d ", mass[i].id);
		}
		if (!reserve && mass[i].early_start == (mass[previousProcess].early_start + mass[previousProcess].time)) {
			previousProcess = i;
			printf("-> %d ", mass[i].id);
		}
	}
	puts("");
}
int main() {
	setlocale(LC_ALL, "Russian");
	FILE* fp;
	int c, n;
	p* mass = NULL;
	for (;;)
	{
		int c = menu();
		switch (c)
		{
		case 1:
			puts("������� ���������� ��������� ");
			scanf_s("%d", &n);
			mass = (p*)malloc(sizeof(p) * n);
			InitProcesses(stdin, mass, n);
			break;
		case 2:
			fp = fopen("data.txt", "r"); 
			fscanf(fp, "%d", &n);
			mass = (p*)malloc(sizeof(p) * n);
			InitProcesses(fp, mass, n);
			break;
		}
		CalculateAllTimes(mass, n);
		PrintTable(mass, n);
	}
	return 0;
}