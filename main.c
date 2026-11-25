#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

// Define
#define LOG_TITLE_SIZE 50
#define LOG_CAPACITY 100
size_t initial = 0;
size_t count = 0;

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

enum LogStatus {
  LOG_ERR_NULL_OUT = -5,
  LOG_ERR_NO_MEMORY = -4,
  LOG_ERR_INVALID_TIME = -3,
  LOG_ERR_INVALID_LEVEL = -2,
  LOG_ERR_INVALID_TITLE = -1,
  LOG_OK = 1
};

void log_free(struct Log *log);
void log_init(struct Log *log);
int create_log(struct Log *out, const char *title, const char *desc, int level, time_t date);
int build_datetime(time_t t, char *buf, size_t size);
int classify_log(const char *title, const char *desc);
static int is_valid_level(int level);
const char *log_status_str(enum LogStatus st);

int create_log(struct Log *out, const char *title, const char *desc, int level, time_t date) {
  if (out == NULL) return LOG_ERR_NULL_OUT;
  if (title == NULL || title[0] == '\0') return LOG_ERR_INVALID_TITLE;
  if (!is_valid_level(level)) return LOG_ERR_INVALID_LEVEL;
  if (date == (time_t) -1) return LOG_ERR_INVALID_TIME;

  char *copyDesc = strdup(desc ? desc : "");
  if (copyDesc == NULL) return LOG_ERR_NO_MEMORY;

  snprintf(out->logTitle, sizeof out->logTitle, "%s", title);
  out->description = copyDesc; 
  out->createdAt = date;
  out->level = classify_log(title, desc);
  out->id = initial++;
  return LOG_OK;
}

int classify_log(const char *title, const char *desc) {
  return 0;
}

const char *log_status_str(enum LogStatus st) {
  switch(st) {
    case LOG_OK: return "ok";
    case LOG_ERR_INVALID_TIME: return "data inválida";
    case LOG_ERR_INVALID_TITLE: return "título inválido";
    case LOG_ERR_INVALID_LEVEL: return "nível inválido";
    case LOG_ERR_NO_MEMORY: return "sem memória";
    case LOG_ERR_NULL_OUT: return "dado nulo";
  }
  return "erro desconhecido";
}

static int is_valid_level(int level) {
  switch(level) {
    case LOG_LEVEL_DEBUG:
    case LOG_LEVEL_INFO:
    case LOG_LEVEL_WARNING:
    case LOG_LEVEL_ERROR:
    case LOG_LEVEL_CRITICAL:
      return 1;
    default:
      return 0;
  }
}

void log_init(struct Log *log) {
  log->id = 0;
  snprintf(log->logTitle, sizeof log->logTitle, "%s", "0");
//  log->logTitle = strdup('0');
  log->description = strdup("");
  log->createdAt = time(NULL);
}

void log_free(struct Log *log) {
  if (log == NULL) return;
  free(log->description);
  memset(log, 0, sizeof *log);
}

int build_datetime(time_t t, char *buf, size_t size) {
  if (buf == NULL || size == 0) {
    return LOG_ERR_NULL_OUT;
  }
  buf[0] = '\0';
  struct tm tm_info;
  if (localtime_r(&t, &tm_info) == NULL) return LOG_ERR_INVALID_TIME;
  if (strftime(buf, size, "%d-%m-%Y %H:%M:%S", &tm_info) == 0) {
    buf[0] = '\0';
    return LOG_ERR_NULL_OUT;
  }
  return LOG_OK;
}

int main(void) {
  int choice;
  char title[256];
  char descp[1024];
  char createdAtInput;
  bool running = true;
  while(running) {
    printf("Log program");
    putchar(10);
    printf("Type your choice to procced: ");
    scanf("%d", &choice);
    struct Log logs[LOG_CAPACITY];
    switch (choice){
      case 1:
        printf("Title: ");
        scanf("%s", title);
        putchar(10);
        printf("Description: ");
        scanf("%s", descp);
        putchar(10);
        printf("Catching current datetime..");
        time_t now = time(NULL);
        if (now == (time_t) -1) {
          fprintf(stderr, "falha ao obter a data");
          putchar(10);
          break;
        }
        int level = classify_log(title, descp);
        int st = create_log(&logs[count], title, descp, level, now);

        if (st != LOG_OK) {
          fprintf(stderr, "falha: %s\n", log_status_str(st));
        } else {
          count++;
        }
        break;
      case 2:
        break;
      case 3:
        printf("Exiting the program...");
        running = false;
        break;
      default: 
        break;
    }
  }
  return EXIT_SUCCESS;
}
