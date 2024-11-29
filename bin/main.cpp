#include "Archive/Argpars.h"
#include "Archive/coding.h"
#include "vector"
#include "iostream"
#include "fstream"

int main(int argc, char** argv) {
    Argpars options;
    options.Parse(argc, argv);
    std::string NameArchive = options.GetArchName();
    uint8_t additional_bits = options.GetAdditionalBits();
    std::vector<std::string> files = options.GetListing();
    if (options.GetFlagCreate()) {
        Create(NameArchive, files, additional_bits);
    }
    if (options.GetFlagList()) {
        ListArchive(NameArchive);
    }
    else if (options.GetAppend()) {
        Append(NameArchive, files, additional_bits);
    } else if (options.GetConcatenate() != std::pair<std::string,std::string>()) {
        ConcatenateArch(NameArchive, options.GetConcatenate().first, options.GetConcatenate().second);
    } else if (options.GetRemove()) {
        DeleteFiles(NameArchive, options.GetListing());
    } else if (options.GetExtract()){
        ExtractFiles(NameArchive, files);
    }
    return 0;
}