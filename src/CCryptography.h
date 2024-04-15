#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/evp.h>

#pragma once

class CCryptography
{
public:
	CCryptography();
	std::string sha256(const std::string stringToHash) const;	
	std::string generateAddress() const;
private:
	const std::string m_charset = "abcdef1234567890";
	const size_t ADDRESS_SIZE = 64, CHARSET_SIZE = 15;
};
