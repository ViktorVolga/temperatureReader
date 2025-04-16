#include "../headers/RomFinder.hpp"
#include "../headers/logger.hpp"

CandidateToRom::CandidateToRom(uint64_t rom, int lastBit)
{
    m_rom = rom;
    m_lastBit = lastBit;
}

RomFinder::RomFinder(DS2482 * ds2482)
{
    if(!ds2482)
        SHTLogger()->error("[RomFinder] ds2482 is nullptr");

    m_ds2482 = ds2482;
    SHTLogger()->debug("[RomFinder] RomFinder() - ok");
}

/*
    return next bit what should send to ds2482 to find new ROM
    if no potencial roms return 1 
*/
bool RomFinder::getNextBit(int step, int round)
{
    if(round && round <= m_romsForVerification.size() && step <= m_romsForVerification[round - 1].size()){
        if(m_romsForVerification[round - 1][step] == '0')
            return false;
    }
    return true;
}

void RomFinder::resolveAnswer(ssize_t answer)
{
    bool romAlreadyFounded = false;
    if((!(answer & 0x01)) && (!(answer & 0x02))){
        /*here we know what we have 2 devise with current base*/
        for(const auto & rom : m_foundedRoms){
            if(rom.find(m_currentRom + '0') == 0)
                romAlreadyFounded = true;
        }
        for(const auto & rom : m_romsForVerification){
            if(rom.find(m_currentRom + '0') == 0)
                romAlreadyFounded = true;
        }
        if(!romAlreadyFounded && (m_currentStep + 1) < 64)
            m_romsForVerification.push_back(m_currentRom + '0');
    }

    if (answer & 4)
        m_currentRom += '1';
    else
        m_currentRom += '0';
}

void RomFinder::findRoms()
{
    while(true){
        resolveAnswer(m_ds2482->W1Triplet(getNextBit(m_currentStep, m_currentRound)));
        m_currentStep++;
        if(m_currentStep >= 63){
            SHTLogger()->info("Founded rom {}", m_currentRom);
            m_currentRound++;
            m_foundedRoms.insert(m_currentRom);
            m_currentStep = 0;
            m_currentRom = "";
            if(m_currentRound > m_romsForVerification.size())
                break;
        }
    }    
}
