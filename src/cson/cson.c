// cson.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cson.h"

JsonValue* create_json_value() {
    return (JsonValue*)calloc(1, sizeof(JsonValue));
}

JsonObject* create_json_object() {
    JsonObject* obj = (JsonObject*)malloc(sizeof(JsonObject));
    obj->keys = NULL;
    obj->values = NULL;
    obj->size = 0;
    obj->capacity = 0;
    return obj;
}

JsonArray* create_json_array() {
    JsonArray* arr = (JsonArray*)malloc(sizeof(JsonArray));
    arr->values = NULL;
    arr->size = 0;
    arr->capacity = 0;
    return arr;
}

void free_json_value(JsonValue* value) {
    if (!value) return;

    switch (value->type) {
        case JSON_STRING:
            free(value->value.string);
            break;
        case JSON_ARRAY:
            for (int i = 0; i < value->value.array->size; i++) {
                free_json_value(value->value.array->values[i]);
            }
            free(value->value.array->values);
            free(value->value.array);
            break;
        case JSON_OBJECT:
            for (int i = 0; i < value->value.object->size; i++) {
                free(value->value.object->keys[i]);
                free_json_value(value->value.object->values[i]);
            }
            free(value->value.object->keys);
            free(value->value.object->values);
            free(value->value.object);
            break;
        default:
            break;
    }

    free(value);
}

void add_to_json_object(JsonObject* obj, const char* key, JsonValue* value) {
    if (obj->size >= obj->capacity) {
        obj->capacity = obj->capacity ? obj->capacity * 2 : 4;
        obj->keys = (char**)realloc(obj->keys, obj->capacity * sizeof(char*));
        obj->values = (JsonValue**)realloc(obj->values, obj->capacity * sizeof(JsonValue*));
    }
    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size] = value;
    obj->size++;
}

void add_to_json_array(JsonArray* arr, JsonValue* value) {
    if (arr->size >= arr->capacity) {
        arr->capacity = arr->capacity ? arr->capacity * 2 : 4;
        arr->values = (JsonValue**)realloc(arr->values, arr->capacity * sizeof(JsonValue*));
    }
    arr->values[arr->size] = value;
    arr->size++;
}

void skip_whitespace(const char** json) {
    while (isspace(**json)) (*json)++;
}

char* parse_string(const char** json) {
    (*json)++; // Skip opening quote
    const char* start = *json;
    while (**json != '"' && **json != '\0') (*json)++;
    int length = *json - start;
    char* result = (char*)malloc(length + 1);
    strncpy(result, start, length);
    result[length] = '\0';
    if (**json == '"') (*json)++; // Skip closing quote
    return result;
}

JsonValue* parse_number(const char** json) {
    JsonValue* value = create_json_value();
    char* end;
    long long int_val = strtoll(*json, &end, 10);

    if (*end == '.' || *end == 'e' || *end == 'E') {
        // Floating point number if decimal point or exponent notation is present
        value->type = JSON_NUMBER;
        value->value.number = strtod(*json, (char**)json);
    } else {
        value->type = JSON_INTEGER;
        value->value.integer = int_val;
        *json = end;
    }

    return value;
}

JsonValue* parse_array(const char** json) {
    JsonValue* value = create_json_value();
    value->type = JSON_ARRAY;
    value->value.array = create_json_array();

    (*json)++; // Skip opening bracket
    skip_whitespace(json);

    if (**json == ']') {
        (*json)++; // Skip closing bracket
        return value;
    }

    while (1) {
        JsonValue* element = parse_json(json);
        add_to_json_array(value->value.array, element);

        skip_whitespace(json);
        if (**json == ',') {
            (*json)++;
            skip_whitespace(json);
        } else if (**json == ']') {
            (*json)++;
            break;
        } else {
            // Error: expected ',' or ']'
            free_json_value(value);
            return NULL;
        }
    }

    return value;
}

