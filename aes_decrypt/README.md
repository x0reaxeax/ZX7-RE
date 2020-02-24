## ZX7 AES decryption mimic POC

```
$ ./decrypt "7" "DEC_7"
$ sha256sum DEC_7 zx7.bin
 517ef2b6c7d90323eb223f22c2609e1efb673b4ee73d1f56ff0e43480e16cfe2  OUTFILE  
 517ef2b6c7d90323eb223f22c2609e1efb673b4ee73d1f56ff0e43480e16cfe2  zx7.bin
 MATCH
 ```
Compile with `-lssl -lcrypto`
