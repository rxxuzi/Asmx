// cson.h
#ifndef CSON_H
#define CSON_H

#ifdef __cplusplus
extern "C" {
#endif
    typedef struct JsonValue JsonValue;
    typedef struct JsonObject JsonObject;
    typedef struct JsonArray JsonArray;

    typedef enum {
        JSON_NULL,
        JSON_BOOLEAN,
        JSON_INTEGER,
        JSON_NUMBER,
        JSON_STRING,
        JSON_ARRAY,
        JSON_OBJECT
    } JsonType;

    struct JsonObject {
        char** keys;
        JsonValue** values;
        int size;
        int capacity;
    };

    struct JsonArray {
        JsonValue** values;
        int size;
        int capacity;
    };

    struct JsonValue {
        JsonType type;
        union {
            int boolean;
            long long integer;
            double number;
            char* string;
            JsonArray* array;
            JsonObject* object;
        } value;
    };

    JsonValue* parse_json(const char** json);
    char* json_value_to_string(JsonValue* value, int indent, int current_indent);
    void free_json_value(JsonValue* value);
    int write_json(JsonValue* value, const char* path, int indent);
    JsonValue* read_json(const char* path);
    void print_json_value(JsonValue* value, int indent_count, int current_indent);
    void print_json_array(JsonArray* array, int indent_count, int current_indent);
    void print_json_object(JsonObject* object, int indent_count, int current_indent);
    void print_json(JsonValue* value, int indent_count);
#ifdef __cplusplus
}
#endif

#endif //CSON_H
