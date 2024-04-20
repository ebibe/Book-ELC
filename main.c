#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include <string.h>
#include <ctype.h>

//extern char apiKey[];  // Declare apiKey as extern

char *strndup(const char *s, size_t n) {
    size_t len = strnlen(s, n);
    char *new = malloc(len + 1);

    if(new == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    new[len] = '\0';
    return memcpy(new, s, len);
}

size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    char **response_ptr =  (char**)userp;
    *response_ptr = strndup(buffer, (size_t)(size *nmemb));
    return size * nmemb;
}

void parse_and_print_json(const char *response) {
    cJSON *json_response = cJSON_Parse(response);
    //const cJSON *name = NULL;
    if(json_response == NULL) {
        fprintf(stderr, "Error parsing JSON response\n");
        return;
    }

    //cJSON_GetObjectItemCaseSensitive
    //cJSON_GetArrayItem

    cJSON *docs_first= cJSON_GetObjectItemCaseSensitive(json_response, "docs");
    if(docs_first == NULL)
    {
        fprintf(stderr, "docs  not found\n");
        return;
    }

    cJSON *array_item=cJSON_GetArrayItem(docs_first,0);
    if(array_item == NULL)
    {
        //fprintf(stderr, " array not found\n");
        printf("Italian edition not found or the name inserted is incorrect");
        return;
    }
    cJSON *editions= cJSON_GetObjectItemCaseSensitive(array_item,"editions");
    if(editions == NULL)
    {
        fprintf(stderr, "editions  not found\n");
        return;
    }
    cJSON *docs_second= cJSON_GetObjectItemCaseSensitive(editions, "docs");
    if(docs_second == NULL)
    {
        fprintf(stderr, "docs_second  not found\n");
        return;
    }
    cJSON *array_item_nested=cJSON_GetArrayItem(docs_second,0);
    if(array_item_nested == NULL)
    {
        fprintf(stderr, " nested array not found\n");
        return;
    }
    cJSON *title= cJSON_GetObjectItemCaseSensitive(array_item_nested, "title");
    if (cJSON_IsString(title) && (title->valuestring != NULL)) {
        printf("Titolo in italiano: %s\n", title->valuestring);
    } else {
        printf("Titolo in italiano not found or is not a string\n");
    }



    char *json_string = cJSON_Print(json_response);
    printf("This is the json parsed text:  %s\n", json_string);
    free(json_string);

    cJSON_Delete(json_response);
}

void replace_white_space(char *str )
{
    int i=0;
    while(str[i] != '\0')
    {
        if(isspace(str[i]))
        {
            if(str[i] == '\n') {
                str[i] = '\0';  // Replace newline with null character
            } else {
                str[i] = '+';
            }
        }
        i++;
    }
}


int main() {
    CURL *curl;
    CURLcode res;
    char *response = NULL;
    char book_title[100];
    char author_name[100];
    printf("Enter the name of the book: ");
    fgets(book_title, 100, stdin);
    replace_white_space(book_title);
    printf("Enter the author's name: ");
    fgets(author_name, 100, stdin);
    replace_white_space(author_name);
    //printf("%s",book_title);
    //printf("%s", author_name);

    char url[256];
    snprintf(url, sizeof(url), "https://openlibrary.org/search.json?q=%s+language:ita"
                               "&author=%s&fields=key,title,author_name,editions,editions.key,editions.title,editions.author_name,"
                               "editions.language",book_title,author_name);

    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl == NULL) {
        fprintf(stderr, "curl_easy_init() failed\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\SSL\\cacert_2024_03_11.pem");

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    else
    {
        printf("Response: %s\n", response);
        parse_and_print_json(response);
    }


    curl_easy_cleanup(curl);

    free(response);
    curl_global_cleanup();
    return 0;
}