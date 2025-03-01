/**
 * @file utils.h
 * @brief Common utility functions for C programs
 * @author pucitos
 * @version 0.1.0
 * @date 2025-03-01
 *
 * A collection of utility functions and macros for everyday C programming.
 */

#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Platform-specific clear screen command
 *
 * Uses "cls" for Windows and "clear" for Unix-based systems
 */
#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

/* ========== CONSOLE UTILITIES ========== */

/**
 * @brief Clears the console screen in a cross-platform way
 */
static inline void clear_screen(void) { system(CLEAR); }

/**
 * @brief Pauses execution until user presses Enter
 */
static inline void pause_screen(void) {
  printf("Press Enter to continue...");
  while (getchar() != '\n')
    ;
}

/* ========== MEMORY UTILITIES ========== */

/**
 * @brief Safe wrapper for malloc with error checking
 *
 * @param size The number of bytes to allocate
 * @return void* Pointer to allocated memory or NULL on failure
 */
static inline void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL && size > 0) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/**
 * @brief Safe wrapper for calloc with error checking
 *
 * @param nmemb Number of elements
 * @param size Size of each element
 * @return void* Pointer to allocated memory or NULL on failure
 */
static inline void *safe_calloc(size_t nmemb, size_t size) {
  void *ptr = calloc(nmemb, size);
  if (ptr == NULL && nmemb > 0 && size > 0) {
    fprintf(stderr, "Error: Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

/**
 * @brief Safe wrapper for realloc with error checking
 *
 * @param ptr Pointer to memory block to resize
 * @param size New size in bytes
 * @return void* Pointer to allocated memory or NULL on failure
 */
static inline void *safe_realloc(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (new_ptr == NULL && size > 0) {
    fprintf(stderr, "Error: Memory reallocation failed\n");
    free(ptr); // Free original memory to prevent leaks
    exit(EXIT_FAILURE);
  }
  return new_ptr;
}

/**
 * @brief Safe wrapper for free that nullifies the pointer after freeing
 *
 * @param ptr Pointer to a pointer to the memory to free
 */
static inline void safe_free(void **ptr) {
  if (ptr != NULL && *ptr != NULL) {
    free(*ptr);
    *ptr = NULL;
  }
}

/* ========== STRING UTILITIES ========== */

/**
 * @brief Safe string duplication with error checking
 *
 * @param str String to duplicate
 * @return char* Newly allocated copy of the string
 */
static inline char *str_duplicate(const char *str) {
  if (str == NULL)
    return NULL;

  size_t len = strlen(str) + 1;
  char *new_str = (char *)safe_malloc(len);
  return strcpy(new_str, str);
}

/**
 * @brief Check if a string starts with a prefix
 *
 * @param str String to check
 * @param prefix Prefix to look for
 * @return true if str starts with prefix, false otherwise
 */
static inline bool str_starts_with(const char *str, const char *prefix) {
  if (str == NULL || prefix == NULL)
    return false;

  size_t str_len = strlen(str);
  size_t prefix_len = strlen(prefix);

  if (prefix_len > str_len)
    return false;

  return strncmp(str, prefix, prefix_len) == 0;
}

/**
 * @brief Check if a string ends with a suffix
 *
 * @param str String to check
 * @param suffix Suffix to look for
 * @return true if str ends with suffix, false otherwise
 */
static inline bool str_ends_with(const char *str, const char *suffix) {
  if (str == NULL || suffix == NULL)
    return false;

  size_t str_len = strlen(str);
  size_t suffix_len = strlen(suffix);

  if (suffix_len > str_len)
    return false;

  return strcmp(str + str_len - suffix_len, suffix) == 0;
}

/**
 * @brief Trim whitespace from the beginning and end of a string
 *
 * @param str String to trim (modified in-place)
 * @return char* Pointer to the trimmed string (same as input)
 */
static inline char *str_trim(char *str) {
  if (str == NULL)
    return NULL;

  // Trim leading space
  char *start = str;
  while (*start && (*start == ' ' || *start == '\t' || *start == '\n' ||
                    *start == '\r')) {
    start++;
  }

  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }

  // Trim trailing space
  char *end = str + strlen(str) - 1;
  while (end > str &&
         (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) {
    *end = '\0';
    end--;
  }

  return str;
}

/* ========== TIME UTILITIES ========== */

/**
 * @brief Get current timestamp as string in format YYYY-MM-DD HH:MM:SS
 *
 * @param buffer Buffer to store the timestamp string (must be at least 20
 * chars)
 * @param size Size of the buffer
 * @return char* Pointer to the buffer containing the timestamp string
 */
static inline char *get_timestamp(char *buffer, size_t size) {
  if (buffer == NULL || size < 20)
    return NULL;

  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
  return buffer;
}

/**
 * @brief Measure execution time of a function in milliseconds
 *
 * @param start Pointer to store start time
 * @param end Pointer to store end time
 * @return double Time elapsed in milliseconds
 */
static inline double time_elapsed_ms(struct timespec *start,
                                     struct timespec *end) {
  return ((end->tv_sec - start->tv_sec) * 1000.0) +
         ((end->tv_nsec - start->tv_nsec) / 1000000.0);
}

/* ========== LOGGING UTILITIES ========== */

/**
 * @brief Log levels for the logging utility
 */
typedef enum {
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARNING,
  LOG_ERROR,
  LOG_FATAL
} LogLevel;

static FILE *log_file = NULL;
static LogLevel current_log_level = LOG_INFO;

/**
 * @brief Initialize the logging system
 *
 * @param filename Name of the log file (NULL for stdout)
 * @param level Minimum log level to record
 * @return true if initialized successfully, false otherwise
 */
static inline bool log_init(const char *filename, LogLevel level) {
  current_log_level = level;

  if (filename == NULL) {
    log_file = stdout;
    return true;
  }

  log_file = fopen(filename, "a");
  if (log_file == NULL) {
    fprintf(stderr, "Error: Could not open log file %s\n", filename);
    return false;
  }

  return true;
}

/**
 * @brief Close the logging system
 */
static inline void log_close(void) {
  if (log_file != NULL && log_file != stdout) {
    fclose(log_file);
    log_file = NULL;
  }
}

/**
 * @brief Log a message with the specified level
 *
 * @param level Log level
 * @param format Format string (printf-style)
 * @param ... Additional arguments for the format string
 */
static inline void log_message(LogLevel level, const char *format, ...) {
  if (log_file == NULL)
    log_file = stdout;
  if (level < current_log_level)
    return;

  const char *level_str[] = {"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"};
  char timestamp[20];
  get_timestamp(timestamp, sizeof(timestamp));

  fprintf(log_file, "[%s] [%s] ", timestamp, level_str[level]);

  va_list args;
  va_start(args, format);
  vfprintf(log_file, format, args);
  va_end(args);

  fprintf(log_file, "\n");
  fflush(log_file);

  if (level == LOG_FATAL) {
    exit(EXIT_FAILURE);
  }
}

/* ========== FILE UTILITIES ========== */

/**
 * @brief Check if a file exists
 *
 * @param filename Path to the file
 * @return true if file exists, false otherwise
 */
static inline bool file_exists(const char *filename) {
  if (filename == NULL)
    return false;

  FILE *file = fopen(filename, "r");
  if (file == NULL)
    return false;

  fclose(file);
  return true;
}

/**
 * @brief Get file size in bytes
 *
 * @param filename Path to the file
 * @return long Size of the file in bytes, or -1 on error
 */
static inline long file_size(const char *filename) {
  if (filename == NULL)
    return -1;

  FILE *file = fopen(filename, "rb");
  if (file == NULL)
    return -1;

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fclose(file);

  return size;
}

/**
 * @brief Read entire file content into a string
 *
 * @param filename Path to the file
 * @return char* Newly allocated string with file content, or NULL on error
 */
static inline char *file_read_all(const char *filename) {
  if (filename == NULL)
    return NULL;

  FILE *file = fopen(filename, "rb");
  if (file == NULL)
    return NULL;

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *content = (char *)safe_malloc(size + 1);
  size_t read_size = fread(content, 1, size, file);
  fclose(file);

  if (read_size != (size_t)size) {
    free(content);
    return NULL;
  }

  content[size] = '\0';
  return content;
}

/* ========== RANDOM UTILITIES ========== */

/**
 * @brief Initialize random number generator with current time
 */
static inline void random_init(void) { srand((unsigned int)time(NULL)); }

/**
 * @brief Generate a random integer in the specified range [min, max]
 *
 * @param min Minimum value (inclusive)
 * @param max Maximum value (inclusive)
 * @return int Random integer in the range [min, max]
 */
static inline int random_int(int min, int max) {
  if (min > max) {
    int temp = min;
    min = max;
    max = temp;
  }
  return min + rand() % (max - min + 1);
}

/**
 * @brief Generate a random double in the specified range [min, max)
 *
 * @param min Minimum value (inclusive)
 * @param max Maximum value (exclusive)
 * @return double Random double in the range [min, max)
 */
static inline double random_double(double min, double max) {
  if (min > max) {
    double temp = min;
    min = max;
    max = temp;
  }
  return min + ((double)rand() / RAND_MAX) * (max - min);
}

/* ========== DEBUGGING MACROS ========== */

/**
 * @brief Debug macro to print variable name and value
 */
#define DEBUG_PRINT(var) printf("%s = %d\n", #var, var)

/**
 * @brief Debug macro to print string variable
 */
#define DEBUG_PRINT_STR(var) printf("%s = %s\n", #var, var)

/**
 * @brief Debug macro to print float/double variable
 */
#define DEBUG_PRINT_FLOAT(var) printf("%s = %f\n", #var, var)

/**
 * @brief Simple assertion macro that prints error message and line number
 */
#define ASSERT(condition)                                                      \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", #condition,  \
              __FILE__, __LINE__);                                             \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

#endif /* UTILS_H */
