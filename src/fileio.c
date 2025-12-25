#include <RGSL/fileio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *rgsl_read_file(const char* filename) {
    FILE *file;
    fopen_s(&file, filename, "rb");
    if (!file) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = (char *)malloc(length + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

char *rgsl_crlf_to_lf(const char* str) {
    size_t len = strlen(str);
    char *buffer = (char *)malloc(len + 1);
    if (!buffer) {
        return NULL;
    }
    size_t j = 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\r' && str[i + 1] == '\n') {
            buffer[j++] = '\n';
            i++; // Skip the '\n'
        } else {
            buffer[j++] = str[i];
        }
    }
    buffer[j] = '\0';
    buffer = (char *)realloc(buffer, j + 1);
    return buffer;
}

void rgsl_free_file_buffer(char* buffer) {
    free(buffer);
}

bool rgsl_file_exists(const char* filename) {
    FILE *file;
    fopen_s(&file, filename, "rb");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}