//  https://github.com/curl/curl/blob/master/docs/examples/simple.c
#include <curl/curl.h> //curl
#include <3ds.h> //libctru

//stdlib stuff
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000
static u32 *SOC_buffer = NULL;
//function for writing response to string ig 
// https://github.com/curl/curl/blob/master/docs/libcurl/opts/CURLOPT_WRITEFUNCTION.3#L83-L103
struct memory {
  char *response;
  size_t size;
};

static size_t cb(void *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)clientp;
 
  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if(ptr == NULL)
    return 0;  /* out of memory! */
 
  mem->response = ptr;
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
 
  return realsize;
}
//end of write function
int main() {
    gfxInitDefault();
    consoleInit(GFX_TOP, GFX_LEFT);
    Result ret;
    //
    SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
	if(SOC_buffer == NULL) {
		printf("memalign: failed to allocate\n");
	}

	// Now intialise soc:u service
	if ((ret = socInit(SOC_buffer, SOC_BUFFERSIZE)) != 0) {
    	printf("socInit: 0x%08X\n", (unsigned int)ret);
	}

    //from now on socket is initialized and we happy use curl

    char response[20000];
    char* url;
    long response_code;
    char* body; 



    /***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * SPDX-License-Identifier: curl
 *
 ***************************************************************************/
/* <DESC>
 * Very simple HTTP GET
 * </DESC>
 */
/*#include <stdio.h>
#include <curl/curl.h>

int main(void)
{*/
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);
 
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  sprintf(response, "%ld %s %s\n",  response_code, url, body);

  /*return 0;
}*/


    /*
    char aresponse[strlen(response)];
    strncpy(aresponse, response, 200);
    aresponse[200] = '\0';
    printf(aresponse);*/
    printf(response);
    while (aptMainLoop()) {
        hidScanInput();
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START) break;
    }


    socExit();
    gfxExit();
}