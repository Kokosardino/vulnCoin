#include "CBlock.h"
#include "CTransactionContainer.h"

#pragma once

class CBlockchain
{
public:
	CBlockchain();
	std::string generateToAddress(const std::string & address, CTransactionContainer & m_transactions);
	void addBlock(const std::vector<std::string> & transactions);
	std::string print() const;
	size_t getBlockCount() const;
	std::string getLastBlockHash() const;
private:
	std::vector<CBlock> m_blockchain; 
};
