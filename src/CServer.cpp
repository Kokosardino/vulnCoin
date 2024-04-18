#include "CServer.h"

int CServer::bindSocket(const int port) {
    //Create the socket bound to IPv4 and TCP.
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket == -1) {
        return 1;
    }

    //Bind to the socket.
    m_serverInfo.sin_port = htons(port);
    m_serverInfo.sin_family = AF_INET;
    m_serverInfo.sin_addr.s_addr = INADDR_ANY;
    if (bind(m_socket, (struct sockaddr *) &m_serverInfo, sizeof(m_serverInfo)) == -1) {
        return 1;
    }

    //Start listening for incoming connections. -> [20] ensures we never surpass the number in simulation.
    if (listen(m_socket, 20) == -1) {
        return 1;
    }

    return 0;
}

std::string CServer::parseString(const char * buffer, size_t & index, const size_t bytesReceived) {
    size_t startIndex = index, cnt = 0;
    //Parse all characters until next whitespaces/binary zero.
    for (; buffer[index] != '\n' && buffer[index] != ' ' && buffer[index] != '\0' && index < bytesReceived; ++index) {
        ++cnt;
    }
    index++;
    return std::string(buffer).substr(startIndex, cnt);
}

std::vector <std::string> CServer::parseTransactionArray(const char * buffer, size_t index, const size_t bytesReceived) {
    //Jump over the '{' symbol.
    index++;
    std::vector <std::string> transactions;
    while (buffer[index] != '}' && index < bytesReceived - 1) {
        std::ostringstream oss;
        while (buffer[index] != '}' && buffer[index] != ' ' && buffer[index] != '\n') {
            oss << buffer[index];
            ++index;
        }
        ++index;
        transactions.push_back(oss.str());
    }
    return transactions;
}

std::string CServer::createNewTransaction(const char * buffer, size_t & index, const size_t bytesReceived) {
    std::string txid = this->parseString(buffer, index, bytesReceived), addressSender = this->parseString(buffer, index,
                                                                                                          bytesReceived), addressReceiver = this->parseString(
            buffer, index, bytesReceived);
    //Check whether the specified UTXO exists.
    int returnPassed = m_unspentTransactions.returnPassed(txid, addressSender);
    //If the specified UTXO was found.
    if (returnPassed != -1) {
        //Remove the specified UTXO from UTXOs.
        m_unspentTransactions.removeTransactionByTxid(txid);
        //Create new transaction inside the mempool.
        CTransaction newTx(addressReceiver, 10, returnPassed + 1, false);
        m_mempool.addTransaction(newTx);
        return newTx.m_txid;
    }
    //If the search for UTXO fails, return empty string.
    return "";
}

bool CServer::loadTransaction(const char * buffer, size_t & index, const size_t bytesReceived) {
    std::string txid = this->parseString(buffer, index, bytesReceived), addressSender = this->parseString(buffer, index,
                                                                                                          bytesReceived), addressReceiver = this->parseString(
            buffer, index, bytesReceived), timestampString = this->parseString(buffer, index, bytesReceived);
    //Check whether the specified UTXO exists.
    int returnPassed = m_unspentTransactions.returnPassed(txid, addressSender);
    //If the specified UTXO was found.
    if (returnPassed != -1) {
        //Remove the specified UTXO from UTXOs.
        m_unspentTransactions.removeTransactionByTxid(txid);
        //Get the current timestamp.
        long long timestampLong = std::stoll(timestampString);
        time_t timestamp = static_cast<time_t>(timestampLong);
        //Create new transaction inside the mempool.
        m_mempool.addTransaction(CTransaction(addressReceiver, 10, returnPassed + 1, false, timestamp));
        return 0;
    }
    return 1;
}

std::string CServer::loadCoinbaseTransaction(const char * buffer, size_t & index, const size_t bytesReceived) {
    std::string addressReceiver = this->parseString(buffer, index, bytesReceived), timestampString = this->parseString(
            buffer, index, bytesReceived);
    //Get the current timestamp.
    long long timestampLong = std::stoll(timestampString);
    time_t timestamp = static_cast<time_t>(timestampLong);
    //Create new coinbase transaction inside the mempool.
    CTransaction transaction(addressReceiver, 10, 0, true, timestamp);
    m_mempool.addTransaction(transaction);
    return transaction.m_txid;
}

bool CServer::proposeBlock(const char * buffer, size_t & index, const size_t bytesReceived) {
    std::vector <std::string> transactions = this->parseTransactionArray(buffer, index, bytesReceived);
    //Ensure that all specified transactions exist in the mempool.
    if (m_mempool.findTransactions(transactions)) {
        for (const std::string &txid: transactions) {
            //Add newly validated transactions into UTXOs pool.
            m_unspentTransactions.addTransaction(m_mempool.getTransactionByTxid(txid));
            //Remove newly validated transactions from mempool.
            m_mempool.removeTransactionByTxid(txid);
        }
        //Append the new block to the blockchain.
        m_blockchain.addBlock(transactions);
        return 0;
    }
    return 1;
}

bool CServer::stake(const char *buffer, size_t &index, const size_t bytesReceived) {
    std::string txid = this->parseString(buffer, index, bytesReceived), addressSender = this->parseString(buffer, index,
                                                                                                          bytesReceived);
    //Ensure that the specified transaction exists in the UTXOs.
    if (m_unspentTransactions.returnPassed(txid, addressSender) != -1) {
        //Add the specified transaction from stakepool.
        m_stakepool.addTransaction(m_unspentTransactions.getTransactionByTxid(txid));
        //Remove specified transaction from the UTXOs.
        m_unspentTransactions.removeTransactionByTxid(txid);
        return 0;
    }
    return 1;
}

