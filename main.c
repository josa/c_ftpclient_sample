#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

static const int FTP_SUCCESS = 1;
static const int FTP_ERROR = 0;

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream){
  curl_off_t nread;
  
  size_t retcode = fread(ptr, size, nmemb, stream);
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %ld bytes from file\n", nread);
  
  return retcode;
}

int ftpPut(char *filePath){
    
    CURL *curl;
    CURLcode res;
    FILE *localFile;
    struct stat fileStat;
    curl_off_t fileSize;
    
    char *putcommand = "RNFR ";
    char *fileName = malloc(1);
    parseFileName(filePath, fileName);
    
    char *cmdPrefix = "RNFR";
    const char putCmd[sizeof(cmdPrefix) + sizeof(fileName)];
    sprintf(putCmd, "%s %s", cmdPrefix, fileName);   
    
    struct curl_slist *headerList = NULL;
    
    if(stat(filePath, &fileStat)){
        printf("error, file not found: %s", filePath, strerror(errno));
        return EXIT_FAILURE;
    }
    
    fileSize = (curl_off_t)fileStat.st_size;
    printf("Local file size: %" "ld" "bytes.\n", fileSize);
    
    localFile = fopen(filePath, "rb");
    
    //TODO: move it
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if(curl){
        headerList = curl_slist_append(headerList, putCmd);
        
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, "ftp://127.0.0.1/bootstrap.zip");
        curl_easy_setopt(curl, CURLOPT_PORT, 8888);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "user1");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "user1");
        curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerList);
        curl_easy_setopt(curl, CURLOPT_READDATA, localFile);
        curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, fileSize);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 15);
        
        res = curl_easy_perform(curl);
        
        if(res == CURLE_OK){
            printf("file uploaded successfully");
        }else if(res == CURLE_UPLOAD_FAILED){
            printf("Error: upload failed");
        }else if(res == CURLE_LOGIN_DENIED){
             printf("Error: access denied");
        }else if(res == CURLE_RECV_ERROR){
             printf("Error: ocorreu um erro");
        }else if(res == CURLE_OPERATION_TIMEDOUT);
        
        curl_slist_free_all(headerList);
        curl_easy_cleanup(curl);
    }
    
    //Free pointers
    free(fileName);
    
    return FTP_SUCCESS;
}

void parseFileName(char *filePath, char *filename){
    int i, len = strlen(filePath);
    char path[len];
    char *tmpToken;
    
    for(i=0; i<=len; i++) 
        path[i] = *(filePath+i);
    
    tmpToken = strtok(path, "/");
    while(NULL != tmpToken){
        len = strlen(tmpToken);
        if(NULL == realloc(filename, len+1)){
            printf("%s", "out of memory");
            exit(0);
        }
        
        for(i=0; i<=len; i++)
            *(filename+i) = *(tmpToken+i);
        
        tmpToken = NULL;
        tmpToken = strtok(NULL, "/");
    }
    free(tmpToken);
    tmpToken = NULL;
}

int main(int argc, char** argv) {

    return EXIT_SUCCESS;

}