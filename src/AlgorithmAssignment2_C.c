/*
 ============================================================================
 Name        : AlgorithmAssignment2_C.c
 Author      : Zhen Du
 Version     :
 Copyright   : No Copyright
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_C 53
#define MAX_LINE 20000
#define FILE_PATH "./ProgrammingAssignment2SampleInput2.txt"
#define OUT_FILE_PATH "./ProgrammingAssignment2SampleOutput2.txt"

long getCurrentTime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void removeDuplicate(char str[]) {
	int n = 1;
	int m = 0;
	if (str == NULL) {
		return;
	}
	m = strlen(str);
	if (m == 0 || m == 1) {
		return;
	}

	char strLine[m + 1];
	char f = str[0];
	strLine[0] = f;
	for (int i = 1; i < m; i++) {
		char c = str[i];
		if (c != '\0') {
			if (f == c) {
				continue;
			} else {
				strLine[n++] = c;
				f = c;
			}
		}
	}
	strLine[n] = '\0';
	strcpy(str, strLine);
	return;
}

int removeSingle(char str[]) {
	int count = 0;
	int n = 0;
	int m = 0;
	int word[26] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0 };
	if (str == NULL) {
		return 0;
	}
	m = strlen(str);
	if (m == 0) {
		return 0;
	}
	char strLine[m + 1];
	for (int i = 0; i < m; i++) {
		char c = str[i];
		if (c != '\0') {
			word[c - 'A']++;
		}
	}
	for (int i = 0; i < m; i++) {
		char c = str[i];
		if (c != '\0') {
			if (word[c - 'A'] == 1) {
				count++;
			} else {
				strLine[n++] = c;
			}
		}
	}
	strLine[n] = '\0';
	strcpy(str, strLine);
//	str = strLine;
	return count;
}
int count[50][50];
void initTable(char str[]) {
	int m = 0;
	if (str == NULL) {
		return;
	}
	m = strlen(str);
	if (m == 0) {
		return;
	}
	if (m == 1 && (str[0] == '\r' || str[0] == '\n')) {
		return;
	}
	for (int k = 0; k < m; k++) {
		for (int i = 0; i + k < m; i++) {
			int j = i + k;
			if (i == j) {
				count[i][j] = 1; //A
			} else if (j - i == 1) {
				count[i][j] = 2; //AB
			} else if (j - i == 2) {
				if (str[i] == str[j]) {
					count[i][j] = 2; //ABA
				} else {
					count[i][j] = 3; //ABC
				}
			} else if (j - i == 3) {
				if (str[i] != str[j - 1] && str[i] != str[j]
						&& str[i + 1] != str[j]) {
					count[i][j] = 4; //ABCD
				} else {
					count[i][j] = 3; //ABCA,ABCB,ABAC
				}
			} else if (j - i == 4) {
				if ((str[i] == str[j] && str[i + 1] == str[j - 1])
						|| (str[i] == str[j] && str[i] == str[i + 2])) {
					count[i][j] = 3; //ABCBA,ABABA
				} else if (str[i] != str[i + 2] && str[i] != str[j]
						&& str[i + 1] != str[j - 1] && str[i + 2] != str[j]
						&& str[i + 1] != str[j] && str[i] != str[j - 1]) {
					count[i][j] = 5; //ABCDE
				} else {
					count[i][j] = 4; //other
				}
			} else {
				count[i][j] = INT_MAX;
			}
		}
	}
}
int calculateSweep(char str[], int i, int j) {
	int m = 0;
	if (str == NULL || i > j || i < 0 || j < 0) {
		return 0;
	}
	m = strlen(str);
	if (m == 0 || i > m - 1 || j > m - 1) {
		return 0;
	}
	if (m == 1 && (str[0] == '\r' || str[0] == '\n')) {
		return 0;
	}
	if (count[i][j] != INT_MAX) {
		return count[i][j];
	} else {
		if (str[i] == str[j]) {
			bool exist = false;
			char c = str[i];
			int n = 2;
			for (int k = i + 1; k < j; k++) {
				if (c == str[k]) {
					n++;
					exist = true;
				}
			}
			if (!exist) {
				if (count[i + 1][j - 1] != INT_MAX) {
					count[i][j] = count[i + 1][j - 1] + 1;
				} else {
					count[i][j] = calculateSweep(str, i + 1, j - 1) + 1;
				}
			} else {
				//A divide to individual parts
				int head[n];
				head[0] = i;
				head[n - 1] = j;
				int m = 1;
				for (int k = i + 1; k < j && m < n - 1; k++) {
					if (c == str[k]) {
						head[m] = k;
						m++;
					}
				}
				int min1 = INT_MAX; //1;
				int min = INT_MAX;
				int sum = INT_MAX;
				for (int r = 1; r <= n - 1; r++) {
					for (int p = 1; p + r <= n; p++) {
						sum = calculateSweep(str, head[0] + 1, head[p - 1] - 1)
								+ calculateSweep(str, head[p - 1] + 1,
										head[p + r - 1] - 1)
								+ calculateSweep(str, head[p + r - 1] + 1,
										head[n - 1] - 1);
						if (sum < min) {
							min = sum;
						}
					}
				}
				min1 = min + 1;
				int min2 = calculateSweep(str, i + 1, j - 1) + 1;
				if (min1 < min2) {
					count[i][j] = min1;
				} else {
					count[i][j] = min2;
				}
			}
		} else {
			int min = INT_MAX;
			int sum = 0;
			for (int r = 1; r <= j - i; r++) {
				for (int p = i; p + r <= j + 1; p++) {
					sum = calculateSweep(str, i, p - 1)
							+ calculateSweep(str, p, p + r - 1)
							+ calculateSweep(str, p + r, j);
					if (sum < min) {
						min = sum;
					}
				}
			}
			count[i][j] = min;
		}
	}
	return count[i][j];
}

int main(void) {
	int count = 0;
	FILE *fp;
	FILE *out;
	long start = getCurrentTime();
	char strLine[MAX_C];
	if ((fp = fopen(FILE_PATH, "r")) == NULL) {
		printf("Open Falied!");
		return -1;
	}
	if ((out = fopen(OUT_FILE_PATH, "w")) == NULL) {
		printf("Open Failed.\n");
		return -1;
	}
	while (!feof(fp)) {
		strLine[0] = '\0';
		fgets(strLine, MAX_C, fp);
		strLine[strlen(strLine) - 1] = '\0';
		if (strlen(strLine) == 0)
			continue;
		count = removeSingle(strLine);
		removeDuplicate(strLine);
		while (1) {
			int n = removeSingle(strLine);
			if (n == 0) {
				break;
			} else {
				removeDuplicate(strLine);
				count += n;
			}
		}
		initTable(strLine);
		count += calculateSweep(strLine, 0, strlen(strLine) - 1);

		fprintf(out, "%d \n", count);
	}
	fclose(fp);
	fclose(out);
	printf("Total time:%.4f", (getCurrentTime() - start) / 1000.0);
	return EXIT_SUCCESS;
}
