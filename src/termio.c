#include <RGSL/termio.h>
#include <RGSL/rgsl.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void rgsl_fprint(FILE *stream, const char* prefix, const char* message) {
    fprintf(stream, "[RGSL %s] %s", prefix, message);
}

void rgsl_format_parser(const char* format, va_list args, char** out_buffer) {
    size_t size = 1024;
    *out_buffer = (char *)malloc(size * sizeof(char));
    va_list args_copy;
    va_copy(args_copy, args);
    for (int n = 0; 
        (size_t)n < size;
        size = (size_t)n + 1, *out_buffer = (char *)realloc(*out_buffer, size * sizeof(char))
    ) {
        n = vsnprintf(*out_buffer, size, format, args_copy);
        if (n < 0) {
            free(*out_buffer);
            *out_buffer = NULL;
            va_end(args_copy);
            return;
        }
        if ((size_t)n < size) {
            break;
        }
    }
    va_end(args_copy);
}

void rgsl_fprintf(FILE *stream, const char* prefix, const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* buffer = NULL;
    rgsl_format_parser(format, args, &buffer);
    if (buffer != NULL) {
        rgsl_fprint(stream, prefix, buffer);
        free(buffer);
    }
    va_end(args);
}

void rgsl_print_info(int verbose_level, const char* message) {
    if (rgsl_global_options.verbose >= verbose_level)
        rgsl_fprint(stdout, "Info", message);
}

void rgsl_printf_info(int verbose_level, const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* buffer = NULL;
    rgsl_format_parser(format, args, &buffer);
    if (buffer != NULL) {
        rgsl_print_info(verbose_level, buffer);
        free(buffer);
    }
    va_end(args);
}

void rgsl_print_error(const char* message) {
    rgsl_fprint(stderr, "Error", message);
}

void rgsl_printf_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* buffer = NULL;
    rgsl_format_parser(format, args, &buffer);
    if (buffer != NULL) {
        rgsl_print_error(buffer);
        free(buffer);
    }
    va_end(args);
}