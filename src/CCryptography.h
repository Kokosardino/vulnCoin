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

    /**
     * Primitive function to generate address. Function has many flaws, mainly:
     *      1. If servers are started at the same time, they receive the same addresses. In PoC code, we tackle this by implementing a one second delay between server starts.
     *      2. No cryptographic proofs can be generated via addresses. ->   Anybody can create a transaction with anybodies asset. For purposes of our simulation, we tackle this problem by presuming that all of the network nodes act honestly. We do NOT use this behavior anywhere in our PoCs.
     * Possible future improvement -> Implement public key cryptography.
     * @return The generated address.
     */
    std::string generateAddress() const;

private:
    //Charset to choose hex characters from.
    const std::string m_charset = "abcdef1234567890";
    //Fixed lengths of the address and charset. Address length CANNOT be changed under 32 bits (ADDRESS_SIZE = 16).
    const size_t ADDRESS_SIZE = 64, CHARSET_SIZE = 15;
};
