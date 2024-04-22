#include "CCryptography.h"

CCryptography::CCryptography() {
    //Initiate randomness.
    std::srand(std::time(nullptr));
}

std::string CCryptography::sha256(const std::string stringToHash) const {
    //Initialize EVP objects from the openssl library.
    EVP_MD_CTX *ctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();
    unsigned int hash_len;
    unsigned char hashOutput[SHA256_DIGEST_LENGTH];

    //Hash.
    EVP_DigestInit_ex(ctx, md, nullptr);
    EVP_DigestUpdate(ctx, stringToHash.c_str(), stringToHash.size());
    EVP_DigestFinal_ex(ctx, hashOutput, &hash_len);

    //Free the allocated EVP object.
    EVP_MD_CTX_free(ctx);

    //Convert received output to hexadecimal string.
    std::ostringstream hashOutputOss;
    for (size_t i = 0; i < hash_len; ++i) {
        hashOutputOss << std::setfill('0') << std::setw(2) << std::hex << (int) hashOutput[i];
    }
    return hashOutputOss.str();
}
