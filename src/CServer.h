#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "CBlockchain.h"

#pragma once

/**
 * Class representing a server.
 */
class CServer {
public:
    /**
     * Default constructor.
     */
    CServer(int coinAge);
    /**
     * Function that starts server on a specified port.
     * @param port Port to start the server on.
     * @return true in case the server has correctly started. false otherwise.
     */
    int bindSocket(const int port);
    /**
    * Very simplistic function to parse a string from buffer. Fragile to deviations in the expected format, application can segfault in case the inputs are not formatted correctly.
    * @param buffer Buffer of received command.
    * @param index Current index in the buffer (function can be used to read multiple words by repeated calls).
    * @param bytesReceived Number of valid chars in the buffer.
    * @return Parsed string.
    */
    std::string parseString(const char * buffer, size_t & index, const size_t bytesReceived);
    /**
     * Parse array of string in format {<string_1> <string_2> ... <string_n>}, other formats lead to undefined behavior.
     * @param buffer Buffer of received command.
     * @param index Current index in the buffer (function can be used to read multiple words by repeated calls).
     * @param bytesReceived Number of valid chars in the buffer.
     * @return Vector of parsed strings.
     */
    std::vector <std::string> parseTransactionArray(const char * buffer, size_t index, const size_t bytesReceived);
    /**
     * Create a new transaction specified in the format "createNewTransaction <txid of UTXO used> <addressReceiver>".
     * @param buffer Buffer of received command.
     * @param index Current index in the buffer.
     * @param bytesReceived Number of valid chars in the buffer.
     * @return Transaction ID of the newly created transaction.
     */
    std::string createNewTransaction(const char * buffer, size_t & index, const size_t bytesReceived);
    /**
     * Load a new transaction created on different server specified in the format "loadTransaction <txid of UTXO used> <addressOfSender> <addressOfReceiver> <timestampOfTheNewUTXO>".
     * @param buffer Buffer of received command.
     * @param index Current index in the buffer.
     * @param bytesReceived Number of valid chars in the buffer.
     * @return true if transaction can be loaded (UTXO used to create it exists). Otherwise false.
     */
    bool loadTransaction(const char * buffer, size_t & index, const size_t bytesReceived);
    /**
     * Load a new coinbase transaction created on different server specified in the format "loadCoinbaseTransaction <addressOfReceiver> <timestampOfTheNewUTXO>".
     * @param buffer Buffer of received command.
     * @param index Current index in the buffer.
     * @param bytesReceived Number of valid chars in the buffer.
     * @return txid of the created transaction (creating cannot fail).
     */
    std::string loadCoinbaseTransaction(const char * buffer, size_t & index, const size_t bytesReceived);
    /**
     * Create a new block with transactions specified by txids as "proposeBlock {<txid_1> <txid_2> ... <txid_n>}". All txids have to be in the mempool and are upon the block creation moved to unspentTransactions pool.
     * @param buffer Buffer of received command.
     * @param index Current index in the buffer.
     * @param bytesReceived Number of valid chars in the buffer.
     * @return true if the block is created (all specified transactions exist). Otherwise false.
     */
    bool proposeBlock(const char * buffer, size_t & index, const size_t bytesReceived);
    /**
     * Stake a transaction to participate in the block creation process with command specified as "stake <txid> <addressSender>".
     * @param buffer Buffer of received command.
     * @param index Current index in the buffer.
     * @param bytesReceived Number of valid chars in the buffer.
     * @return true if the specified transaction can be staked. false otherwise.
     */
    bool stake(const char *buffer, size_t &index, const size_t bytesReceived);
    /**
     * Count the index of the next block validator.
     * @return The index of the transaction owner of which is chosen as creator of the next block.
     */
    std::string countNextValidator();
    std::string countNextValidatorCoinAge();
    /**
     * Print transaction specified by transaction ID in the format "printTransaction <txid>".
     * @param buffer Buffer of received command.
     * @param index Current index in the buffer.
     * @param bytesReceived Number of valid chars in the buffer.
     * @return Transaction information formatted as JSON.
     */
    std::string printTransaction(const char *buffer, size_t &index, const size_t bytesReceived);
    std::string printStakesCoinAge(const CTransactionContainer &stakepool);
    /**
     * Start the server. Can be stopped simply by command "stop".
     */
    void run();

private:
    //Socket the server connects to.
    int m_coinAge, m_socket;
    //Information about server.
    sockaddr_in m_serverInfo;
    //Blockchain the server holds. -> Is lost when the server is stopped.
    CBlockchain m_blockchain;
    //Create the pools to store transaction in:
    //                                          unspentTransactions         -> spendable UTXOs.
    //                                          mempool                     -> UTXOs to be embedded into a block.
    //                                          stakePool & oldStakepool    -> pools staked UTXOs are saved into.
    CTransactionContainer m_unspentTransactions, m_mempool, m_stakepool, m_oldStakepool;
    //Generated address of the node running the server.
    std::string m_address;
};
