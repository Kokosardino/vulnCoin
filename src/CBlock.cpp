#include "CBlock.h"

CBlock::CBlock(const std::string prevBlockHash, const std::string transactionsHash, const std::vector<std::string> transactions): m_prevBlockHash(prevBlockHash), m_transactionsHash(transactionsHash), m_transactions(transactions)
{}

std::string CBlock::print() const
{
	std::ostringstream oss;
	oss 	<< "{" << std::endl 
		<< "	\"prevBlockHash\": 	\"" << m_prevBlockHash << "\"," << std::endl
		<< "	\"transactionsHash\":	\"" << m_transactionsHash << "\"," << std::endl
		<< "	\"transactions\": 	[" << std::endl;
	
	for(size_t transactionCnt = 0; transactionCnt < m_transactions.size(); ++transactionCnt)
	{
		oss << "					\"" << m_transactions[transactionCnt] << "\"";
		if(transactionCnt != m_transactions.size() - 1)
		{
			oss << ",";
		}
		oss << std::endl;

	}
	oss 	<< "    	      			]" << std::endl << "}";
	
	return oss.str();
}

std::string CBlock::getBlockHash() const
{
	std::ostringstream oss;
        oss << m_prevBlockHash << m_transactionsHash;
	CCryptography crypto;
	
	return crypto.sha256(oss.str());
}
