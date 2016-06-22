#include <stdio.h>
#include "common.h"
#include "partial/partial.h"

void callback(ZipInfo* info, CDFile* file, size_t progress) {
	int percentDone = progress * 100/file->compressedSize;
	fprintf(stderr, "Getting: %d%%\n", percentDone);
}

int main(int argc, char* argv[]) {
	if(3 > argc) {
		fprintf(stderr, "%s <zipfile> <extract> [<outfile>]\r\n", argv[0]);
		return -1;
	}

	char* extract = argv[2];
        char fname[strlen(argv[1]) + sizeof("file://")];
	char* outfile = argv[2];

	if(argc >= 4) {
		outfile = argv[3];
	}

	if(strstr(argv[1], "://") == NULL) {
		strcpy(fname, "file://");
	}

	strcat(fname, argv[1]);

	ZipInfo* info = PartialZipInit(fname);
	if(!info) {
		fprintf(stderr, "Cannot open %s\n", fname);
		return 0;
	}

	PartialZipSetProgressCallback(info, callback);

	CDFile* file = PartialZipFindFile(info, extract);
	if(!file) {
		fprintf(stderr, "Cannot find %s in %s\n", extract, fname);
		return 0;
	}

	unsigned char* data = PartialZipGetFile(info, file);
	int dataLen = file->size;

	PartialZipRelease(info);

	data = realloc(data, dataLen + 1);
	data[dataLen] = '\0';

	FILE* out;
	out = fopen(outfile, "w");

	if (out == NULL) {
		fprintf(stderr, "Failed to open file");
		exit(-1);
	}

	int done = 0;
	done = fwrite(data, sizeof(char), dataLen, out);

	fclose(out);

	free(data);

	return 0;
}

