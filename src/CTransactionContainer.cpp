#include "CTransactionContainer.h"

void CTransactionContainer::addTransaction(const CTransaction transaction)
{
	m_container.insert(std::make_pair(transaction.m_txid, transaction));
}

CTransaction CTransactionContainer::getTransactionByTxid(const std::string txid)
{
	return (m_container.find(txid))->second;
}

std::string CTransactionContainer::print() const
{
	std::ostringstream oss;
	oss << "]" << std::endl;
	for(std::map<std::string, CTransaction>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
	{
		oss << it->second.print();
	}
	oss << "]" << std::endl;
	return oss.str();
}

std::string CTransactionContainer::print(const std::string & address) const
{
	std::ostringstream oss;
	oss << "[" << std::endl;
	for(std::map<std::string, CTransaction>::const_iterator it = m_container.begin(); it != m_container.end(); ++it)
	{
		if(address == it->second.m_address)
		{

			oss << it->second.print();
		}
	}
	oss << "]" << std::endl;
	return oss.str();
}

bool CTransactionContainer::findTransactions(const std::vector<std::string> & transactions) const
{
	for(const std::string & txid : transactions)
	{
		if(m_container.find(txid) == m_container.end())
		{
			std::cerr << rang::fg::red << rang::style::bold << "Transaction of txid [" << txid << "] was not found in the mempool." << rang::style::reset << std::endl;
			return 0;
		}
	}
	return 1;
}

int CTransactionContainer::returnPassed(const std::string & txid, const std::string & address)
{
	std::map<std::string, CTransaction>::iterator it = m_container.find(txid);
	
	if(it != m_container.end() && it->second.m_address == address)
	{
		return it->second.m_passed;
	}
	
	return -1;
}

void CTransactionContainer::removeTransactionByTxid(const std::string & txid)
{
	std::map<std::string, CTransaction>::iterator it = m_container.find(txid);
	if(it != m_container.end())
	{
		m_container.erase(it);
	}
}

size_t CTransactionContainer::size() const
{
	return m_container.size();
}

CTransaction CTransactionContainer::operator[](size_t index)
{
	std::map<std::string, CTransaction>::iterator it = m_container.begin();
	std::advance(it, index);
	return it->second;
}
