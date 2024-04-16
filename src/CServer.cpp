#include "CServer.h"

int CServer::bindSocket(const int port)
{
        //Create the socket bound to IPv4 and TCP.	
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(m_socket == -1)
	{		
		std::cerr << rang::fg::red << rang::style::bold << "Cannot create the socket, exiting!" << rang::style::reset << std::endl;
		return 1;
	}

	//Bind to the socket.
	m_serverInfo.sin_port = htons(port);
	m_serverInfo.sin_family = AF_INET;
	m_serverInfo.sin_addr.s_addr = INADDR_ANY;
	if(bind(m_socket, (struct sockaddr*)&m_serverInfo, sizeof(m_serverInfo)) == -1)
	{
       		std::cerr << rang::fg::red << rang::style::bold << "Cannot bind to the socket on port [" << port << "], exiting!" << rang::style::reset << std::endl;
		return 1;
	}

	//Start listening for incoming connections. -> [20] ensures we never surpass the number in simulation.
	if(listen(m_socket, 20) == -1)
	{
		std::cerr << "Cannot start listening on the socket, exiting!" << std::endl;
		return 1;
	}

	std::cout << rang::fg::green << rang::style::bold << "Server is listening to incoming communication on port [" << port << "]." << rang::style::reset << std::endl;

	return 0;
}

std::string CServer::parseString(const char * buffer, size_t & index, const size_t bytesReceived)
{ 
	size_t startIndex = index, cnt = 0;
	for(; buffer[index] != '\n' && buffer[index] != ' ' && buffer[index] != '\0' && index < bytesReceived; ++index)
	{
		++cnt;
	}
	index++;
	return std::string(buffer).substr(startIndex, cnt);
}

std::vector<std::string> CServer::parseTransactionArray(const char * buffer, size_t index, const size_t bytesReceived)
{
	index++;
	std::vector<std::string> transactions;
	while(buffer[index] != '}' && index < bytesReceived - 1)
	{
		std::ostringstream oss;
		while(buffer[index] != '}' && buffer[index] != ' ' && buffer[index] != '\n')
		{
			oss << buffer[index];
			++index;
		}
		++index;
		transactions.push_back(oss.str());
	}
	return transactions;
}

bool CServer::createNewTransaction(const char * buffer, size_t & index, const size_t bytesReceived)
{
	std::string txid = this->parseString(buffer, index, bytesReceived), addressSender = this->parseString(buffer, index, bytesReceived), addressReceiver = this->parseString(buffer, index, bytesReceived);
	int returnPassed = m_unspentTransactions.returnPassed(txid, addressSender);
	if(returnPassed != -1)
	{
		m_unspentTransactions.removeTransactionByTxid(txid);
		m_mempool.addTransaction(CTransaction(addressReceiver, 10, returnPassed + 1, false));
   		return 0;		
	} 
	return 1;
	
}

bool CServer::loadTransaction(const char * buffer, size_t & index, const size_t bytesReceived)
{
	std::string txid = this->parseString(buffer, index, bytesReceived), addressSender = this->parseString(buffer, index, bytesReceived), addressReceiver = this->parseString(buffer, index, bytesReceived), timestampString = this->parseString(buffer, index, bytesReceived);
			
	int returnPassed = m_unspentTransactions.returnPassed(txid, addressSender);
			
	if(returnPassed != -1)
	{
		m_unspentTransactions.removeTransactionByTxid(txid);
		long long timestampLong = std::stoll(timestampString);
		time_t timestamp = static_cast<time_t>(timestampLong);
		m_mempool.addTransaction(CTransaction(addressReceiver, 10, returnPassed + 1, false, timestamp));	     
		return 0;
	} 
	return 1;
}

void CServer::loadCoinbaseTransaction(const char * buffer, size_t & index, const size_t bytesReceived)
{
	std::string addressReceiver = this->parseString(buffer, index, bytesReceived), timestampString = this->parseString(buffer, index, bytesReceived);
			
	long long timestampLong = std::stoll(timestampString);
	time_t timestamp = static_cast<time_t>(timestampLong);
	m_mempool.addTransaction(CTransaction(addressReceiver, 10, 0, true, timestamp));	    }

bool CServer::proposeBlock(const char * buffer, size_t & index, const size_t bytesReceived)
{
	std::vector<std::string> transactions = this->parseTransactionArray(buffer, index, bytesReceived);
	if(m_mempool.findTransactions(transactions))
	{
		for(const std::string & txid : transactions)
		{
			m_unspentTransactions.addTransaction(m_mempool.getTransactionByTxid(txid));
			m_mempool.removeTransactionByTxid(txid);
		}
		m_blockchain.addBlock(transactions);
		
		return 0;
	}
	return 1;
}

bool CServer::stake(const char * buffer, size_t & index, const size_t bytesReceived)
{
	std::string txid = this->parseString(buffer, index, bytesReceived), addressSender = this->parseString(buffer, index, bytesReceived);
	if(m_unspentTransactions.returnPassed(txid, addressSender) != -1)
	{
		m_stakepool.addTransaction(m_unspentTransactions.getTransactionByTxid(txid));	 
		m_unspentTransactions.removeTransactionByTxid(txid);
		return 0;
	}
	return 1;
}

