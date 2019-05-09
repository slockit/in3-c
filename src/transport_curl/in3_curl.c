#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "in3_curl.h"
#include "../core/client/client.h"
#include "../core/util/stringbuilder.h"
#include <curl/curl.h>

#ifndef CURL_MAX_PARALLEL
#define CURL_MAX_PARALLEL 50
#endif

/*
struct MemoryStruct {
  char *memory = NULL;
  size_t size;
};
 */
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  in3_response_t* r = (in3_response_t*) userp;
  sb_add_range(&r->result, contents, 0, size * nmemb );
  return size * nmemb ;
}

static void readDataNonBlocking(CURLM *cm, const char* url, const char* payload, in3_response_t* r) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(payload));

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)r);

    /* Perform the request, res will get the return code */ 
    res = curl_multi_add_handle(cm, curl);
    if(res != CURLM_OK) {
      sb_add_chars(&r->error, "curl_multi_add_handle() failed:");
      sb_add_chars(&r->error, (char*) curl_easy_strerror(res));
    }
  }
  else 
    sb_add_chars(&r->error, "no curl:");
}

int send_curl_nonblocking(const char** urls,int urls_len, char* payload, in3_response_t* result) {
  CURLM *cm;
  CURLMsg *msg;
  int transfers = 0;
  int msgs_left = -1;
  int still_alive = 1;

  curl_global_init(CURL_GLOBAL_ALL);
  cm = curl_multi_init();

  int max_conn = min(CURL_MAX_PARALLEL, urls_len/10) + 1;
  // printf("%d\n", max_conn);

  curl_multi_setopt(cm, CURLMOPT_MAXCONNECTS, (long) max_conn);

  for(transfers = 0; transfers < min(max_conn, urls_len); transfers++)
    readDataNonBlocking(cm, urls[transfers],payload, result+transfers);

  do {
    curl_multi_perform(cm, &still_alive);

    while((msg = curl_multi_info_read(cm, &msgs_left))) {
      if(msg->msg == CURLMSG_DONE) {
        char *url;
        CURL *e = msg->easy_handle;
        curl_easy_getinfo(msg->easy_handle, CURLINFO_PRIVATE, &url);
        // fprintf(stderr, "R: %d - %s <%s>\n",
        //        msg->data.result, curl_easy_strerror(msg->data.result), url);
        curl_multi_remove_handle(cm, e);
        curl_easy_cleanup(e);
      }
      else {
        // fprintf(stderr, "E: CURLMsg (%d)\n", msg->msg);
        sb_add_chars(&result->error, "E: CURLMsg");
      }
      if(transfers < urls_len) {
        readDataNonBlocking(cm, urls[transfers], payload, result + transfers);
        transfers++;
      }
    }

    if(still_alive)
      curl_multi_wait(cm, NULL, 0, 1000, NULL);

  } while(still_alive || (transfers < urls_len));

  curl_multi_cleanup(cm);
  curl_global_cleanup();
  return 0;
}

static void readDataBlocking(const char* url, char* payload, in3_response_t* r) {
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);

    /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
       itself */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(payload));

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)r);


    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK) {
      sb_add_chars(&r->error, "curl_easy_perform() failed:");
      sb_add_chars(&r->error, (char*) curl_easy_strerror(res));
    }

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  else
    sb_add_chars(&r->error, "no curl:");

}

int send_curl_blocking(const char** urls,int urls_len, char* payload, in3_response_t* result) {
  int i;
  for (i=0;i<urls_len;i++) {
    readDataBlocking(urls[i],payload, result+i );
  }
  return 0;
}

int send_curl(char** urls,int urls_len, char* payload, in3_response_t* result) {
#ifdef CURL_BLOCKING
  return send_curl_blocking((const char**) urls, urls_len, payload, result);
#else
  return send_curl_nonblocking((const char**) urls, urls_len, payload, result);
#endif
}