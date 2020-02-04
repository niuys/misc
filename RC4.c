#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/rc4.h>

int main(int argc,char* argv[])
{
  if(argc < 4) {
      fprintf(stderr, "Need 3 Params: input output key \n");
      return -1;
  }
  char *inputFileName = argv[1];
  char *outputFileName = argv[2];
  unsigned char *key= argv[3];
  int keyLen = strlen(key);

  FILE *inFile = fopen(inputFileName, "rb");
  if(!inFile) {
      fprintf(stderr, "Fail to open input file: %s\n", inputFileName);
      return -1;
  }
  fseek(inFile, 0, SEEK_END);
  long inDataLen = ftell(inFile);
  unsigned char *indata = malloc(inDataLen);
  fseek(inFile, 0, SEEK_SET);
  fread(indata, inDataLen, 1, inFile);
  fclose(inFile);

  RC4_KEY rc4Key;
  RC4_set_key(&rc4Key, keyLen, key);
  unsigned char *outData =(unsigned char *)malloc(inDataLen);
  RC4(&rc4Key, inDataLen, indata, outData);

  FILE *outFile = fopen(outputFileName, "wb");
  if(!outFile) {
      fprintf(stderr, "Fail to open output file: %s", outputFileName);
      return -1;
  }
  fwrite(outData, inDataLen, 1, outFile);
  fclose(outFile);

  fprintf(stdout, "RC4 Enc from %s to %s use key:%s completed \n", inputFileName, outputFileName, key);
  return 0;
}
