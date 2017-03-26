#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int get_next_line(const char* data, char* dst, int dst_size);

int http_get_requested_file(const char* http, char* dst, int dst_size){
	char lineBuffer[1024];
	int i = 0;
	int n = 0; 
    int j;
	memset(dst, 0, dst_size);
	while(n != 2){
		n = get_next_line(http + i, lineBuffer, sizeof(lineBuffer));
		i += n;
		if(n == -1)
			continue;
		
		if(strstr(lineBuffer, "GET ") == lineBuffer){
			printf("%s\n", lineBuffer);
			char* req = lineBuffer + 4;
			int max = sizeof(lineBuffer) - 4;
			max = max < dst_size - 1 ? max : dst_size - 1;
			for(j = 0; j < max; j++){
				if(req[j] == ' ')
					return j;
				dst[j] = req[j];
			}
			return -1;
		}
	}
	return 0;
}

int http_get_response_header(int code, int len, char* dst, int dst_size){
	if(code == 200){
		snprintf(dst, dst_size, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n", len);
		return strlen(dst);
	}
	else{
		snprintf(dst, dst_size, "HTTP/1.1 %d ERROR\r\n\r\n", code);
		return strlen(dst);
	}

}

int get_next_line(const char* data, char* dst, int dst_size){
	int i;
    memset(dst, 0, dst_size);
	char lastChar = 0;
	for(i = 0; i < dst_size - 1; i++){
		if(lastChar == '\r' && data[i] == '\n'){
			return i + 1;
		}
		lastChar = data[i];
		if(lastChar != '\r' && lastChar != '\n')
			dst[i] = data[i];
	}
	return -1;
}
