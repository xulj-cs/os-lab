void memcpy(void *dst,void *src,int size){

	char* pdst = dst;
	char* psrc = src;

	while(size--){
		*pdst++ = *psrc++;
	}
}

void memset(void *dst,int n,int size){

	char *pdst = dst;

	while(size--){
		*pdst++ = (char)n;
	}
}
