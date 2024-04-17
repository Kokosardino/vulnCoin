#include <iostream>

#include "CCryptography.h"

#pragma once

/**
 * Class representing one transaction.
 * -> Transactions are for the purposes of simulation tied to the receiver addresses.
 * -> Transactions should be created with the minimum of one second passing within their creation to prevent problems with duplicit txids.
 * -> If duplicit transactions are created, network defaults to undefined behavior while working them.
 * -> For simplification purposes all transactions have default amount of 10 (value).
 */
class CTransaction {
public:
    /**
     * Create a transaction with current timestamp.
     * @param address Address of the reciever (holder) of the transaction output.
     * @param ammount Value of the transaction output.
     * @param passed Number of how many times the transaction output was passed.
     * @param coinbase Specify whether the transaction output is coinbase or not.
     */
    CTransaction(const std::string address, const int ammount, const int passed, const bool coinbase);
    /**
     * Create a transaction with predefined timestamp.
     * @param address Address of the reciever (holder) of the transaction output.
     * @param ammount Value of the transaction output.
     * @param passed Number of how many times the transaction output was passed.
     * @param coinbase Specify whether the transaction output is coinbase or not.
     * @param timestamp Must be a valid UNIX timestamp!
     */
    CTransaction(const std::string address, const int ammount, const int passed, const bool coinbase,
                 const time_t timestamp);
    /**
     * Return the information about the transaction in printable format.
     * @return JSON formatted string.
     */
    std::string print() const;

    std::string m_address, m_txid;
    int m_ammount, m_passed;
    bool m_coinbase;
    std::time_t m_timestamp;
};
