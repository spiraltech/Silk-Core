// Copyright (c) 2009-2016 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Developers
// Copyright (c) 2015-2016 Silk Network Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */
static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    std::vector<CTxIn> vin;
    vin.resize(1);
    vin[0].scriptSig = CScript() << 1473949500 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    std::vector<CTxOut> vout;
    vout.resize(1);
    vout[0].scriptPubKey = genesisOutputScript;
    vout[0].nValue = 0;
    CMutableTransaction txNew(1, 1473949300, vin, vout, 0);

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();

    return genesis;
}

static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "NY Times Sept 15th 2016: Pardon Edward Snowden";
    const CScript genesisOutputScript = CScript() << ParseHex("") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0, uint256("0x000000251356c62e0aa14c63e2b0db2a05ac0d3316ea5000c797a281be8c9fd7"))
        ( 200, uint256("0x000000297583f342ee3a945bfe4b7c9e30965cbfe97d0573d4a8ad3dfbd59c0c"))
        ( 1000, uint256("0x0000000cb3d1d4589f7f4b6339ca7106c3539146ad307a80232ab09a6f6140f1"))
        ( 4000, uint256("0x00000030b6226239a0d3b809e3b076e3a75476fe8048dc41859a6138f63a7a28"))
        ;
static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1473949500, // * UNIX timestamp of last checkpoint block
        0,          // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        2000        // * estimated number of transactions per day after checkpoint
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, uint256("0x000000d5ac7eb1616789c30baccc4c76a1ea609fa28d3c4774e6686169234510"))
        ;
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1473949500,
        0,
        1000
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0x000003b8d718c47f10afdaa9b59c7d709e0bc8daf79b41dada0c411e7dc9985f"))
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        1473949500,
        0,
        500
    };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x01;
        pchMessageStart[1] = 0x10;
        pchMessageStart[2] = 0x02;
        pchMessageStart[3] = 0x20;
        vAlertPubKey = ParseHex("");
        nDefaultPort = 16662;
        bnProofOfWorkLimit = ~uint256(0) >> 26;
        bnProofOfStakeLimit = ~uint256(0) >> 26;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetSpacingMax = 1 * 64;     // 64 second max spacing target
        nPoWTargetSpacing = 1 * 60;     // 60 seconds PoW Target
        nPoSTargetSpacing = 1 * 64;     // 64 seconds PoS Target
        nCoinbaseMaturity = 10;         // 10 blocks for full confirmation
        nStakeMinAge = 1 * 60 * 60;     // 1 hour minimum stake age
        nStakeMaxAge = std::numeric_limits<int64_t>::max(); // Unlimited stake age
        nModifierInterval = 15 * 60;    // 15 minutes to elapse before new modifier is computed
        nLastPOWBlock = 10000;          // Proof of Work finishes on block 10000

        genesis = CreateGenesisBlock(1473949500, 37239843, bnProofOfWorkLimit.GetCompact(), 1, (0 * COIN));

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x000000251356c62e0aa14c63e2b0db2a05ac0d3316ea5000c797a281be8c9fd7"));
        assert(genesis.hashMerkleRoot == uint256("0x73d6f8c42dfa8c9175b8bf4bf75ebfd10d22b0b6b1a39a82ce0e408447418e4b"));

        vSeeds.push_back(CDNSSeedData("dnsseeder.io", "silk.dnsseeder.io"));
        vSeeds.push_back(CDNSSeedData("dnsseeder.com", "silk.dnsseeder.com"));
        vSeeds.push_back(CDNSSeedData("dnsseeder.host", "silk.dnsseeder.host"));
        vSeeds.push_back(CDNSSeedData("dnsseeder.net", "silk.dnsseeder.net"));
        //vSeeds.push_back(CDNSSeedData("silknetwork.net", "slk1.silknetwork.net"));
        //vSeeds.push_back(CDNSSeedData("silknetwork.net", "slk2.silknetwork.net"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(63);    // Silk public addresses begin with 'S'
        base58Prefixes[SCRIPT_ADDRESS] = list_of(64);    // Silk script (multisig) addresses begin with 'S' 
        base58Prefixes[SECRET_KEY] =     list_of(170);   // Silk private addresses begin with 'S'
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x02;
        pchMessageStart[1] = 0x20;
        pchMessageStart[2] = 0x03;
        pchMessageStart[3] = 0x30;
        vAlertPubKey = ParseHex("");
        nDefaultPort = 16664;
        bnProofOfWorkLimit = ~uint256(0) >> 24;
        bnProofOfStakeLimit = ~uint256(0) >> 24;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nCoinbaseMaturity = 6;          // 6 blocks for full confirmation
        nStakeMinAge = 30 * 60;         // 30 minute minimum stake age
        nModifierInterval = 15 * 60;    // 15 minutes to elapse before new modifier is computed
        nLastPOWBlock = 500;            // Proof of Work finishes on block 500

        genesis = CreateGenesisBlock(1473949500, 47280689, bnProofOfWorkLimit.GetCompact(), 1, (0 * COIN));

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x000000d5ac7eb1616789c30baccc4c76a1ea609fa28d3c4774e6686169234510"));
        assert(genesis.hashMerkleRoot == uint256("0x73d6f8c42dfa8c9175b8bf4bf75ebfd10d22b0b6b1a39a82ce0e408447418e4b"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(63);   // Silk: addresses begin with 'S'
        base58Prefixes[SCRIPT_ADDRESS] = list_of(85);   // Silk: addresses begin with 's'
        base58Prefixes[SECRET_KEY] =     list_of(153);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x40;
        pchMessageStart[1] = 0x04;
        pchMessageStart[2] = 0x50;
        pchMessageStart[3] = 0x05;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nDefaultPort = 16670;
        nMinerThreads = 1;
        bnProofOfWorkLimit = ~uint256(0) >> 22;
        bnProofOfStakeLimit = ~uint256(0) >> 22;
        nLastPOWBlock = 100;    // Proof of Work finishes on block 100

        genesis = CreateGenesisBlock(1473949500, 1427578, 0x1e00ffff, 1, (0 * COIN));

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x000003b8d718c47f10afdaa9b59c7d709e0bc8daf79b41dada0c411e7dc9985f"));
        assert(genesis.hashMerkleRoot == uint256("0x73d6f8c42dfa8c9175b8bf4bf75ebfd10d22b0b6b1a39a82ce0e408447418e4b"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 6665;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
