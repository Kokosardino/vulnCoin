#include <map>

#include "CTransaction.h"

#include "../libs/rang.hpp"

#pragma once

class CTransactionContainer
{
public:
	CTransactionContainer() = default;
	CTransaction getTransactionByTxid(const std::string txid);
	void addTransaction(const CTransaction transaction);
	void print() const;
	void print(const std::string & address) const;
	int returnPassed(const std::string & txid, const std::string & address);
	bool findTransactions(const std::vector<std::string> & transactions) const;
	void removeTransactionByTxid(const std::string & txid);
	size_t size() const;

	CTransaction operator[](size_t index);
private:
	//Transactions sorted by txid.
	std::map<std::string, CTransaction> m_container;
};
