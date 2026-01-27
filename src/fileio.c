#include <RGSL/fileio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif

size_t rgsl_read_file(const char* filename, char **out_buffer) {
    FILE *file;
    fopen_s(&file, filename, "rb");
    if (file == NULL) {
        return 0;
    }
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    *out_buffer = (char *)malloc((file_size + 1) * sizeof(char));
    if (*out_buffer == NULL) {
        fclose(file);
        return 0;
    }
    size_t read_size = fread(*out_buffer, sizeof(char), file_size, file);
    (*out_buffer)[read_size] = '\0'; // Null-terminate the string
    fclose(file);
    return read_size;
}

bool rgsl_write_file(const char* filename, const char* buffer, size_t size) {
    bool valid = true;
    FILE *file;
    fopen_s(&file, filename, "wb");
    if (file == NULL) {
        return false;
    }
    if (size) {
        size_t written_size = fwrite(buffer, sizeof(char), size, file);
        valid &= (written_size == size);
    } else {
        size_t written_size = fwrite(buffer, sizeof(char), strlen(buffer), file);
        valid &= (written_size == strlen(buffer));
    }
    
    fclose(file);
    return valid;
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