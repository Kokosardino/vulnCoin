#include <vector>

#include "CCryptography.h"
#include "CTransaction.h"

#pragma once

class CBlock
{
public:
	CBlock(const std::string prevBlockHash, const std::string transactionsHash, const std::vector<std::string> transactions);
	void print() const;
	std::string getBlockHash() const;

private:
	std::string m_prevBlockHash;
	std::string m_transactionsHash;
	std::vector<std::string> m_transactions;
};
