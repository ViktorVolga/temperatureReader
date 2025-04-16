#pragma once

#include <map>
#include <string>

class TReaderInterface{

};

class w1TReader : TReaderInterface{
    std::map<std::string /*ROM*/,
            float /*temperature*/> sensorsMap;
public:
};