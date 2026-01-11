#include "../headers/RomFinder.hpp"
#include "../headers/logger.hpp"
#include "../headers/w1.hpp"

CandidateToRom::CandidateToRom(uint64_t rom, uint8_t lastBit)
{
    m_rom = rom;
    m_lastBit = lastBit;
}

RomFinder::RomFinder(std::shared_ptr<iDS2482> ds2482)
{
    if(ds2482)
    {
        m_ds2482 = ds2482;
        SHTLogger()->debug("[RomFinder] RomFinder() - ok");
    }
    else
    {
        SHTLogger()->error("[RomFinder] RomFinder() - ds2482");
    }
}

/*
    return next bit what should send to ds2482 to find new ROM
    if no potencial roms return 1 
*/
bool RomFinder::getNextBit(int step, int round)
{
    if(round && round <= m_romsForVerification.size() && step <= m_romsForVerification[round - 1].second)
    {
        if(getBit(m_romsForVerification[round - 1].first, step) == 0)
        {
            return false;
        }
    }
    SHTLogger()->info("No potential roms - return true");
    return true;
}

void RomFinder::resolveAnswer(ssize_t answer)
{
    bool romAlreadyFounded = false;
    SHTLogger()->info("answer {}", answer);
    if((!(answer & 0x01)) && (!(answer & 0x02))){
        /*here we know what we have 2 devise with current base*/
        for(const auto & rom : m_foundedRoms)
        {
//            if(rom.find(m_currentRom + '0') == 0)
//                romAlreadyFounded = true;
            if(isEqual(rom, m_currentRom, m_currentStep))
            {
                romAlreadyFounded = true;
            }
        }
        for(const auto & rom : m_romsForVerification)
        {
//            if(rom.find(m_currentRom + '0') == 0)
//            {
//                romAlreadyFounded = true;
//            }
            if(isEqual(m_currentRom, rom.first, m_currentStep))
            {
                romAlreadyFounded = true;
            }
        }
        if(!romAlreadyFounded && (m_currentStep + 1) < 64)
        {
//            m_romsForVerification.push_back(m_currentRom + '0');
            SHTLogger()->info("romAlreadyFounded -false, m_currentStep {}", m_currentStep);
            m_romsForVerification.push_back(std::make_pair(m_currentRom, m_currentStep));
        }
    }

    if (answer & 4)
    {
        setBit(m_currentRom, m_currentStep);
    }
}

void RomFinder::findRoms()
{
    if (auto ds2482 = m_ds2482.lock())
    {
        ds2482->W1WriteByte(static_cast<uint8_t>(W1Commands::matchRom));
    }
    while(true)
    {
        if (auto ds2482 = m_ds2482.lock())
        {
            /*
                to do later - may be here need nore atomic lock
                and not create temporary wariable
            */
            resolveAnswer(ds2482->W1Triplet(getNextBit(m_currentStep, m_currentRound)));
        }
        m_currentStep++;
        if(m_currentStep >= 63)
        {
            SHTLogger()->info("Founded rom {}", m_currentRom);
            m_currentRound++;
            m_foundedRoms.insert(m_currentRom);
            m_currentStep = 0;
            m_currentRom = 0;
            if(m_currentRound > m_romsForVerification.size())
            {
                break;
            }
            if (auto ds2482 = m_ds2482.lock())
            {
                ds2482->W1ResetBus();
                ds2482->W1WriteByte(static_cast<uint8_t>(W1Commands::matchRom));
            }
        }
    }
}

bool RomFinder::isEqual(uint64_t alredyFoundedRom, uint64_t candidateToRom, int step)
{
    alredyFoundedRom = alredyFoundedRom >> (63 - step);
    candidateToRom = candidateToRom >> (63 - step);
    return alredyFoundedRom == candidateToRom;
}

int RomFinder::getBit(uint64_t rom, int step)
{
    if (step < 0 || step >= 63)
    {
        SHTLogger()->error("RomFinder::getBit() - wrong step {}", step);
    }
    if(((1ULL << (63 - step)) & rom))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void RomFinder::setBit(uint64_t& rom, int step)
{
    if (step < 0 || step >= 63)
    {
        SHTLogger()->error("RomFinder::setBit() - wrong step {}", step);
        return;
    }
    rom |= (1ULL << (63 -step));
    SHTLogger()->info("rom after set bit {}", rom);
}
