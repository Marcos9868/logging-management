#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

size_t initial = 0;

typedef struct {
  uint8_t id;
  char logTitle[50];
  char *description;
  char *createdAt;
} Log;

void log_free(Log *log);
void log_init(Log *log);
Log create_log(char title, char *desc, char *date);
char build_datetime();

Log create_log(char title, char *desc, char *date) {
  Log log;
  uint8_t i = initial;
  if (strlen(&title) == 0 && desc == NULL) {
    perror("Título e descrição estão vazios ou inválidos");
    log_free(&log);
    return log;
  }
  initial++;
  log.id = initial; 
  log.logTitle[0] = title;
  log.description = desc;
  log.createdAt = date;
  return log;
}

void log_init(Log *log) {
  log->id = 0;
  log->logTitle[0] = '0';
  log->description = NULL;
  log->createdAt = NULL;
}

void log_free(Log *log) {
  free(log->description);
  free(log->createdAt);
  log->description = NULL;
  log->createdAt = NULL;
}

char build_datetime() {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  char buffer[100];

  char date = strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", t);
  return date;
}

int main(void) {
  int choice;
  char *title;
  char *descp;
  char createdAtInput;
  bool running = true;
  while(running) {
    printf("Log program");
    putchar(10);
    printf("Type your choice to procced: ");
    scanf("%d", &choice);
    switch (choice){
      case 1:
        printf("Title: ");
        scanf("%s", title);
        putchar(10);
        printf("Description: ");
        scanf("%s", descp);
        putchar(10);
        printf("Catching current datetime..");
        createdAtInput = build_datetime();
        Log log = create_log(*title, descp, &createdAtInput);
        break;
      case 2:
        break;
      case 3:
        printf("Exiting the program...");
        running = false;
      default: 
        break;
    }
  }
  return EXIT_SUCCESS;
}
