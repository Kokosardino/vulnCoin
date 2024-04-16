#include "CTransaction.h"

CTransaction::CTransaction(const std::string address, const int ammount, const int passed, bool coinbase)
{
	m_address = address;
	m_ammount = ammount;
	m_passed = passed;
	m_coinbase = coinbase;
	m_timestamp = std::time(0);

	std::ostringstream toBeHashedOss;
	toBeHashedOss << address << ammount << passed << coinbase << m_timestamp;
	CCryptography crypto;
	m_txid = crypto.sha256(toBeHashedOss.str());
}

CTransaction::CTransaction(const std::string address, const int ammount, const int passed, bool coinbase, const time_t timestamp)
{
	m_address = address;
	m_ammount = ammount;
	m_passed = passed;
	m_coinbase = coinbase;
	m_timestamp = timestamp;

	std::ostringstream toBeHashedOss;
	toBeHashedOss << address << ammount << passed << coinbase << m_timestamp;
	CCryptography crypto;
	m_txid = crypto.sha256(toBeHashedOss.str());
}

std::string CTransaction::print() const
{
	std::ostringstream oss;
	oss << "{" << std::endl
		<< "	\"address\": \"" << m_address << "\"," << std::endl
		<< "	\"txid\": \"" << m_txid << "\"," << std::endl
		<< "	\"ammount\": \"" << m_ammount << "\"," << std::endl
		<< "	\"coinbase\": \"" << m_coinbase << "\"," << std::endl
		<< "	\"passed\": \"" << m_passed <<  "\"," << std::endl
		<< "	\"timestamp\": \"" << m_timestamp <<  "\"" << std::endl
		<< "}";
	return oss.str();
}