std::string CServer::countNextValidator() {
    unsigned int validator = 0, x;

    //Convert first 32 bits of lastBlockHash into u_int and modulate it. Will overflow and break the attack on architectures that implement u_int as 16-bit instead of 32-bit.
    sscanf(m_blockchain.getLastBlockHash().substr(0, 16).c_str(), "%x", &x);
    validator = x % m_stakepool.size();

    //Convert first 32 bits of each address from the previous stakepool into u_int and modulate it. Will overflow and break the attack on architectures that implement u_int as 16-bit instead of 32-bit.
    for (size_t i = 0; i < m_oldStakepool.size(); ++i) {
        sscanf(m_oldStakepool[i].m_address.substr(0, 16).c_str(), "%x", &x);
        validator += x % m_stakepool.size();
    }

    //Save size of the current mempool before clearing it.
    size_t stakepoolSize = m_stakepool.size();
    m_oldStakepool = m_stakepool;
    while (m_stakepool.size() != 0) {
        //Move transactions from stakepool back betwwen other UTXOs.
        m_unspentTransactions.addTransaction(m_stakepool[0]);
        m_stakepool.removeTransactionByTxid(m_stakepool[0].m_txid);
    }

    //Return result address.
    return m_oldStakepool[validator % stakepoolSize].m_address;
}

std::string CServer::printTransaction(const char *buffer, size_t &index, const size_t bytesReceived) {
    std::string txid = this->parseString(buffer, index, bytesReceived);

    //Search for the txid in the mempool.
    std::string foundString = m_mempool.printTransaction(txid);
    if (foundString != "") {
        return foundString;
    }

    //Search for the txid in the stakepool.
    foundString = m_stakepool.printTransaction(txid);
    if (foundString != "") {
        return foundString;
    }

    //Search for the txid in the UTXOs.
    foundString = m_unspentTransactions.printTransaction(txid);
    return foundString;
}

void CServer::run() {
    //Generate address for the server.
    CCryptography crypto;
    m_address = crypto.generateAddress();

    //Server waits for commands until receiving the "stop" command.
    bool runFlag = true;
    while (runFlag) {
        //Create socket to the client.
        sockaddr_in clientInfo;
        socklen_t clientSize = sizeof(clientInfo);
        int socketClient = accept(m_socket, (sockaddr * ) & clientInfo, &clientSize);
        //Receive message from the client.
        char buffer[4096];
        size_t bytesReceived = recv(socketClient, buffer, sizeof(buffer), 0);
        buffer[bytesReceived] = '\0';
        //Parse command from the received buffer.
        size_t index = 0;
        std::string choice = this->parseString(buffer, index, bytesReceived);
        //Act accordingly to the specified command.
        if (choice == "stop") {
            close(m_socket);
            runFlag = false;
        } else if (choice == "getBlockCount") {
            char response[256];
            snprintf(response, sizeof response, "%zu", m_blockchain.getBlockCount());
            send(socketClient, response, strlen(response), 0);
        } else if (choice == "printBlockchain") {
            std::string response = m_blockchain.print();
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "createNewTransaction") {
            std::string response = this->createNewTransaction(buffer, index, bytesReceived);
            if (response != "") {
                send(socketClient, response.c_str(), response.size(), 0);
            } else {
                const char *error = "Cannot create the transaction.";
                send(socketClient, error, strlen(error), 0);
            }
        } else if (choice == "loadTransaction") {
            if (this->loadTransaction(buffer, index, bytesReceived) != 0) {
                const char *response = "Cannot create the transaction.";
                send(socketClient, response, strlen(response), 0);
            }
        } else if (choice == "loadCoinbaseTransaction") {
            std::string response = this->loadCoinbaseTransaction(buffer, index, bytesReceived);
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "proposeBlock") {
            if (this->proposeBlock(buffer, index, bytesReceived) != 0) {
                const char *response = "Cannot create the proposed block.";
                send(socketClient, response, strlen(response), 0);
            }
        } else if (choice == "stake") {
            if (this->stake(buffer, index, bytesReceived) != 0) {
                const char *response = "Specified transaction is not in the pool of unspent transactions.";
                send(socketClient, response, strlen(response), 0);
            }
        } else if (choice == "countNextValidator") {
            std::string response = this->countNextValidator();
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "listMempool") {
            std::string response = m_mempool.print();
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "listMempoolLinkedToMe") {
            std::string response = m_mempool.print(m_address);
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "listUnspent") {
            std::string response = m_unspentTransactions.print();
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "listUnspentLinkedToMe") {
            std::string response = m_unspentTransactions.print(m_address);
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "listStakepool") {
            std::string response = m_stakepool.print();
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "listOldStakepool") {
            std::string response = m_oldStakepool.print();
            send(socketClient, response.c_str(), response.size(), 0);
        } else if (choice == "generate") {
            m_blockchain.generateToAddress(m_address, m_unspentTransactions);
        } else if (choice == "printAddress") {
            send(socketClient, m_address.c_str(), m_address.size(), 0);
        } else if (choice == "printTransaction") {
            std::string response = this->printTransaction(buffer, index, bytesReceived);
            send(socketClient, response.c_str(), response.size(), 0);
        } else {
            std::ostringstream oss;
            oss << "Unknown command: \"" << std::string(buffer, bytesReceived - 1) << "\"" << std::endl;
            send(socketClient, oss.str().c_str(), oss.str().size(), 0);
        }
        close(socketClient);
    }
}
