# C-Utils

A comprehensive collection of utility functions and macros for everyday C programming tasks.

## Overview

C-Utils provides a centralized library of common utilities that every C programmer needs. Instead of repeatedly implementing the same functions across projects, this library offers reliable, tested, and well-documented utilities that can be easily integrated into any C project.

## Features

### Console Utilities
- Cross-platform screen clearing
- Screen pause functionality

### Memory Utilities
- Safe memory allocation wrappers with error handling
- Memory deallocation with pointer nullification

### String Utilities
- String duplication with error handling
- Prefix and suffix checking
- Whitespace trimming

### Time Utilities
- Timestamp generation
- Execution time measurement

### Logging System
- Multiple log levels (DEBUG, INFO, WARNING, ERROR, FATAL)
- Timestamped log entries
- File or stdout logging

### File Utilities
- File existence checking
- File size determination
- Complete file reading

### Random Number Generation
- Seeded initialization
- Integer and floating-point random number generation

### Debugging Tools
- Variable inspection macros
- Assertion handling

## Installation

### As a Git Submodule (recommended)

```bash
# Add as a submodule to your project
git submodule add https://github.com/pucitos/c-utils.git lib/c-utils

# Update your build configuration
# In your Makefile:
# CFLAGS += -I./lib/c-utils/include
```

### Manual Installation

```bash
# Clone the repository
git clone https://github.com/pucitos/c-utils.git

# Include in your compilation
gcc -I/path/to/c-utils/include your_program.c -o your_program
```

## Usage Examples

### Basic Screen Clearing

```c
#include "utils.h"

int main() {
    clear_screen();
    printf("Hello, clean world!\n");
    return 0;
}
```

### Safe Memory Management

```c
#include "utils.h"

int main() {
    int* array = safe_malloc(10 * sizeof(int));
    
    // Use the array...
    
    // Safely free and nullify
    safe_free((void**)&array);
    
    // array is now NULL
    return 0;
}
```

### Logging

```c
#include "utils.h"

int main() {
    log_init("application.log", LOG_INFO);
    
    log_message(LOG_INFO, "Application started");
    log_message(LOG_WARNING, "Configuration file not found, using defaults");
    
    // Fatal error will exit the program
    if (critical_error)
        log_message(LOG_FATAL, "Critical error: %s", error_message);
    
    log_close();
    return 0;
}
```

### String Operations

```c
#include "utils.h"

void process_filename(const char* filename) {
    if (str_ends_with(filename, ".c")) {
        printf("Processing C source file: %s\n", filename);
    }
}
```

## Contributing

Contributions are welcome! If you have a useful utility function that could benefit others, please submit a pull request. Make sure your code is:

- Well-documented with clear function descriptions
- Error-resistant with appropriate checks
- Consistent with the existing code style
- Platform-independent where possible


## Roadmap

- Add more data structure implementations (dynamic arrays, hash tables)
- Add network utility functions
- Add JSON parsing utilities
- Implement unit tests for all functions
- Create more comprehensive documentation

---

Created and maintained by [pucitos](https://github.com/pucitos)
