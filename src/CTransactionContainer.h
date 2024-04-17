#include <map>
#include <vector>

#include "CTransaction.h"

#pragma once

/**
 * Container to hold CTransaction objects, mainly used for mempools, stakepools etc.
 */
class CTransactionContainer {
public:
    /**
     * Default constructor.
     */
    CTransactionContainer() = default;
    /**
     * Return CTransaction object based on the txid. It always needs to be checked whether the specified CTransaction object exists in the container before calling this function.
     * @param txid Transaction ID of the returned transaction.
     * @return Desired transaction or m_container.end() (if called incorrectly).
     */
    CTransaction getTransactionByTxid(const std::string txid);
    /**
     * Add new CTransaction object into the container.
     * @param transaction CTransaction object to save.
     */
    void addTransaction(const CTransaction transaction);
    /**
     * Print contents of the container.
     * @return JSON formatted string fit for printing.
     */
    std::string print() const;
    /**
     * Print all contents of the container tied to a specified address.
     * @param address Address of the receiver (holder) to check transactions against.
     * @return JSON formatted string fit for printing.
     */
    std::string print(const std::string &address) const;
    /**
     * Print a transaction specified by txid.
     * @param txid ID of the searched transaction.
     * @return JSON formatted string fit for printing.
     */
    std::string printTransaction(const std::string &txid) const;
    /**
     * Function working as a check whether transaction exist in a mempool.
     * @param txid ID of the searched transaction.
     * @param address Address of the holder of searched transaction.
     * @return Number of times the transaction has been passed if found. Otherwise returns [-1].
     */
    int returnPassed(const std::string &txid, const std::string &address);
    /**
     * Checks existence of all transactions in the received vector of txids against the container.
     * @param transactions Vector of transaction IDs to check existence of.
     * @return true if all transaction IDs are in the container. Otherwise false.
     */
    bool findTransactions(const std::vector <std::string> &transactions) const;
    /**
     * Remove a transaction specified by txid from the container. Check whether transaction exists is implemented.
     * @param txid Transaction ID of the to-be-removed transaction.
     */
    void removeTransactionByTxid(const std::string &txid);
    /**
     * @return Size of the container.
     */
    size_t size() const;
    /**
     * Dumb and ineffective implementation of [] operator (same as for std::vector), we use it only in one very specific scenario.
     * @param index Index of the searched transaction.
     * @return Transaction on the specified index.
     */
    CTransaction operator[](size_t index);

private:
    //Transactions sorted by txid.
    std::map <std::string, CTransaction> m_container;
};
