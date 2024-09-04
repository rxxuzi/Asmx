// csonx.h
#ifndef CSONX_H
#define CSONX_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cson.h"

// JsonObjectからキーを指定して値を取得する
JsonValue* json_object_get(JsonObject* obj, const char* key);

// JsonObjectから整数値を取得する
long long json_object_get_integer(JsonObject* obj, const char* key, long long default_value);

// JsonObjectから浮動小数点数を取得する
double json_object_get_number(JsonObject* obj, const char* key, double default_value);

// JsonObjectから文字列を取得する
const char* json_object_get_string(JsonObject* obj, const char* key, const char* default_value);

// JsonObjectからブール値を取得する
int json_object_get_boolean(JsonObject* obj, const char* key, int default_value);

// JsonObjectから配列を取得する
JsonArray* json_object_get_array(JsonObject* obj, const char* key);

// JsonObjectからオブジェクトを取得する
JsonObject* json_object_get_object(JsonObject* obj, const char* key);

// JsonValueからJsonObjectを取得する（型チェック付き）
JsonObject* json_value_as_object(JsonValue* value);

// JsonValueからJsonArrayを取得する（型チェック付き）
JsonArray* json_value_as_array(JsonValue* value);

// JsonArrayの要素数を取得する
int json_array_size(JsonArray* arr);

// JsonArrayから指定インデックスの要素を取得する
JsonValue* json_array_get(JsonArray* arr, int index);

// JsonObjectの中にキーが存在するかチェック
int json_object_has_key(JsonObject* obj, const char* key);

// JsonObjectから指定したパスの値を取得
JsonValue* json_object_get_path(JsonObject* obj, const char* path);

// JsonArrayから条件に合う要素を検索
JsonValue* json_array_find(JsonArray* arr, int (*predicate)(JsonValue*));

// JsonObjectの全てのキーを取得
char** json_object_keys(JsonObject* obj, int* count);

// JsonValueの型を文字列で取得
const char* json_type_to_string(JsonType type);

JsonValue* json_create_null();
JsonValue* json_create_boolean(int boolean);
JsonValue* json_create_integer(long long integer);
JsonValue* json_create_number(double number);
JsonValue* json_create_string(const char* string);
JsonValue* json_create_array();
JsonValue* json_create_object();

// JsonArrayに要素を追加
void json_array_append(JsonArray* arr, JsonValue* value);

// JsonObjectにキーと値のペアを追加
void json_object_set(JsonObject* obj, const char* key, JsonValue* value);

// JsonValueを深くコピー
JsonValue* json_value_deep_copy(JsonValue* value);

#ifdef __cplusplus
}
#endif
#endif // CSONX_H