JsonValue* parse_object(const char** json) {
    JsonValue* value = create_json_value();
    value->type = JSON_OBJECT;
    value->value.object = create_json_object();

    (*json)++; // Skip opening brace
    skip_whitespace(json);

    if (**json == '}') {
        (*json)++; // Skip closing brace
        return value;
    }

    while (1) {
        if (**json != '"') {
            // Error: expected string as key
            free_json_value(value);
            return NULL;
        }

        char* key = parse_string(json);
        skip_whitespace(json);

        if (**json != ':') {
            // Error: expected ':' after key
            free(key);
            free_json_value(value);
            return NULL;
        }
        (*json)++;
        skip_whitespace(json);

        JsonValue* element = parse_json(json);
        add_to_json_object(value->value.object, key, element);
        free(key);

        skip_whitespace(json);
        if (**json == ',') {
            (*json)++;
            skip_whitespace(json);
        } else if (**json == '}') {
            (*json)++;
            break;
        } else {
            // Error: expected ',' or '}'
            free_json_value(value);
            return NULL;
        }
    }

    return value;
}

JsonValue* parse_json(const char** json) {
    skip_whitespace(json);

    switch (**json) {
        case '{':
            return parse_object(json);
        case '[':
            return parse_array(json);
        case '"':
            {
                JsonValue* value = create_json_value();
                value->type = JSON_STRING;
                value->value.string = parse_string(json);
                return value;
            }
        case 't':
            if (strncmp(*json, "true", 4) == 0) {
                *json += 4;
                JsonValue* value = create_json_value();
                value->type = JSON_BOOLEAN;
                value->value.boolean = 1;
                return value;
            }
            break;
        case 'f':
            if (strncmp(*json, "false", 5) == 0) {
                *json += 5;
                JsonValue* value = create_json_value();
                value->type = JSON_BOOLEAN;
                value->value.boolean = 0;
                return value;
            }
            break;
        case 'n':
            if (strncmp(*json, "null", 4) == 0) {
                *json += 4;
                JsonValue* value = create_json_value();
                value->type = JSON_NULL;
                return value;
            }
            break;
        default:
            if (**json == '-' || isdigit(**json)) {
                return parse_number(json);
            }
    }

    // Error: unexpected character
    return NULL;
}

void print_indent(int indent_count) {
    for (int i = 0; i < indent_count; i++) {
        printf(" ");
    }
}

void print_json_array(JsonArray* array, int indent_count, int current_indent) {
    if (!array) return;

    printf("[");
    if (array->size > 0) {
        printf("\n");
        for (int i = 0; i < array->size; i++) {
            print_indent(current_indent + indent_count);
            print_json_value(array->values[i], indent_count, current_indent + indent_count);
            if (i < array->size - 1) {
                printf(",");
            }
            printf("\n");
        }
        print_indent(current_indent);
    }
    printf("]");
}

void print_json_object(JsonObject* object, int indent_count, int current_indent) {
    if (!object) return;

    printf("{");
    if (object->size > 0) {
        printf("\n");
        for (int i = 0; i < object->size; i++) {
            print_indent(current_indent + indent_count);
            printf("\"%s\": ", object->keys[i]);
            print_json_value(object->values[i], indent_count, current_indent + indent_count);
            if (i < object->size - 1) {
                printf(",");
            }
            printf("\n");
        }
        print_indent(current_indent);
    }
    printf("}");
}

void print_json_value(JsonValue* value, int indent_count, int current_indent) {
    if (!value) return;

    if (indent_count >= 0) {
        switch (value->type) {
            case JSON_NULL:
                printf("null");
                break;
            case JSON_BOOLEAN:
                printf(value->value.boolean ? "true" : "false");
                break;
            case JSON_INTEGER:
                printf("%lld", value->value.integer);
                break;
            case JSON_NUMBER:
                printf("%g", value->value.number);
                break;
            case JSON_STRING:
                printf("\"%s\"", value->value.string);
                break;
            case JSON_ARRAY:
                print_json_array(value->value.array, indent_count, current_indent);
                break;
            case JSON_OBJECT:
                print_json_object(value->value.object, indent_count, current_indent);
                break;
        }
    } else {
        char* json_str = json_value_to_string(value, -1, 0);
        printf("%s", json_str);
        free(json_str);
    }
}

void print_json(JsonValue* value, int indent_count) {
    print_json_value(value, indent_count, 0);
}