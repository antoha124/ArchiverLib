#include "iostream"
#include "fstream"
#include "math.h"


void Create(const std::string& arch, std::vector<std::string>& files, uint8_t kDopBits);
//-c --create
void Append(const std::string& name, std::vector<std::string>& files, uint8_t kControlBits);
//-a --append
void ListArchive(std::string name);
//-l --list
void ConcatenateArch(const std::string& archive, const std::string& name_first, const std::string& name_second);
//-A --concatenate
bool NumbersofDopBits(std::vector<bool>& bits, uint8_t index, uint64_t step);

void WriteFile(std::ofstream& output, std::vector<bool>& bits);

void EncodingNumber(std::vector<bool>& bits, uint8_t kControlBits);

void EncodeToFile(std::ofstream& output, const std::string& NameArchive, int kDopbits);

void ExtractFiles(std::string& name_archive, std::vector<std::string>& files);

void Extract(std::string& name_archive, std::string file);
//-x --extract
void Delete(const std::string& archive, const std::string& try_del);
//-d
void DeleteFiles(const std::string& archive, const std::vector<std::string>& files);

void EncodeHead(const std::string& NameArchive, std::ofstream& output, size_t file_size, uint8_t kDopBits);

void Decode(std::vector<bool>& bits, uint8_t kControlBits);

struct NameArchive {
    std::string name;
    uintmax_t size;
    uint8_t kControlBits;

    NameArchive() {
        name = "";
        size =  0;
        kControlBits = 3;
    }

    NameArchive(std::string new_name, uint64_t new_size, uint8_t new_kControlBits){
        name = new_name;
        size = new_size;
        kControlBits = new_kControlBits;
    }

    ~NameArchive() {}
};

NameArchive DecodeHead(std::ifstream& archive);