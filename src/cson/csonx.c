// csonx.c
#include "csonx.h"
#include <string.h>

JsonValue* json_object_get(JsonObject* obj, const char* key) {
    if (!obj || !key) return NULL;
    for (int i = 0; i < obj->size; i++) {
        if (strcmp(obj->keys[i], key) == 0) {
            return obj->values[i];
        }
    }
    return NULL;
}

long long json_object_get_integer(JsonObject* obj, const char* key, long long default_value) {
    JsonValue* value = json_object_get(obj, key);
    if (value && value->type == JSON_INTEGER) {
        return value->value.integer;
    }
    return default_value;
}

double json_object_get_number(JsonObject* obj, const char* key, double default_value) {
    JsonValue* value = json_object_get(obj, key);
    if (value) {
        if (value->type == JSON_NUMBER) {
            return value->value.number;
        } else if (value->type == JSON_INTEGER) {
            return (double)value->value.integer;
        }
    }
    return default_value;
}

const char* json_object_get_string(JsonObject* obj, const char* key, const char* default_value) {
    JsonValue* value = json_object_get(obj, key);
    if (value && value->type == JSON_STRING) {
        return value->value.string;
    }
    return default_value;
}

int json_object_get_boolean(JsonObject* obj, const char* key, int default_value) {
    JsonValue* value = json_object_get(obj, key);
    if (value && value->type == JSON_BOOLEAN) {
        return value->value.boolean;
    }
    return default_value;
}

JsonArray* json_object_get_array(JsonObject* obj, const char* key) {
    JsonValue* value = json_object_get(obj, key);
    if (value && value->type == JSON_ARRAY) {
        return value->value.array;
    }
    return NULL;
}

JsonObject* json_object_get_object(JsonObject* obj, const char* key) {
    JsonValue* value = json_object_get(obj, key);
    if (value && value->type == JSON_OBJECT) {
        return value->value.object;
    }
    return NULL;
}

JsonObject* json_value_as_object(JsonValue* value) {
    if (value && value->type == JSON_OBJECT) {
        return value->value.object;
    }
    return NULL;
}

JsonArray* json_value_as_array(JsonValue* value) {
    if (value && value->type == JSON_ARRAY) {
        return value->value.array;
    }
    return NULL;
}

int json_array_size(JsonArray* arr) {
    return arr ? arr->size : 0;
}

JsonValue* json_array_get(JsonArray* arr, int index) {
    if (arr && index >= 0 && index < arr->size) {
        return arr->values[index];
    }
    return NULL;
}