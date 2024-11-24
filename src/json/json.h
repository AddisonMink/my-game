#pragma once

#include "raylib.h"

typedef struct Json Json;

typedef bool (*JsonExtractor)(void *result, const Json *json);

Json *JsonLoadFile(const char *filename);

void JsonUnload(Json *json);

// int *result
bool JsonExtractInt(void *result, const Json *json);

// bool *result
bool JsonExtractBool(void *result, const Json *json);

// char *result
bool JsonExtractString(void *result, const Json *json);

// TODO Function that allocates a string and extracts into it.
// char **result
// bool JsonExtractStringAlloc(void *result, const Json *json);

bool JsonExtractArray(void *result, int *count, int elementSize, const Json *json, JsonExtractor extractor);

bool JsonObjectExtract(void *result, const Json *json, const char *key, JsonExtractor extractor);

bool JsonObjectExtractArray(void *result, int *count, int elementSize, const Json *json, const char *key, JsonExtractor extractor);
