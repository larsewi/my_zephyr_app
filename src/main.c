/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <zephyr/kernel.h>
#include <psa/crypto.h>

#define EXPECTED_HASH "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad"

int main(void)
{
    psa_status_t status;
    psa_algorithm_t alg = PSA_ALG_SHA_256;
    psa_hash_operation_t operation = PSA_HASH_OPERATION_INIT;
    unsigned char input[] = { 'a', 'b', 'c' };
    unsigned char actual_hash[PSA_HASH_MAX_SIZE];
    size_t actual_hash_len;

    fflush(stdout);

    /* Initialize PSA Crypto */
    status = psa_crypto_init();
    if (status != PSA_SUCCESS) {
        printf("Failed to initialize PSA Crypto\n");
        return 1;
    }

    /* Compute hash of message  */
    status = psa_hash_setup(&operation, alg);
    if (status != PSA_SUCCESS) {
        printf("Failed to begin hash operation\n");
        return 1;
    }
    status = psa_hash_update(&operation, input, sizeof(input));
    if (status != PSA_SUCCESS) {
        printf("Failed to update hash operation\n");
        return 1;
    }
    status = psa_hash_finish(&operation, actual_hash, sizeof(actual_hash),
                             &actual_hash_len);
    if (status != PSA_SUCCESS) {
        printf("Failed to finish hash operation\n");
        return 1;
    }

    printf("Hashed message from %s: ", CONFIG_BOARD_TARGET);

	for (size_t i = 0; i < actual_hash_len; i++) {
		printf("%02x", actual_hash[i]);
	}
	printf("\n");

    assert(strncmp(actual_hash, EXPECTED_HASH, actual_hash_len));

    /* Clean up hash operation context */
    psa_hash_abort(&operation);

    mbedtls_psa_crypto_free();

	return 0;
}
