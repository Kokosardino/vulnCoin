#include "CBlock.h"

CBlock::CBlock(const std::string prevBlockHash, const std::string transactionsHash, const std::vector<std::string> transactions): m_prevBlockHash(prevBlockHash), m_transactionsHash(transactionsHash), m_transactions(transactions)
{}

void CBlock::print() const
{
	std::cout << "{" << std::endl 
		<< "	prevBlockHash: 		\"" << m_prevBlockHash << "\"" << std::endl
		<< "	transactionsHash:	\"" << m_transactionsHash << "\"" << std::endl
		<< "	transactions: 		[" << std::endl;
	
	for(const std::string & transaction : m_transactions)
	{
		std::cout << "					\"" << transaction << "\"" << std::endl;

	}
	std::cout << "    	      			]" << std::endl << "}" << std::endl;
}

std::string CBlock::getBlockHash() const
{
	std::ostringstream oss;
        oss << m_prevBlockHash << m_transactionsHash;
	CCryptography crypto;
	
	return crypto.sha256(oss.str());
}
