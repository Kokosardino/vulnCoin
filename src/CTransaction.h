#include <iostream>

#include "CCryptography.h"

#pragma once

class CTransaction
{
public: 
	CTransaction(const std::string address, const int ammount, const int passed, const bool coinbase);
	CTransaction(const std::string address, const int ammount, const int passed, const bool coinbase, const time_t timestamp);
	void print() const;
	std::string m_address, m_txid;
	int m_ammount, m_passed;
        bool m_coinbase;
	std::time_t m_timestamp;
};
