#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "CBlockchain.h"

#pragma once

class CServer
{
public:
	CServer() = default;
	int bindSocket(const int port);
	std::string parseString(const char * buffer, size_t & index, const size_t bytesReceived);
	std::vector<std::string> parseTransactionArray(const char * buffer, size_t index, const size_t bytesReceived);
	void createNewTransaction(const char * buffer, size_t & index, const size_t bytes_received);
	void loadTransaction(const char * buffer, size_t & index, const size_t bytes_received);
	void loadCoinbaseTransaction(const char * buffer, size_t & index, const size_t bytes_received);
	void proposeBlock(const char * buffer, size_t & index, const size_t bytes_received);
	void stake(const char * buffer, size_t & index, const size_t bytes_received);
	void countNextValidator(const char * buffer, size_t & index, const size_t bytes_received);
	void run();
private:
	int m_socket;
	sockaddr_in m_serverInfo;
	CBlockchain m_blockchain;
	CTransactionContainer m_unspentTransactions, m_mempool, m_stakepool, m_oldStakepool;
	std::string m_address;
};
