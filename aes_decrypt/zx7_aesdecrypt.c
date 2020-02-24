#include <openssl/aes.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define _ENCRYPT 1
#define _DECRYPT 0

const char *CLR_CYAN     = "\033[1;38;5;51m";
const char *CLR_ORANGE   = "\033[38;5;202m";
const char *H0TLIN3      = "\033[38;5;198m";
const char *H0TLIN3_BOLD = "\033[1;38;5;198m";
const char *RR           = "\033[0m";

int _aesdecrypt(FILE *ENC_FILE, FILE *DEC_FILE, const char *aeskey, const char *aesiv) {

    int chunk_size = 512;
    unsigned char encbuf[chunk_size];
    unsigned char decbuf[chunk_size + EVP_MAX_BLOCK_LENGTH];
    
    int enclen;
    int declen;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    EVP_CIPHER_CTX_init(ctx);
    EVP_CipherInit_ex(ctx, EVP_bf_cbc(), NULL, NULL, NULL, _DECRYPT);
    EVP_CIPHER_CTX_set_key_length(ctx, 16);
    EVP_CipherInit_ex(ctx, NULL, NULL, aeskey, aesiv, 0);
    
    while (1) {
        enclen = fread(encbuf, 1, chunk_size, ENC_FILE);
        if(enclen <= 0)
           break;

        if(!EVP_CipherUpdate(ctx, decbuf, &declen, encbuf, enclen)) {   /* err */
            EVP_CIPHER_CTX_cleanup(ctx);
            return EXIT_FAILURE;
        }
        fwrite(decbuf, 1, declen, DEC_FILE);
    }
    if(!EVP_CipherFinal_ex(ctx, decbuf, &declen)) { /* err */
        EVP_CIPHER_CTX_cleanup(ctx);
        return -EXIT_FAILURE;
    }

    fwrite(decbuf, 1, declen, DEC_FILE);
    
    EVP_CIPHER_CTX_cleanup(ctx);
    
    rewind(ENC_FILE);
    rewind(DEC_FILE);

    return EXIT_SUCCESS;

}


int main(int argc, char const *argv[]) {

    const char *aeskey = "ZX7ZX7ZX7ZX7ZX7ZX7ZX7";
    const char *aesiv  = "XALFLIXALFLIXALFLI";

    if (argc < 3) {
        printf("%s >> %sUsage:%s %s%s %s\"ENCRYPTED_FILE\" \"OUTPUT_FILE\"\n", H0TLIN3_BOLD, CLR_CYAN, RR, H0TLIN3, argv[0], CLR_ORANGE, argv[1], argv[2]);
        return -EXIT_FAILURE;
    }

    FILE *encfile = fopen(argv[1], "rb");
    FILE *decfile = fopen(argv[2], "wb");

    if (!encfile) {
        fprintf(stderr, "[ERR]: Unable to open '%s' for reading.\n", argv[1]);
        return -EXIT_FAILURE;
    }

    if (!decfile) {
        fprintf(stderr, "[ERR] Unable to open '%s' for writing.\n", argv[2]);
    }

    int dret;
    if ((dret = _aesdecrypt(encfile, decfile, aeskey, aesiv)) != EXIT_SUCCESS)
        printf("[ERR]: Unable to decrypt file '%s' :: return code '%d'\n", argv[1], dret);
    else 
        printf("[OK]: File '%s' decrypted successfully! :: Decrypted file - '%s'\n", argv[1], argv[2]);

    fclose(encfile);
    fclose(decfile);

    return 0;
}
