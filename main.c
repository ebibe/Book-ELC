#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

int main() {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "John Doe");
    char *string = cJSON_Print(root);
    printf("%s\n", string);
    cJSON_Delete(root);
    free(string);
    return 0;
}