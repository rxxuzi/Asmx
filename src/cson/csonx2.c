// csonx2.c

#include "cson.h"
#include "csonx.h"
#include <stdlib.h>
#include <string.h>

const char* json_type_to_string(JsonType type) {
    switch (type) {
        case JSON_NULL: return "null";
        case JSON_BOOLEAN: return "boolean";
        case JSON_INTEGER: return "integer";
        case JSON_NUMBER: return "number";
        case JSON_STRING: return "string";
        case JSON_ARRAY: return "array";
        case JSON_OBJECT: return "object";
        default: return "unknown";
    }
}

JsonValue* json_create_null() {
    JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
    value->type = JSON_NULL;
    return value;
}

JsonValue* json_create_boolean(int boolean) {
    JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
    value->type = JSON_BOOLEAN;
    value->value.boolean = boolean;
    return value;
}

JsonValue* json_create_integer(long long integer) {
    JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
    value->type = JSON_INTEGER;
    value->value.integer = integer;
    return value;
}

JsonValue* json_create_number(double number) {
    JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
    value->type = JSON_NUMBER;
    value->value.number = number;
    return value;
}

JsonValue* json_create_string(const char* string) {
    JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
    value->type = JSON_STRING;
    value->value.string = strdup(string);
    return value;
}

JsonValue* json_create_array() {
    JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
    value->type = JSON_ARRAY;
    value->value.array = (JsonArray*)malloc(sizeof(JsonArray));
    value->value.array->values = NULL;
    value->value.array->size = 0;
    value->value.array->capacity = 0;
    return value;
}

JsonValue* json_create_object() {
    JsonValue* value = (JsonValue*)malloc(sizeof(JsonValue));
    value->type = JSON_OBJECT;
    value->value.object = (JsonObject*)malloc(sizeof(JsonObject));
    value->value.object->keys = NULL;
    value->value.object->values = NULL;
    value->value.object->size = 0;
    value->value.object->capacity = 0;
    return value;
}

void json_array_append(JsonArray* arr, JsonValue* value) {
    if (arr->size >= arr->capacity) {
        arr->capacity = arr->capacity ? arr->capacity * 2 : 4;
        arr->values = (JsonValue**)realloc(arr->values, arr->capacity * sizeof(JsonValue*));
    }
    arr->values[arr->size++] = value;
}

void json_object_set(JsonObject* obj, const char* key, JsonValue* value) {
    if (obj->size >= obj->capacity) {
        obj->capacity = obj->capacity ? obj->capacity * 2 : 4;
        obj->keys = (char**)realloc(obj->keys, obj->capacity * sizeof(char*));
        obj->values = (JsonValue**)realloc(obj->values, obj->capacity * sizeof(JsonValue*));
    }
    obj->keys[obj->size] = strdup(key);
    obj->values[obj->size] = value;
    obj->size++;
}

JsonValue* json_value_deep_copy(JsonValue* value) {
    if (!value) return NULL;

    JsonValue* copy = (JsonValue*)malloc(sizeof(JsonValue));
    copy->type = value->type;

    switch (value->type) {
        case JSON_NULL:
            break;
        case JSON_BOOLEAN:
            copy->value.boolean = value->value.boolean;
            break;
        case JSON_INTEGER:
            copy->value.integer = value->value.integer;
            break;
        case JSON_NUMBER:
            copy->value.number = value->value.number;
            break;
        case JSON_STRING:
            copy->value.string = strdup(value->value.string);
            break;
        case JSON_ARRAY:
            copy->value.array = (JsonArray*)malloc(sizeof(JsonArray));
            copy->value.array->size = value->value.array->size;
            copy->value.array->capacity = value->value.array->capacity;
            copy->value.array->values = (JsonValue**)malloc(copy->value.array->capacity * sizeof(JsonValue*));
            for (int i = 0; i < copy->value.array->size; i++) {
                copy->value.array->values[i] = json_value_deep_copy(value->value.array->values[i]);
            }
            break;
        case JSON_OBJECT:
            copy->value.object = (JsonObject*)malloc(sizeof(JsonObject));
            copy->value.object->size = value->value.object->size;
            copy->value.object->capacity = value->value.object->capacity;
            copy->value.object->keys = (char**)malloc(copy->value.object->capacity * sizeof(char*));
            copy->value.object->values = (JsonValue**)malloc(copy->value.object->capacity * sizeof(JsonValue*));
            for (int i = 0; i < copy->value.object->size; i++) {
                copy->value.object->keys[i] = strdup(value->value.object->keys[i]);
                copy->value.object->values[i] = json_value_deep_copy(value->value.object->values[i]);
            }
            break;
    }

    return copy;
}