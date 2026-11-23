#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Define
#define LOG_TITLE_SIZE 50
size_t initial = 0;

struct Log {
  time_t createdAt;
  char *description;
  uint32_t id;
  int level;
  char logTitle[LOG_TITLE_SIZE];
};

typedef enum {
  LOG_LEVEL_DEBUG = 10,
  LOG_LEVEL_INFO = 20,
  LOG_LEVEL_WARNING = 30,
  LOG_LEVEL_ERROR = 40,
  LOG_LEVEL_CRITICAL = 50
} LogLevel;

typedef enum {
  LOG_ERR_FULL = -5,
  LOG_ERR_NO_MEMORY = -4,
  LOG_ERR_INVALID_TIME = -3,
  LOG_ERR_INVALID_LEVEL = -2,
  LOG_ERR_INVALID_TITLE = -1,
  LOG_OK = 1
} LogStatus;

void log_free(struct Log *log);
void log_init(struct Log *log);
int create_log(struct Log *out, const char *title, const char *desc, int level, time_t date);
char build_datetime();
int classify_log(const char *title, const char *desc);

int create_log(struct Log *out, const char *title, const char *desc, int level, time_t date) {
  uint32_t i = initial;
  if (title == NULL || title[0] == '\0') {
    return LOG_ERR_INVALID_TITLE;
  }
  initial++;
  out->id = initial; 
  char *copyDesc = strdup(desc ? desc : "");
  snprintf(out->logTitle, sizeof out->logTitle, "%s", title);

  //  log->logTitle = strdup(title);
  out->description = copyDesc; 
  out->createdAt = date;
  out->level = classify_log(title, desc);
  if (create_log(...) != LOG_OK) {
    return LOG_ERR_FULL;
  }
  return LOG_OK;
}

int classify_log(const char *title, const char *desc) {
  return 0;
}

void log_init(struct Log *log) {
  log->id = 0;
  log->logTitle = '0';
  log->description = NULL;
  log->createdAt = NULL;
}

void log_free(struct Log *log) {
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
    struct Log *log;
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
        *log = create_log(title, descp, createdAtInput);
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
