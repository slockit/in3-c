/** @file 
 * simple commandline-util parsing json and creating bin
 * */

#include <math.h>
#include <rlp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <util/data.h>
#include <util/utils.h>

char* read_from_stdin(FILE* file) {
  if (file == NULL) {
    printf("File not found!");
    _Exit(1);
    return NULL;
  }

  size_t   allocated = 1024;
  size_t   len       = 0;
  uint8_t* buffer    = _malloc(1025);
  size_t   r;

  while (1) {
    r = fread(buffer + len, 1, allocated - len, file);
    len += r;
    if (feof(file)) break;
    buffer = _realloc(buffer, allocated * 2 + 1, allocated);
    allocated *= 2;
  }

  buffer[len] = 0;
  if (file != stdin) fclose(file);
  return (char*) buffer;
}

static char* BLOCK_HEADER[] = {
    "BLOCKHEADER",
    "parent hash",
    "sha3 uncles",
    "miner",
    "transaction root",
    "receipt root",
    "logs bloom",
    "difficulty",
    "number",
    "gas limit",
    "gas used",
    "timestamp",
    "extra data",
    "mixhash/proposerSeed",
    "nonce/signature",
    "extra sealed field"};

static char* TX[] = {
    "TRANSACTION",
    "nonce",
    "gas price",
    "gas",
    "to",
    "value",
    "data",
    "v",
    "r",
    "s"};

static char* TX_RECEIPT[] = {
    "TRANSACTION RECEIPT",
    "status",
    "cumulative gas",
    "logs bloom",
    "logs"};

static char* LOG[] = {
    "EVENT",
    "address",
    "topics",
    "data"};

static char* TRIE_LEAF[] = {
    "MERKLE LEAF",
    "path",
    "value"};

static char* TRIE_BRANCH[] = {
    "MERKLE LEAF",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "value"};

void write(bytes_t* data, char* l, char** tt) {
  bytes_t t;
  //  names
  char prefix[100];
  int  i, j, type, p = strlen(l), d;
  for (i = 0;; i++) {
    type = rlp_decode(data, i, &t);
    if (type == 0) return;
    if (type == 1) {
      printf("%s", l);
      if (tt) {
        d = printf("%-20s : ", tt[i + 1]);
      }

      if (tt == TRIE_LEAF && i == 0)
        d = printf("%s (%s)", (t.data[0] & 32) ? "LEAF" : "EXTENSION", (t.data[0] & 16) ? "odd" : "even");

      else if (t.len == 0)
        d = printf("0");
      else if (t.len < 9)
        d = printf("%llu", bytes_to_long(t.data, t.len));
      else if (t.len == 20)
        d = printf("<address>");
      else if (t.len == 32)
        d = printf("<hash>");
      else
        d = printf("<data %i>", t.len);
      for (j = d; j < 17; j++) printf(" ");
      if (t.len > 0)
        printf("0x");
      else
        printf("<EMPTY>");

      for (j = 0; j < t.len; j++)
        printf("%02x", t.data[j]);

      printf("\n");

    } else if (type == 2) {
      int    l2 = rlp_decode_len(&t);
      char** t2;
      switch (l2) {
        case 15:
        case 16:
          t2 = BLOCK_HEADER;
          break;
        case 17:
          t2 = TRIE_BRANCH;
          break;
        case 2:
          t2 = TRIE_LEAF;
          break;
        case 9:
          t2 = TX;
          break;
        case 3:
          t2 = LOG;
          break;
        case 4:
          t2 = TX_RECEIPT;
          break;
      }
      if (tt) t2 = NULL;
      printf("%s", l);
      if (tt) {
        d = printf("%-20s : ", tt[i + 1]);
      }

      printf("[ %s\n", t2 ? t2[0] : "");
      sprintf(prefix, "%s   ", l);
      write(&t, prefix, t2);
      printf("%s]\n", l);
    }
  }
}

int main(int argc, char* argv[]) {
  char* default_format = "hex";
  char* input          = NULL;
  char* format         = default_format;

  int i;

  // fill from args
  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-f") == 0)
      input = read_from_stdin(fopen(argv[++i], "r"));
    else if (strcmp(argv[i], "-o") == 0)
      format = argv[++i];
    else
      input = argv[i];
  }

  if (input == NULL) input = read_from_stdin(stdin);

  if (input[0] == '0' && input[1] == 'x') input += 2;

  write(hex2byte_new_bytes(input, strlen(input)), "", NULL);

  //  printf("INPUT: %s", input);
  return 0;
}