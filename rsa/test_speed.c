#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tomcrypt.h>
#include "cpucycles.h"
#include "speed_print.h"
#include "libtomcryptrsa.h"

#define NTESTS 1000


int main(void)
{
  uint64_t t[NTESTS];

  // DEFINING PK, SK, CIPHERTEXT LENGTHS....
  int k = (log(KEYSIZE) / log(2)) - 8;
  int crypto_secretkeybytes = (k * 384) + ((k * 384) + 32) + (2*32);

  int polyveccompressedbytes, polycompressedbytes;
  if (KEYSIZE==1024) {polyveccompressedbytes = k * 320; polycompressedbytes=128;}
  else if (KEYSIZE==2048) {polyveccompressedbytes = k * 320; polycompressedbytes=128;}
  else if (KEYSIZE==4096) {polyveccompressedbytes = k * 352; polycompressedbytes=128;}
  int crypto_ciphertextbytes = polyveccompressedbytes + polycompressedbytes;

  // INITIALIZING PRIVATEKEY, SECRETKEY, AND CIPHERTEXT FOR TESTING 
  rsa_key pk;
  rsa_key sk;
  unsigned char secrettext[crypto_secretkeybytes];
  unsigned char ciphertext[crypto_ciphertextbytes];

  // INITIALIZE PSUEDO-RANDOM NUMBER GENERATOR
  prng_state prng;
  rng_make_prng(KEYSIZE, find_prng("fortuna"), &prng, NULL);
  fortuna_start(&prng);

  /////////////////////// STARTING SPEED TESTS //////////////////////// 
  int i = 0;
  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    int err = rsa_keypair_generate(KEYSIZE, &prng, &pk, &sk);
    if (err == CRYPT_ERROR) printf("FAILED TO DECRYPT");
  }
  print_results("rsa_keypair: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    int err = rsa_encrypt(&sk, crypto_secretkeybytes, 
                &ciphertext, &crypto_ciphertextbytes, 
                &prng, &pk);
    if (err == CRYPT_ERROR) printf("FAILED TO DECRYPT");
  }
  print_results("rsa_encaps: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    unsigned char dk[crypto_secretkeybytes];
    int err = rsa_decrypt(&ciphertext, crypto_ciphertextbytes, 
                &dk, &crypto_secretkeybytes,
                &prng, &sk);
    if (err == CRYPT_ERROR) printf("FAILED TO DECRYPT");
  }
  print_results("rsa_decaps: ", t, NTESTS);
  //////////////////////// DONE WITH SPEED TESTS ///////////////////////

  // Clean up PRNG
  fortuna_done(&prng); 

  return 0;
}