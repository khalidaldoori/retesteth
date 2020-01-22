#pragma once
#include "TestBlockchain.h"
#include <ethObjects/common.h>
#include <retesteth/RPCSession.h>
#include <string>
#include <vector>

// TestBlockchain class with chain reorgs
class TestBlockchainManager
{
public:
    TestBlockchainManager(RPCSession& _session, std::string const& _network)
      : m_session(_session), m_defaultNetwork(_network)
    {
        m_wasAtLeastOneFork = false;
    }

    // Perform block generation logic by parsing the filler file section
    void parseBlockFromJson(blockSection const& _block, bool _generateUncles);

    // Return the last block from the current chain (not the top of all chains)
    TestBlock const& getLastBlock()
    {
        TestBlockchain const& chain = mapOfKnownChain.at(m_sCurrentChainName);
        assert(chain.getBlocks().size() > 0);
        return chain.getBlocks().at(chain.getBlocks().size() - 1);
    }

    // Add a generated block from the tests block array
    void registerTestBlockRLP(std::string const& _rlp) { m_testBlockRLPs.push_back(_rlp); }

    // Import all generated blocks at the same order as they are in tests
    void syncOnRemoteClient() const
    {
        ETH_LOGC("IMPORT KNOWN BLOCKS ", 6, LogColor::LIME);
        if (m_wasAtLeastOneFork)
        {
            m_session.test_rewindToBlock(0);
            for (auto const& rlp : m_testBlockRLPs)
                m_session.test_importRawBlock(rlp);
        }
    }

    // When mining the first block, set the current chain name
    void attemptToSetCurrentChainName(std::string const& _defaultChainName)
    {
        if (m_sCurrentChainName.empty())
            m_sCurrentChainName = _defaultChainName;
    }

private:
    // Reorg chains on the client if needed (for new block generation)
    void reorgChains(std::string const& _newBlockChainName, size_t _newBLockNumber);

    // Prepare uncles
    vectorOfSchemeBlock prepareUncles(blockSection const& _block, string const& _debug);

    // Parse uncle section in block and generate uncles from all the chain information
    test::scheme_block prepareUncle(
        scheme_uncleHeader _uncleOverwrite, vectorOfSchemeBlock const& _currentBlockPreparedUncles);

    RPCSession& m_session;
    std::string m_defaultNetwork;                           // Chain network (fork)
    std::string m_sCurrentChainName;                        // Chain name that is mining blocks
    bool m_wasAtLeastOneFork;                               // Wether we swithced chains on remote
    std::vector<std::string> m_testBlockRLPs;  // List of prepared blocks as they are in test
    std::map<std::string, TestBlockchain> mapOfKnownChain;  // Memory of test chains
};