#include <stdlib.h>
#include <tomcrypt.h>
#include <stdio.h>

int rsa_keypair_generate(int keysize, prng_state *prng, rsa_key *pk, rsa_key *sk) {
    int err = rsa_make_key(prng, find_prng("fortuna"), KEYSIZE, 65537, pk);
    if (err != CRYPT_OK) {
        printf("FAILED TO GENERATE KEYPAIR");
        return CRYPT_ERROR;
    }
    return CRYPT_OK;
}

int rsa_encrypt(unsigned char *plaintext, unsigned long plaintext_len,
                unsigned char *ciphertext, unsigned long *ciphertext_len,
                prng_state *prng, rsa_key *publickey) {
    // 1. Check key validity
    if (publickey == NULL || publickey->type != PK_PUBLIC) {
        return CRYPT_ERROR; // Handle invalid key
    }

    // 2. Determine required buffer size for ciphertext (depends on padding)
    int block_size = (KEYSIZE / 8) - 42;
    unsigned long output_len = (KEYSIZE + LTC_PAD_PKCS7) & ~(block_size - 1);
    if (output_len < plaintext_len) {
        return CRYPT_ERROR; // Handle message too large for key
    }

    // 3. Apply PKCS#1 v1.5 padding (or alternative padding)
    int err = rsa_encrypt_key_ex(plaintext, plaintext_len, 
                                ciphertext, &output_len, 
                                NULL, 0,
                                prng, -1, 
                                LTC_PKCS_1_V1_5, -1,
                                LTC_PAD_PKCS7, publickey);

    // 4. Handle encryption result
    if (err != CRYPT_OK) {
        return CRYPT_ERROR; // Handle encryption error
    }

    // 5. Update ciphertext length
    *ciphertext_len = output_len;

    return CRYPT_OK;
}

int rsa_decrypt(unsigned char *ciphertext, unsigned long ciphertext_len,
                unsigned char *plaintext, unsigned long *plaintext_len,
                rsa_key *privatekey) {
    // 1. Check key validity
    if (privatekey == NULL || privatekey->type != PK_PRIVATE) {
        return CRYPT_ERROR; // Handle invalid key
    }

    // 2. Determine expected output size based on key size
    unsigned long output_len = KEYSIZE;

    // 3. Perform decryption with padding validation
    int err = rsa_decrypt_key_ex(ciphertext, ciphertext_len, 
                                 plaintext, &output_len, 
                                 NULL, 0, 
                                 LTC_PKCS_1_V1_5, -1,
                                 LTC_PAD_PKCS7, 
                                 NULL, privatekey);

    // 4. Handle decryption result
    if (err != CRYPT_OK) {
        return CRYPT_ERROR; // Handle decryption error or invalid padding
    }

    // 5. Update plaintext length (assuming successful decryption)
    *plaintext_len = output_len;

    return CRYPT_OK;
}
