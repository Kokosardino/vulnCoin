#include <vector>

#include "CCryptography.h"
#include "CTransaction.h"

#pragma once

/**
 * Class representing a single data block within the blockchain structure.
 */
class CBlock {
public:
    /**
     * Standard constructor of a new block.
     * @param prevBlockHash Hash of the previous block.
     * @param transactionsHash Hash of concatenated transactions IDs.
     * @param transactions IDs of all transactions validated by the block.
     */
    CBlock(const std::string prevBlockHash, const std::string transactionsHash,
           const std::vector <std::string> transactions);

    /**
     * Function to return the string with information about the block that can further be printed.
     * @return Block data encoded in a JSON format.
     */
    std::string print() const;

    /**
     * Function to get hash of the block.
     * @return String representing the sha256 hash of the block.
     */
    std::string getBlockHash() const;
    bool containsTransaction(const std::string &txid) const;
private:
    //sha256 hash of the previous block.
    std::string m_prevBlockHash;
    //sha256 hash of concatenated transaction IDs embedded into the block.
    std::string m_transactionsHash;
    //Vector of txids of transactions embedded into the block.
    std::vector <std::string> m_transactions;
};
