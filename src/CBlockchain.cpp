#include "CBlockchain.h"

CBlockchain::CBlockchain()
{
	//Crete genesis block.
	m_blockchain.emplace_back(CBlock("0000000000000000000000000000000000000000000000000000000000000000", "0000000000000000000000000000000000000000000000000000000000000000", std::vector<std::string>()));
}

std::string CBlockchain::generateToAddress(const std::string & address, CTransactionContainer & transactions)
{
	CTransaction coinbaseTrans(address, 10, 0, true);
	CCryptography crypto;
	m_blockchain.emplace_back(CBlock(m_blockchain[m_blockchain.size() - 1].getBlockHash(), crypto.sha256(coinbaseTrans.m_txid), {coinbaseTrans.m_txid}));

	transactions.addTransaction(coinbaseTrans);

	return m_blockchain[m_blockchain.size()-1].getBlockHash();	
}

void CBlockchain::addBlock(const std::vector<std::string> & transactions)
{
	std::string transactionHash;
	if(transactions.empty())
	{
		transactionHash = "0000000000000000000000000000000000000000000000000000000000000000";
	} else
	{
		std::ostringstream oss;
		for(const std::string & txid : transactions)
		{
			oss << txid;
		}
		transactionHash = oss.str();
	}

	CCryptography crypto;
	m_blockchain.emplace_back(CBlock(m_blockchain[m_blockchain.size() - 1].getBlockHash(), crypto.sha256(transactionHash), transactions));
}

std::string CBlockchain::print() const
{
	std::ostringstream oss;
	for(size_t i = 0; i < m_blockchain.size(); ++i)
	{
		oss << m_blockchain[i].print();
	}

	return oss.str();
}

size_t CBlockchain::getBlockCount() const
{
	return m_blockchain.size();
}

std::string CBlockchain::getLastBlockHash() const
{
	return m_blockchain[m_blockchain.size() - 1].getBlockHash();
}

