#pragma once

#include <set>
#include <memory>
#include <vector>

#include "ds2482.hpp"

struct CandidateToRom{
    uint64_t m_rom = 0;
    int m_lastBit = 0;
    CandidateToRom(uint64_t rom, uint8_t lastBit);
};

/*class for looking roms on w1 buss*/
class RomFinder
{
    /*storage for founded roms*/
    std::vector <uint64_t> m_roms;
    std::string m_currentRom{};
    int m_currentStep = 0;
    std::weak_ptr<iDS2482> m_ds2482;
    int m_currentRound = 0;
    std::vector<std::pair<uint64_t, int>> m_romsForVerification;
    std::set <uint64_t> m_foundedRoms;
public:
    RomFinder(std::shared_ptr<iDS2482> ds2482);
    RomFinder(const RomFinder & other) = delete;
    /*returns next bit -what should be send to W1 buss */
    bool getNextBit(int step, int round);
    void resolveAnswer(ssize_t answer);
    void findRoms();
    bool isEqual(uint64_t alredyFoundedRom, uint64_t candidateToRom, int step);
    int getBit(uint64_t rom, int step);
    void setBit(uint64_t& rom, int step);
};