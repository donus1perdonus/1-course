#include <stdio.h>
#include <string.h>
#include "2lib.h"

//Функция для очистки массива символов

char* freestr(char* a) {
	int i = 0;
	for (; a[i] != '\0'; ++i)
		a[i] = '\0';
	return a;
}

//Функция для посимвольного копирования строки

char* copystr(char* a, char* b){
	int c = 0;
	for (; c < (int)strlen(a); ++c) b[c] = a[c];
	b[c] = '\0';
	return b;
}

// Функция для пеедачи информации в логи

void logToFile(char id, const char* logMessage) {
    FILE* logFile;
    
    if (id == '0') {
        // Если id равен 0, завершаем работу функции
        return;
    }

    logFile = fopen("/var/log/bz_logs/2.log", "a");

    if (logFile != NULL) {
        fprintf(logFile, "%s", logMessage);
        fclose(logFile);
    }
}

