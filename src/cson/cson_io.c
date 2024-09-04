// cson_io.c

#include <stdio.h>
#include <stdlib.h>
#include "cson.h"

static void write_json_value(FILE* file, JsonValue* value, int indent, int current_indent);
static void write_json_array(FILE* file, JsonArray* array, int indent, int current_indent);
static void write_json_object(FILE* file, JsonObject* object, int indent, int current_indent);

static void write_indent(FILE* file, int indent_count) {
    for (int i = 0; i < indent_count; i++) {
        fprintf(file, " ");
    }
}

int write_json(JsonValue* value, const char* path, int indent) {
    if (!value || !path) return -1;

    FILE* file = fopen(path, "w");
    if (!file) {
        perror("Error opening file for writing");
        return -1;
    }

    write_json_value(file, value, indent, 0);

    if (indent >= 0) {
        fprintf(file, "\n");
    }

    fclose(file);
    return 0;
}

static void write_json_value(FILE* file, JsonValue* value, int indent, int current_indent) {
    if (!value) return;

    switch (value->type) {
        case JSON_NULL:
            fprintf(file, "null");
            break;
        case JSON_BOOLEAN:
            fprintf(file, value->value.boolean ? "true" : "false");
            break;
        case JSON_INTEGER:
            fprintf(file, "%lld", value->value.integer);
            break;
        case JSON_NUMBER:
            fprintf(file, "%g", value->value.number);
            break;
        case JSON_STRING:
            fprintf(file, "\"%s\"", value->value.string);
            break;
        case JSON_ARRAY:
            write_json_array(file, value->value.array, indent, current_indent);
            break;
        case JSON_OBJECT:
            write_json_object(file, value->value.object, indent, current_indent);
            break;
    }
}

static void write_json_array(FILE* file, JsonArray* array, int indent, int current_indent) {
    if (!array) return;

    fprintf(file, "[");
    if (array->size > 0) {
        if (indent >= 0) fprintf(file, "\n");
        for (int i = 0; i < array->size; i++) {
            if (indent >= 0) write_indent(file, current_indent + indent);
            write_json_value(file, array->values[i], indent, current_indent + indent);
            if (i < array->size - 1) {
                fprintf(file, ",");
            }
            if (indent >= 0) fprintf(file, "\n");
        }
        if (indent >= 0) write_indent(file, current_indent);
    }
    fprintf(file, "]");
}

static void write_json_object(FILE* file, JsonObject* object, int indent, int current_indent) {
    if (!object) return;

    fprintf(file, "{");
    if (object->size > 0) {
        if (indent >= 0) fprintf(file, "\n");
        for (int i = 0; i < object->size; i++) {
            if (indent >= 0) write_indent(file, current_indent + indent);
            fprintf(file, "\"%s\": ", object->keys[i]);
            write_json_value(file, object->values[i], indent, current_indent + indent);
            if (i < object->size - 1) {
                fprintf(file, ",");
            }
            if (indent >= 0) fprintf(file, "\n");
        }
        if (indent >= 0) write_indent(file, current_indent);
    }
    fprintf(file, "}");
}

// ファイルからJSONを読み込む関数
JsonValue* read_json(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Error opening file for reading");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* json_str = malloc(file_size + 1);
    fread(json_str, 1, file_size, file);
    json_str[file_size] = '\0';

    fclose(file);

    const char* json_ptr = json_str;
    JsonValue* result = parse_json(&json_ptr);

    free(json_str);
    return result;
}
