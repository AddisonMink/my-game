#include "json.h"

#include "../lib/json.h"

#pragma region PRIVATE

typedef struct Json
{
  struct json_value_s *root;
} Json;

#pragma endregion

Json *JsonLoadFile(const char *filename)
{
  int fileSize;
  unsigned char *fileData = LoadFileData(filename, &fileSize);

  Json *json = MemAlloc(sizeof(Json));
  json->root = json_parse(fileData, strlen((char *)fileData));
  UnloadFileData(fileData);

  return json;
}

void JsonUnload(Json *json)
{
  free(json->root);
  MemFree(json);
}

bool JsonExtractInt(void *result, const Json *json)
{
  if (json->root->type == json_type_number)
  {
    struct json_number_s *number = (struct json_number_s *)json->root->payload;
    *(int *)result = atoi(number->number);
    return true;
  }
  else
  {
    TraceLog(LOG_ERROR, "Value is not a number.");
    return false;
  }
}

bool JsonExtractBool(void *result, const Json *json)
{
  if (json->root->type == json_type_true)
  {
    *(bool *)result = true;
    return true;
  }
  else if (json->root->type == json_type_false)
  {
    *(bool *)result = false;
    return true;
  }
  else
  {
    TraceLog(LOG_ERROR, "Value is not a boolean.");
    return false;
  }
}

bool JsonExtractString(void *result, const Json *json)
{
  if (json->root->type == json_type_string)
  {
    struct json_string_s *string = (struct json_string_s *)json->root->payload;
    TextCopy((char *)result, string->string);
    return true;
  }
  else
  {
    TraceLog(LOG_ERROR, "Value is not a string.");
    return false;
  }
}

bool JsonExtractArray(void *result, int *count, int elementSize, const Json *json, JsonExtractor extractor)
{
  if (json->root->type == json_type_array)
  {
    struct json_array_s *array = (struct json_array_s *)json->root->payload;
    *count = array->length;
    struct json_array_element_s *element = array->start;
    int i = 0;
    while (element)
    {
      Json e = {element->value};
      if (!extractor((char *)result + i * elementSize, &e))
      {
        return false;
      }
      element = element->next;
      i++;
    }
    return true;
  }
  else
  {
    TraceLog(LOG_ERROR, "Value is not an array.");
    return false;
  }
}

bool JsonObjectExtract(void *result, const Json *json, const char *key, JsonExtractor extractor)
{
  if (json->root->type == json_type_object)
  {
    struct json_object_s *object = (struct json_object_s *)json->root->payload;
    struct json_object_element_s *element = object->start;
    while (element)
    {
      if (strcmp(element->name->string, key) == 0)
      {
        Json e = {element->value};
        return extractor(result, &e);
      }
      element = element->next;
    }
    TraceLog(LOG_ERROR, "Key \"%s\" not found.", key);
    return false;
  }
  else
  {
    TraceLog(LOG_ERROR, "Value is not an object.");
    return false;
  }
}

bool JsonObjectExtractArray(void *result, int *count, int elementSize, const Json *json, const char *key, JsonExtractor extractor)
{
  if (json->root->type == json_type_object)
  {
    struct json_object_s *object = (struct json_object_s *)json->root->payload;
    struct json_object_element_s *element = object->start;
    while (element)
    {
      if (strcmp(element->name->string, key) == 0)
      {
        Json e = {element->value};
        return JsonExtractArray(result, count, elementSize, &e, extractor);
      }
      element = element->next;
    }
    TraceLog(LOG_ERROR, "Key \"%s\" not found.", key);
    return false;
  }
  else
  {
    TraceLog(LOG_ERROR, "Value is not an object.");
    return false;
  }
}