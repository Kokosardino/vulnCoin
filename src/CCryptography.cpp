#include "CCryptography.h"

CCryptography::CCryptography()
{
	std::srand(std::time(nullptr));
}

std::string CCryptography::sha256(const std::string stringToHash) const
{
	EVP_MD_CTX * ctx = EVP_MD_CTX_new();
	const EVP_MD * md = EVP_sha256();
	unsigned int hash_len;
	unsigned char hashOutput[SHA256_DIGEST_LENGTH];

	EVP_DigestInit_ex(ctx, md, nullptr);
	EVP_DigestUpdate(ctx, stringToHash.c_str(), stringToHash.size());
	EVP_DigestFinal_ex(ctx, hashOutput, &hash_len);

	EVP_MD_CTX_free(ctx);

	std::ostringstream hashOutputOss;
	for(size_t i = 0; i < hash_len; ++i)
	{
		hashOutputOss << std::setfill('0') << std::setw(2) << std::hex << (int)hashOutput[i];
	}

	return hashOutputOss.str();
}

std::string CCryptography::generateAddress() const
{
	std::ostringstream address;
	for(size_t i = 0; i < ADDRESS_SIZE; ++i)
	{
		address << m_charset[std::rand() % CHARSET_SIZE];
	}		

	return address.str();
}
