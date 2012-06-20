/* 
 * File:   main.c
 * Author: jgalvao
 *
 * Created on June 19, 2012, 5:27 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FTP_SUCCESS = 1;
#define FTP_ERROR = 0;

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream){
  curl_off_t nread;
  
  size_t retcode = fread(ptr, size, nmemb, stream);
 
  nread = (curl_off_t)retcode;
 
  fprintf(stderr, "*** We read %ld bytes from file\n", nread);
  
  return retcode;
}




int main(int argc, char** argv) {

    CURL *curl;
    CURLcode res;
    FILE *localFile;
    struct stat fileStat;
    curl_off_t fileSize;
    
    struct curl_slist *headerList = NULL;
    static const char buf1[] = "RNFR bootstrap.zip";
    static const char buf2[] = "RNTO bootstrap-renamed.zip";
        
    char *localFilePath = "/home/jgalvao/Downloads/bootstrap.zip";
    if(stat(localFilePath, &fileStat)){
        printf("error, file not found: %s", localFilePath, strerror(errno));
        return EXIT_FAILURE;
    }
    
    //Recupera o tamanho o arquivo em bytes
    fileSize = (curl_off_t)fileStat.st_size;
    printf("Local file size: %" "ld" "bytes.\n", fileSize);
            
    localFile = fopen(localFilePath, "rb");
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    curl = curl_easy_init();
    
    if(curl){
        headerList = curl_slist_append(headerList, buf1);
        headerList = curl_slist_append(headerList, buf2);
        
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
        
    }else{
        printf("Error: cURL not initialized");
    }
    
    fclose(localFile);
    curl_global_cleanup();
    
    return (EXIT_SUCCESS);
}

