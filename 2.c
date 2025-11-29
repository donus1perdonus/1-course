#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "2lib.h"

struct Message {
	int id;
	char* text;
	int size;
};

int main(int argc, char* argv[]) {
	
	if (argc == 0){
		printf(" Требуются аргументы командной строки!");
	}
	
	FILE* fi;
	FILE* logfi;
	logfi = fopen("/var/log/bz_logs/2.log", "a");
	
	time_t startTime = time(NULL);
    	char logMessage[BUFSIZ];
	
	//Создание файла на запись
	
	fi = fopen("/home/vboxuser/bz/temp/2_tmp.txt", "w+");
	if (fi == NULL){
		printf("Ошибка создания файла!");
		
		// Логирование общей продолжительности работы
    		time_t endTime = time(NULL);
    		double duration = difftime(endTime, startTime);
    		sprintf(logMessage, "Общая продолжительность работы: %.2f секунд\n", duration);
    		logToFile(argv[1][0], logMessage);
    		
		sprintf(logMessage, "Код завершения программы: %d\n", 1);
    		logToFile(argv[1][0], logMessage);
		return 1;
	}

	char cond[BUFSIZ] = "", ch;
	char* pb = cond;
	int count_id = 0;

	//Запись информации с консоли в файл
	if ((argv[1][0]) == 'd'){
		while (1) {
		printf("Введите сообщение: ");
		fgets(cond, sizeof(cond), stdin);
		cond[strcspn(cond, "\n")] = '\0';
		fprintf(logfi, "Введите сообщение: %s\n", cond);
		if (strcmp(cond, "Стоп") == 0)
			break;
		fprintf(fi, "%d %s %ld\n", count_id, cond, ((int)strlen(cond) + 1) * sizeof(char));
		count_id++;
		}
	}
	else{
		while (1) {
		printf("Введите сообщение: ");
		fgets(cond, sizeof(cond), stdin);
		cond[strcspn(cond, "\n")] = '\0';
		if (strcmp(cond, "Стоп") == 0)
			break;
		fprintf(fi, "%d %s %ld\n", count_id, cond, ((int)strlen(cond) + 1) * sizeof(char));
		count_id++;
		}
	}
	
	fclose(fi);
	freestr(cond);
	
	//Открытие файла для чтения
	
	fi = fopen("/home/vboxuser/bz/temp/2_tmp.txt", "r");
	if (fi == NULL){
		printf("Ошибка чтения файла!");
		
		// Логирование общей продолжительности работы
    		time_t endTime = time(NULL);
    		double duration = difftime(endTime, startTime);
    		sprintf(logMessage, "Общая продолжительность работы: %.2f секунд\n", duration);
    		logToFile(argv[1][0], logMessage);
    		
    		sprintf(logMessage, "Код завершения программы: %d\n", 3);
    		logToFile(argv[1][0], logMessage);
		return 3;
	}
	fseek(fi, 0, SEEK_SET);
	
	//Выделение памяти под динамический массив структур
	
	struct Message* arr = (struct Message*)malloc((count_id + 1) * sizeof(struct Message));
	if (arr == NULL){
		printf("Ошибка выделения памяти!");
		
		// Логирование общей продолжительности работы
    		time_t endTime = time(NULL);
    		double duration = difftime(endTime, startTime);
    		sprintf(logMessage, "Общая продолжительность работы: %.2f секунд\n", duration);
    		logToFile(argv[1][0], (logMessage));
    		
		sprintf(logMessage, "Код завершения программы: %d\n", 2);
    		logToFile(argv[1][0], logMessage);
		return 2;
	}
	
	//Заполнение динамического массива структуры
	
	int i = 0;
	for (; i < count_id; ++i) {
		while ((ch = fgetc(fi)) != ' ');
		arr[i].id = i;
		
		//------------------------
		
		while ((ch = fgetc(fi)) != ' ') {
			*pb++ = ch;
		}
		*pb = '\0';
		
		arr[i].text = (char*)malloc((strlen(cond) + 1) * sizeof(char));
		if (arr[i].text == NULL){
			printf("Ошибка выделения памяти!");
			
			// Логирование общей продолжительности работы
    			time_t endTime = time(NULL);
    			double duration = difftime(endTime, startTime);
    			sprintf(logMessage, "Общая продолжительность работы: %.2f секунд\n", duration);
    			logToFile(argv[1][0], logMessage);
    		
			sprintf(logMessage, "Код завершения программы: %d\n", 0);
    			logToFile(argv[1][0], logMessage);
			return 2;
		}
		
		copystr(cond, arr[i].text);
		
		freestr(cond);
		pb = cond;
		
		//------------------------
		
		while (((ch = fgetc(fi)) != '\n') && (ch != EOF)) {
			*pb++ = ch;
		}
		*pb = '\0';
		
		arr[i].size = atoi(cond);
		
		freestr(cond);
		pb = cond;
		
		//------------------------
		
		printf("%d %s %d\n", arr[i].id, arr[i].text, arr[i].size);
	}
	
	//Освобождение памяти
	
	int k = 0;
	for (; k < count_id; ++k) {
		free(arr[i].text);
	}
	
	fclose(fi);
	free(arr);

	// Логирование общей продолжительности работы
    	time_t endTime = time(NULL);
    	double duration = difftime(endTime, startTime);
    	sprintf(logMessage, "Общая продолжительность работы: %.2f секунд\n", duration);
    	logToFile(argv[1][0], logMessage);
	
	sprintf(logMessage, "Код завершения программы: %d\n", 0);
    	logToFile(argv[1][0], logMessage);

	return 0;
}
