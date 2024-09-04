// csonp.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cson.h"
#define CSON_BUFFER_SIZE 1024
#define MAX_RECURSION_DEPTH 100

char* json_value_to_string_recursive(JsonValue* value, int indent, int current_indent, int depth) {
    if (depth > MAX_RECURSION_DEPTH) {
        fprintf(stderr, "Error: Maximum recursion depth exceeded\n");
        return strdup("null");
    }

    if (!value) return strdup("null");

    char* result = NULL;
    char temp[CSON_BUFFER_SIZE];

    switch (value->type) {
        case JSON_NULL:
            return strdup("null");
        case JSON_BOOLEAN:
            return strdup(value->value.boolean ? "true" : "false");
        case JSON_INTEGER:
            snprintf(temp, sizeof(temp), "%lld", value->value.integer);
            return strdup(temp);
        case JSON_NUMBER:
            snprintf(temp, sizeof(temp), "%g", value->value.number);
            return strdup(temp);
        case JSON_STRING:
            snprintf(temp, sizeof(temp), "\"%s\"", value->value.string);
            return strdup(temp);
        case JSON_ARRAY:
        {
            int array_size = value->value.array->size;
            char** elements = calloc(array_size, sizeof(char*));
            if (!elements) {
                fprintf(stderr, "Memory allocation failed for elements\n");
                return strdup("null");
            }

            size_t total_length = 2;  // [] and null terminator
            for (int i = 0; i < array_size; i++) {
                elements[i] = json_value_to_string_recursive(value->value.array->values[i], indent, current_indent + (indent >= 0 ? indent : 0), depth + 1);
                if (!elements[i]) {
                    for (int j = 0; j < i; j++) free(elements[j]);
                    free(elements);
                    return strdup("null");
                }
                total_length += strlen(elements[i]) + 2;  // element + ", "
                if (indent >= 0) total_length += indent + 1;  // indentation + newline
            }

            result = malloc(total_length);
            if (!result) {
                for (int i = 0; i < array_size; i++) free(elements[i]);
                free(elements);
                return strdup("null");
            }

            char* p = result;
            *p++ = '[';
            for (int i = 0; i < array_size; i++) {
                if (indent >= 0) {
                    *p++ = '\n';
                    for (int j = 0; j < current_indent + indent; j++) *p++ = ' ';
                }
                strcpy(p, elements[i]);
                p += strlen(elements[i]);
                if (i < array_size - 1) *p++ = ',';
                free(elements[i]);
            }
            if (indent >= 0 && array_size > 0) {
                *p++ = '\n';
                for (int j = 0; j < current_indent; j++) *p++ = ' ';
            }
            *p++ = ']';
            *p = '\0';

            free(elements);
            return result;
        }
        case JSON_OBJECT:
        {
            int object_size = value->value.object->size;
            char** keys = calloc(object_size, sizeof(char*));
            char** values = calloc(object_size, sizeof(char*));
            if (!keys || !values) {
                free(keys);
                free(values);
                return strdup("null");
            }

            size_t total_length = 2;  // {} and null terminator
            for (int i = 0; i < object_size; i++) {
                keys[i] = strdup(value->value.object->keys[i]);
                values[i] = json_value_to_string_recursive(value->value.object->values[i], indent, current_indent + (indent >= 0 ? indent : 0), depth + 1);
                if (!keys[i] || !values[i]) {
                    for (int j = 0; j < i; j++) {
                        free(keys[j]);
                        free(values[j]);
                    }
                    free(keys);
                    free(values);
                    return strdup("null");
                }
                total_length += strlen(keys[i]) + strlen(values[i]) + 4;  // "key": value,
                if (indent >= 0) total_length += indent + 1;  // indentation + newline
            }

            result = malloc(total_length);
            if (!result) {
                for (int i = 0; i < object_size; i++) {
                    free(keys[i]);
                    free(values[i]);
                }
                free(keys);
                free(values);
                return strdup("null");
            }

            char* p = result;
            *p++ = '{';
            for (int i = 0; i < object_size; i++) {
                if (indent >= 0) {
                    *p++ = '\n';
                    for (int j = 0; j < current_indent + indent; j++) *p++ = ' ';
                }
                p += sprintf(p, "\"%s\": ", keys[i]);
                strcpy(p, values[i]);
                p += strlen(values[i]);
                if (i < object_size - 1) *p++ = ',';
                free(keys[i]);
                free(values[i]);
            }
            if (indent >= 0 && object_size > 0) {
                *p++ = '\n';
                for (int j = 0; j < current_indent; j++) *p++ = ' ';
            }
            *p++ = '}';
            *p = '\0';

            free(keys);
            free(values);
            return result;
        }
    }

    return strdup("null");
}

char* json_value_to_string(JsonValue* value, int indent, int current_indent) {
    return json_value_to_string_recursive(value, indent, current_indent, 0);
}