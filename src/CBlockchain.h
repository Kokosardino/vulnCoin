#include "CBlock.h"
#include "CTransactionContainer.h"

#pragma once

/**
 * Class representing the blockchain structure.
 * When adding new block, prevBlockHash is automatically enforced to be used.
 */
class CBlockchain
{
public:
    /**
     *  We need to create genesis block. -> Constructor is needed.
     */
	CBlockchain();
    /**
     * Generate a data block with automatically created coinbase transaction to a specified address. Coinbase transaction is saved into received pool "m_transactions".
     * @param address Address of the block creator.
     * @param m_transactions Pool into which the newly created transaction is saved. Should be pool with the usable unspent outputs.
     * @return sha256 hash of the newly created block.
     */
	std::string generateToAddress(const std::string & address, CTransactionContainer & m_transactions);
	/**
	 * Add block with the specified transaction to the blockchain.
	 * @param transactions Vector of transaction IDs to be embedded into the block.
	 */
    void addBlock(const std::vector<std::string> & transactions);
    /**
     * Return printable string representing the blockchain data.
     * @return String representing the blockchain data in JSON formatting.
     */
 	std::string print() const;
    /**
     * @return Number of blocks in the blockchain.
     */
     size_t getBlockCount() const;
     int getTransactionIndex(const std::string &txid) const; 
     /**
     * @return sha256 hash of the last block in the blockchain.
     */
	std::string getLastBlockHash() const;
private:
    //Vector of blocks.
	std::vector<CBlock> m_blockchain; 
};
