#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/evp.h>

#pragma once

/**
 * Helper class providing basic cryptographic and cryptographic-like functions.
 */
class CCryptography {
public:
    /**
     * Constructor is needed to initiate randomness.
     */
    CCryptography();

    /**
     * Function to return sha256 hash of a provided string.
     * @param stringToHash String to be hashed.
     * @return sha256 hash of the provided string.
     */
    std::string sha256(const std::string stringToHash) const;
};
