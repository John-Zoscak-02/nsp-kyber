#ifndef ENCRYPT_TEXT_H
#define ENCRYPT_TEXT_H

int rsa_keypair_generate(int keysize, prng_state *prng, rsa_key *pk, rsa_key *sk);

int rsa_encrypt(unsigned char *plaintext, unsigned long plaintext_len,
                unsigned char *ciphertext, unsigned long *ciphertext_len,
                prng_state *prng, rsa_key *publickey);

int rsa_decrypt(unsigned char *ciphertext, unsigned long ciphertext_len,
                unsigned char *plaintext, unsigned long *plaintext_len,
                prng_state *prng, rsa_key *privatekey);

#endif
