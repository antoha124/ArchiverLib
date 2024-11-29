#pragma once
#include <iostream>
#include <vector>

class Argpars {
private:
    std::vector<std::string> files;
    std::string name;
    bool create;
    bool list;
    bool extract;
    bool append;
    bool remove;
    std::pair<std::string, std::string> concatenate;
    uint8_t control_bit;
public:
    Argpars() {
        name = "";
        create = false;
        list = false;
        extract = false;
        append = false;
        remove = false;
        concatenate = std::pair<std::string, std::string>();
        control_bit = 3;
    }
    std::string GetArchName();

    bool GetFlagCreate();

    bool GetExtract();

    bool GetAppend();

    bool GetRemove();

    uint8_t GetAdditionalBits();

    bool GetFlagList();

    std::vector<std::string> GetListing();

    std::pair<std::string, std::string> GetConcatenate();

    void Parse(int argc, char** argv);

    ~Argpars(){};
};
