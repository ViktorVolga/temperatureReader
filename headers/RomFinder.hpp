#pragma once

#include <set>
#include <memory>
#include <vector>

#include "ds2482.hpp"

struct CandidateToRom{
    uint64_t m_rom = 0;
    int m_lastBit = 0;
    CandidateToRom(uint64_t rom, int lastBit);
};

/*class for looking roms on w1 buss*/
class RomFinder{
    /*storage for founded roms*/
    std::set <std::string> m_roms;
    std::string m_currentRom;
    int m_currentStep = 0;
    DS2482 * m_ds2482 = nullptr;
    int m_currentRound = 0;
    std::vector<std::string> m_romsForVerification;
    std::set <std::string> m_foundedRoms;
public:
    RomFinder(DS2482 * ds2482);
    RomFinder(const RomFinder & other) = delete;
    /*returns next bit -what should be send to W1 buss */
    bool getNextBit(int step, int round);
    void resolveAnswer(ssize_t answer);
    void findRoms();
};