#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include <stdlib.h>

#define HWID_LEN 9


void getHWID(char _hwid[]) {
	DWORD dhwid;

	if (GetVolumeInformationA("C://", NULL, NULL, &dhwid, NULL, NULL, NULL, NULL)) {
		snprintf(_hwid, HWID_LEN+1, "%lu", dhwid);
	}
}

void shifter(char sstr[], int shift) {
	unsigned int itr;
	
	size_t _slen = strlen(sstr);
	
	for (itr = 0; itr < _slen; ++itr) {
		sstr[itr] = sstr[itr] - shift;
	}
}

int main(int argc, const char** argv) {
	if (argc < 2) {
		return -EXIT_FAILURE;
	}
	
	if (strlen(argv[1]) != 9) {
		fprintf(stderr, "[x] '%s' is not a valid ZX7 result code\n", argv[1]);
		return -EXIT_FAILURE;
	}

	char HWID[10] = "\0";


	for (int i = 0; i < 0xFF; ++i) {
		getHWID(HWID);

		if (HWID[0] == '\0') {
			return -EXIT_FAILURE;
		}

		shifter(HWID, i);
		if (!strncmp(HWID, argv[1], HWID_LEN)) {
			printf("[%d]-[MATCH]: > '%s' == '%s' <\n", i, HWID, argv[1]);
			break;
		}
		
		printf("[%d]-[shifter]: '%s' != '%s'\n", i, HWID, argv[1]);
	}

	return EXIT_SUCCESS;

}
