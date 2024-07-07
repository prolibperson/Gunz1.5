#include "HMAC_SHA1.h"

#ifdef _DEBUG
#pragma comment(lib,"cmld.lib")
#else
#pragma comment(lib,"cml.lib")
#endif

void main(int argc, char* argv[])
{
	if( argv[1] == NULL)
	{
		return;
	}

	BYTE digest[20];
	BYTE Key[GUNZ_HMAC_KEY_LENGTH];

	memset(Key, 0, 20);
	memcpy(Key, GUNZ_HMAC_KEY, strlen(GUNZ_HMAC_KEY));

	CHMAC_SHA1 HMAC_SHA1 ;
	HMAC_SHA1.HMAC_SHA1_file(argv[1], Key, GUNZ_HMAC_KEY_LENGTH, digest) ;

	if( argv[2] == NULL)
	{
		for(int i=0; i< 20; i++)
		{
			printf("%x ", digest[i]);
		}
		char dummy[4];
		scanf(dummy);
	}
	else
	{
		FILE *f;
		f = fopen(argv[2], "w");
		for(int i=0; i <20 ; i++)
		{
			fprintf(f, "%x ", digest[i]);
		}
		fclose(f);
	}
}