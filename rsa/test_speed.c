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

//extern const ltc_math_descriptor tfm_desc;
extern const ltc_math_descriptor ltm_desc;

int main(void)
{
#ifdef $USE_LTM
ltc_mp = ltm_desc;
#elif defined $USE_TFM
ltc_mp = tfm_desc;
#endif

  uint64_t t[NTESTS];

  // DEFINING PK, SK, CIPHERTEXT LENGTHS....
  int k = (log(KEYSIZE) / log(2)) - 8;
  int crypto_secretkeybytes = (k * 384) + ((k * 384) + 32) + (2*32);
  int crypto_publickeybytes = (k * 384) + 32;

  int polyveccompressedbytes, polycompressedbytes;
  if (KEYSIZE==1024) {polyveccompressedbytes = k * 320; polycompressedbytes=128;}
  else if (KEYSIZE==2048) {polyveccompressedbytes = k * 320; polycompressedbytes=128;}
  else if (KEYSIZE==4096) {polyveccompressedbytes = k * 352; polycompressedbytes=128;}
  int crypto_ciphertextbytes = polyveccompressedbytes + polycompressedbytes;

  // INITIALIZING PRIVATEKEY, SECRETKEY, AND CIPHERTEXT FOR TESTING 
  int err, hash_idx, prng_idx, res;
  rsa_key key;
  unsigned char secretkey1[crypto_secretkeybytes];
  unsigned char secretkey2[crypto_secretkeybytes];
  unsigned char ciphertext[crypto_ciphertextbytes];

  // REGISTERING PRNG and HASH 
  //prng_state prng;
  //rng_make_prng(KEYSIZE, find_prng("fortuna"), &prng, NULL);
  //if (fortuna_start(&prng) != CRYPT_OK) {
  //  return 1;
  //}
  
  /* register prng/hash */
  if (register_prng(&sprng_desc) == -1) {
    return 1;
  }
  /* register a math library (in this case TomsFastMath) */
  if (register_hash(&sha1_desc) == -1) {
    return 1;
  }
  hash_idx = find_hash("sha1");
  prng_idx = find_prng("sprng");

  /////////////////////// STARTING SPEED TESTS //////////////////////// 
  int i = 0;
  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    //int err = rsa_keypair_generate(KEYSIZE, &prng, &pk, &sk);
    //if (err == CRYPT_ERROR) printf("FAILED TO GENERATE KEY");
    /* make an RSA-1024 key */
    if ((err = rsa_make_key(NULL, /* PRNG state */
                prng_idx, /* PRNG idx */
                KEYSIZE/8, /* 1024-bit key */
                65537, /* we like e=65537 */
                &key) /* where to store the key */
        ) != CRYPT_OK) {
      printf("FAILED TO GENERATE KEY");
      return 1;
    }
  }
  print_results("rsa_keypair: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    //int err = rsa_encrypt(&sk, crypto_secretkeybytes, 
    //            &ciphertext, &crypto_ciphertextbytes, 
    //            &prng, &pk);
    //if (err == CRYPT_ERROR) printf("FAILED TO DECRYPT");
    if ((err = rsa_encrypt_key(
                secretkey1, /* data we wish to encrypt */
                crypto_secretkeybytes, /* data is 16 bytes long */
                ciphertext, /* where to store ciphertext */
                crypto_ciphertextbytes, /* length of ciphertext */
                "test_rsa_speed", /* our lparam for this program */
                14, /* lparam is 7 bytes long */
                NULL, /* PRNG state */
                prng_idx, /* prng idx */
                hash_idx, /* hash idx */
                &key) /* our RSA key */
        ) != CRYPT_OK) {
      printf("FAILED TO ENCRYPT");
      return 1;
    }
  }
  print_results("rsa_encaps: ", t, NTESTS);

  for(i=0;i<NTESTS;i++) {
    t[i] = cpucycles();
    //unsigned char dk[crypto_secretkeybytes];
    //int err = rsa_decrypt(&ciphertext, crypto_ciphertextbytes, 
    //            &dk, &crypto_secretkeybytes,
    //            &prng, &sk);
    //if (err == CRYPT_ERROR) printf("FAILED TO DECRYPT");
    if ((err = rsa_decrypt_key(
                ciphertext, /* encrypted data */
                crypto_ciphertextbytes, /* length of ciphertext */
                secretkey2, /* where to put plaintext */
                crypto_secretkeybytes, /* plaintext length */
                "test_rsa_speed", /* lparam for this program */
                14, /* lparam is 7 bytes long */
                hash_idx, /* hash idx */
                &res, /* validity of data */
                &key) /* our RSA key */
      ) != CRYPT_OK) {
      printf("FAILED TO DECRYPT");
      return 1;
    }
  }
  print_results("rsa_decaps: ", t, NTESTS);
  //////////////////////// DONE WITH SPEED TESTS ///////////////////////

  // Clean up PRNG
  return 0;
}