#include <stdio.h>
#include "common.h"
#include "partial/partial.h"

char endianness = IS_LITTLE_ENDIAN;

void callback(ZipInfo* info, CDFile* file, size_t progress) {
	int percentDone = progress * 100/file->compressedSize;
	printf("Getting: %d%%\n", percentDone);
}

int main(int argc, char* argv[]) {
	ZipInfo* info = PartialZipInit("file://test.zip");
	if(!info)
	{
		printf("Cannot find /tmp/test.zip\n");
		return 0;
	}

	PartialZipSetProgressCallback(info, callback);

	CDFile* file = PartialZipFindFile(info, "test.txt");
	if(!file)
	{
		printf("Cannot find test.txt in /tmp/test.zip\n");
		return 0;
	}

	unsigned char* data = PartialZipGetFile(info, file);
	int dataLen = file->size; 

	PartialZipRelease(info);

	data = realloc(data, dataLen + 1);
	data[dataLen] = '\0';
	
	printf("%s\n", data);

	free(data);

	return 0;
}