std::string CServer::countNextValidator(const char * buffer, size_t & index, const size_t bytesReceived)
{
	unsigned int validator = 0, x;
	//Convert first 32 bits of lastBlockHash into u_int and modulate it. Will overflow and break the attack on architectures that implement u_int as 16-bit instead of 32-bit.
	
	sscanf(m_blockchain.getLastBlockHash().substr(0, 16).c_str(), "%x", &x);
	validator = x % m_stakepool.size();
	
	//Convert first 32 bits of address into u_int and modulate it. Will overflow and break the attack on architectures that implement u_int as 16-bit instead of 32-bit.
	for (size_t i = 0; i < m_oldStakepool.size(); ++i) {
		sscanf(m_oldStakepool[i].m_address.substr(0, 16).c_str(), "%x", &x);
		validator += x % m_stakepool.size();
		std::cout << validator << std::endl;
	}
	
	size_t stakepoolSize = m_stakepool.size();
	m_oldStakepool = m_stakepool;
	while(m_stakepool.size() != 0)
	{
		m_unspentTransactions.addTransaction(m_stakepool[0]);
		m_stakepool.removeTransactionByTxid(m_stakepool[0].m_txid);
	}

	
	//Return result.
	return m_oldStakepool[validator % stakepoolSize].m_address;
}

void CServer::run()
{
	CCryptography crypto;
	m_address = crypto.generateAddress();
	
	bool runFlag = true;
	while(runFlag)
	{
		sockaddr_in clientInfo;
		socklen_t clientSize = sizeof(clientInfo);
		int socketClient = accept(m_socket, (sockaddr*)&clientInfo, &clientSize);

		char buffer[1024];
		size_t bytesReceived = recv(socketClient, buffer, sizeof(buffer), 0);
		size_t index = 0;
		std::string choice = this->parseString(buffer, index, bytesReceived);
	
		if(choice == "stop")
		{
			close(m_socket);
			runFlag = false;
		} else if(choice == "getBlockCount")
		{
			char response[256];
			snprintf(response, sizeof response, "%zu", m_blockchain.getBlockCount());
			send(socketClient, response, strlen(response), 0);
		} else if(choice == "printBlockchain")
		{
			std::string response = m_blockchain.print();
			send(socketClient, response.c_str(), response.size(), 0);
		} else if(choice == "createNewTransaction")
		{
			if(this->createNewTransaction(buffer, index, bytesReceived) != 0)
			{
				const char * response = "Cannot create the transaction.";	
				send(socketClient, response, strlen(response), 0);
			}
		} else if(choice == "loadTransaction")
		{
			if(this->loadTransaction(buffer, index, bytesReceived) != 0)
			{
				const char * response = "Cannot create the transaction.";	
				send(socketClient, response, strlen(response), 0);
			}
		} else if(choice == "loadCoinbaseTransaction")
		{
			this->loadCoinbaseTransaction(buffer, index, bytesReceived);		
		} else if(choice == "proposeBlock")
		{
			if(this->proposeBlock(buffer, index, bytesReceived) != 0)
			{
				const char * response = "Cannot create the proposed block.";	
				send(socketClient, response, strlen(response), 0);
			}
		} else if(choice == "stake") 
		{
			if(this->stake(buffer, index, bytesReceived) != 0 )
			{
				const char * response = "Specified transaction is not in the pool of unspent transactions.";	
				send(socketClient, response, strlen(response), 0);
			}
		} else if(choice == "countNextValidator")
		{
			
			std::string response = this->countNextValidator(buffer, index, bytesReceived);
			send(socketClient, response.c_str(), response.size(), 0);
		} else if(choice == "listMempool")
		{
			std::string response = m_mempool.print();
			send(socketClient, response.c_str(), response.size(), 0);
		} else if(choice == "listMempoolLinkedToMe")
		{
			std::string response = m_mempool.print(m_address);
			send(socketClient, response.c_str(), response.size(), 0);
		} else if(choice == "listUnspent")
		{
			std::string response = m_unspentTransactions.print();
			send(socketClient, response.c_str(), response.size(), 0);
		} else if(choice == "listUnspentLinkedToMe")
		{
			std::string response = m_unspentTransactions.print(m_address);
			send(socketClient, response.c_str(), response.size(), 0);
		} else if(choice == "listStakepool")
		{
			std::string response = m_stakepool.print();
			send(socketClient, response.c_str(), response.size(), 0);
		} else if(choice == "listOldStakepool")
		{
			std::string response = m_oldStakepool.print(m_address);
			send(socketClient, response.c_str(), response.size(), 0);
		} else if(choice == "generate")
		{
			m_blockchain.generateToAddress(m_address, m_unspentTransactions);
		} else if(choice == "printAddress")
		{
			send(socketClient, m_address.c_str(), m_address.size(), 0);
		} else
		{
			std::ostringstream oss;
		        oss << "Unknown command: \"" << std::string(buffer, bytesReceived - 1) << "\"" << std::endl;
			send(socketClient, oss.str().c_str(), oss.str().size(), 0);
		} 
		close(socketClient);
	}
}
