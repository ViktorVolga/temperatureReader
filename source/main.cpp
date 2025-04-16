#include <iostream>

#include "../headers/ds2482.hpp"
#include "../headers/RomFinder.hpp"
#include "../headers/w1.hpp"

int main(){
    std::unique_ptr<DS2482> ds2482 = std::make_unique<DS2482>();
    std::unique_ptr<RomFinder> romFinder = std::make_unique<RomFinder>(ds2482.get());
    w1Buss w1 = w1Buss();
    std::string s = "";
    w1.readTemperature(s);
    return 0;
}