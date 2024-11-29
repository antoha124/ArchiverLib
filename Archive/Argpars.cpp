#include "Argpars.h"


void Argpars::Parse(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string point = argv[i];
        if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--create")) {
            create = true;
        } else if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--list")) {
            list = true;
        } else if (!strcmp(argv[i], "-x") || !strcmp(argv[i], "--extract")) {
            extract = true;
        } else if ((!strcmp(argv[i], "-a") && i < argc) ||  (!strcmp(argv[i], "--append") && i < argc)) {
            append = true;
        } else if (!strcmp(argv[i], "-d") || (!strcmp(argv[i], "--delete") && i < argc)) {
            remove = true;

        } else if (!strcmp(argv[i], "-A") || !strcmp(argv[i],  "--concatenate")) {
            concatenate.first = argv[i + 1];
            concatenate.second = argv[i + 2];
            i += 2;
        } else if (!strcmp(argv[i], "-f") || !strncmp(argv[i], "--file", 6)){
            if (!strcmp(argv[i], "-f")){
                name = argv[i + 1];
                ++i;
            } else {
                std::string arg = argv[i];
                name = arg.substr(arg.find('=') + 1);
            }
        } else if (!strcmp(argv[i], "-b") || !strncmp(argv[i], "--bits", 6)){
            if (!strcmp(argv[i], "-b")) {
                control_bit = std::stoi(argv[i + 1]);
                ++i;
            } else{
                control_bit = std::stoi((argv[i] + 7, argv[i] + strlen(argv[i])));
            }
        } else{
            files.push_back(argv[i]);
        }
    }

}

std::string Argpars::GetArchName() { return name; }

bool Argpars::GetFlagCreate() { return create; }

bool Argpars::GetExtract() { return extract; }

bool Argpars::GetAppend() { return append; }

bool Argpars::GetRemove() { return remove; }

uint8_t Argpars::GetAdditionalBits() { return control_bit; }

bool Argpars::GetFlagList() { return list; }

std::vector<std::string> Argpars::GetListing() { return files; }

std::pair<std::string, std::string> Argpars::GetConcatenate() {
    return concatenate;
}

