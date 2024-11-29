#include "iostream"
#include "coding.h"

int kCharToBit = 8;

void Create(const std::string& arch, std::vector<std::string>& files, uint8_t kDopBits){
    std::ofstream archive("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + arch + ".haf");
    archive.close();
    Append(arch, files, kDopBits);
}

void Append(const std::string& name, std::vector<std::string>& files, uint8_t kControlBits){
    std::ofstream output;
    output.open("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + name + ".haf", std::ios::app);
    for (size_t i = 0; i < files.size(); ++i) {
        EncodeToFile(output, files[i], kControlBits);
    }
    output.close();
}

void ListArchive(std::string name){
    std::ifstream input;
    input.open("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + name + ".haf");
    if (!input.is_open()){
        std::cerr << "File dont open!\n";
    }
    while (input){
        NameArchive res = DecodeHead(input);
        std::cout << res.name << "\n";
        input.seekg(res.size, std::ios::cur);
    }
    input.close();
}

void ConcatenateArch(const std::string& archive, const std::string& name_first, const std::string& name_second){
    std::ifstream input_first(
            "/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + name_first + ".haf");
    std::ofstream output("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + archive + ".haf");
    char ch;
    while (input_first.get(ch)) {
        output << ch;
    }
    std::ifstream input_second(
            "/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + name_second + ".haf");
    while (input_second.get(ch)) {
        output << ch;
    }
    output.close();
}

bool NumbersofDopBits(std::vector<bool>& bits, uint8_t index, uint64_t step){
    bool result = false;
    for (size_t i = index; i < bits.size(); i += (1 << step)) {
        for (size_t j = i; (j < i + (1 << (step - 1))) && j < bits.size(); ++j) {
            result ^= bits[j];
        }
    }
    return result;
}

void EncodingNumber(std::vector<bool>& bits, uint8_t kControlBits) {
    bool partial_bit = false;
    for (size_t i = 0; i < kControlBits; ++i) {
        bits.insert(bits.begin() + (1 << i) - 1, 0);
    }
    for (size_t i = 0; i < kControlBits; ++i) {
        bits[(1 << i) - 1] = NumbersofDopBits(bits, (1 << i) - 1, i + 1);
    }
    for (size_t i = 0; i < bits.size(); i++) {
        partial_bit ^= bits[i];
    }
    bits.push_back(partial_bit);
}

void WriteFile(std::ofstream& output, std::vector<bool>& bits){
    uint8_t res = 0;
    for (int i = 0; i < bits.size() / kCharToBit; ++i){
        res = 0;
        for (int j = 0; j < kCharToBit; ++j){
            res |= (bits[i * kCharToBit + j] << j);
        }
        output << res;
    }
    bits.clear();
}

void EncodeToFile(std::ofstream& output, const std::string& NameArchive, int kDopBits) {
    std::ifstream input("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + NameArchive, std::ios::binary);
    uint8_t block = (1 << kDopBits) - kDopBits - 1;;
    size_t file_size = std::__fs::filesystem::file_size(("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + NameArchive));
    EncodeHead(NameArchive, output, file_size, kDopBits);
    std::vector<bool> bits;
    char ch;
    while (input.get(ch)){
        for (int i = 0; i < kCharToBit; ++i){
            bits.push_back((ch >> i) & 1);
            if (bits.size() == block){
                EncodingNumber(bits, kDopBits);
                WriteFile(output, bits);
            }
        }
    }
    input.close();
}

void ExtractFiles(std::string& name_archive, std::vector<std::string>& files){
    for (size_t i = 0; i < files.size(); ++i){
        Extract(name_archive, files[i]);
    }
}

void Extract(std::string& name_archive, std::string file){
    std::ifstream input;
    input.open("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + name_archive + ".haf", std::ios::binary);
    std::ofstream output("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/clone_" + file);
    if (!input.is_open()){
        std::cerr << "File dont open!\n";
    }
    char temp;
    std::vector<bool> bits;
    while (input){
        NameArchive res = DecodeHead(input);
        if (file == res.name) {
            for (int i = 0; i < res.size / 2; ++i) {
                uint8_t temp_char_name = 0;
                for (int k = 0; k < 2; ++k) {
                    input >> temp;
                    for (int j = 0; j < kCharToBit; ++j) {
                        bits.push_back((temp >> j) & 1);
                    }
                    Decode(bits, 3);
                    for (int j = 0; j < kCharToBit; ++j) {
                        temp_char_name |= (bits[j] << (j + 4 * (k)));
                    }
                    bits.clear();
                }
                output << static_cast<char>(temp_char_name);
            }
        } else {
            input.seekg(res.size, std::ios::cur);
        }
    }
    input.close();
    output.close();
}

void Delete(const std::string& archive, const std::string& try_del){
    std::ifstream input;
    input.open("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + archive + ".haf", std::ios::binary);
    std::ofstream output("/Users/hom1c1d3/github-classroom/is-itmo-c-23/labwork6-antoha124/test/" + archive + "_clone" + ".haf");
    if (!input.is_open()){
        std::cerr << "File dont open!\n";
    }
    char temp;
    while (input){
        NameArchive res = DecodeHead(input);
        if (try_del == res.name) {
            input.seekg(res.size, std::ios::cur);
        } else {
            EncodeHead(res.name, output, res.size, res.kControlBits);
            for (int i = 0; i < res.size; ++i) {
                input >> temp;
                output << temp;
            }
        }
    }
    input.close();
    output.close();

}

void DeleteFiles(const std::string& archive, const std::vector<std::string>& files){
    for (int i = 0; i < files.size(); ++i){
        Delete(archive, files[i]);
    }
}

void EncodeHead(const std::string& NameArchive, std::ofstream& output, size_t file_size, uint8_t kDopBits) {
    std::vector<bool> bits;
    size_t len_blocks_name = (NameArchive.size() * kCharToBit) / 4;
    for (int i = 0; i < 11; ++i) {
        bits.push_back((len_blocks_name >> i) & 1);
    }
    EncodingNumber(bits, 4);
    WriteFile(output, bits);
    for (int i = 0; i < NameArchive.size(); ++i) {
        for (int j = 0; j < kCharToBit; ++j) {
            bits.push_back((NameArchive[i] >> j) & 1);
            if (bits.size() == 4) {
                EncodingNumber(bits, 3);
                WriteFile(output, bits);
            }
        }
    }
    for (int i = 0; i < 4; ++i) {
        bits.push_back((kDopBits >> i) & 1);
    }
    EncodingNumber(bits, 3);
    WriteFile(output, bits);

    size_t len_blocks_size = std::ceil(file_size / 4);
    for (int i = 0; i < 26; ++i) {
        bits.push_back((len_blocks_size >> i) & 1);
    }
    EncodingNumber(bits, 5);
    WriteFile(output, bits);
    for (int i = 0; i < len_blocks_size; ++i) {
        for (int j = 0; j < kCharToBit; ++j) {
            bits.push_back((file_size >> j) & 1);
            if (bits.size() == 4) {
                EncodingNumber(bits, 3);
                WriteFile(output, bits);
            }
        }
    }
}

void Decode(std::vector<bool>& bits, uint8_t kControlBits) {
    std::vector<bool> error_bit;
    bool double_error_bit = bits.back();
    bits.pop_back();
    for (size_t i = 0; i < kControlBits; ++i) {
        error_bit.push_back(
                NumbersofDopBits(bits, (1 << i) - 1, i + 1));
    }
    uint32_t error_number = 0;
    for (int32_t i = 0; i < error_bit.size() - 1; ++i) {
        error_number |= (error_bit[i] << i);
    }
    if (!double_error_bit && error_number != 0) {
        std::cerr << "Error" << std::endl;
    }
    if (error_number != 0) {
        bits[error_number - 1] = (bits[error_number - 1] + 1) % 2;
    }
    std::vector<bool> result;
    uint8_t current_pow = 1;
    for (size_t i = 0; i < bits.size(); i++) {
        if (i == current_pow - 1) {
            current_pow *= 2;
            continue;
        }
        result.push_back(bits[i]);
    }
    bits = result;
}

NameArchive DecodeHead(std::ifstream& archive){
    uint64_t len_size_text_bit = 0;
    uint64_t size_text_bit = 0;
    uint64_t size_name = 0;
    std::string full_name = "";
    uint8_t temp_char_first, temp_char_second;
    std::vector<bool> bits;
    archive >> temp_char_first >> temp_char_second;
    for (int j = 0; j < kCharToBit; ++j) {
        bits.push_back((temp_char_first >> j) & 1);
    }
    for (int j = 0; j < kCharToBit; ++j) {
        bits.push_back((temp_char_second >> j) & 1);
    }
    Decode(bits, 4);
    for (uint64_t i = 0; i < 11; ++i){
        size_name |= (bits[i] << i);
    }
    bits.clear();
    // decode size name
    for (uint64_t i = 0; i < size_name / 2; ++i){
        archive >> temp_char_first >> temp_char_second;
        for (int j = 0; j < kCharToBit; ++j) {
            bits.push_back((temp_char_first >> j) & 1);
        }
        Decode(bits, 3);
        uint8_t temp_char_name = 0;
        for (uint64_t j = 0; j < 4; ++j){
            temp_char_name |= (bits[j] << j);
        }
        bits.clear();
        for (int j = 0; j < kCharToBit; ++j) {
            bits.push_back((temp_char_second >> j) & 1);
        }
        Decode(bits, 3);
        for (uint64_t j = 0; j < 4; ++j){
            temp_char_name |= (bits[j] << (j + 4));
        }
        bits.clear();
        full_name += static_cast<char>(temp_char_name);
    }
    // decode name
    uint8_t kDopBit = 0;
    archive >> temp_char_first;
    for (int j = 0; j < kCharToBit; ++j) {
        bits.push_back((temp_char_first >> j) & 1);
    }
    Decode(bits, 3);
    for (uint64_t j = 0; j < 4; ++j){
        kDopBit |= (bits[j] << j);
    }
    bits.clear();
    // decode dop bit
    uint8_t temp_char_third, temp_char_fourth;
    archive >> temp_char_first >> temp_char_second >> temp_char_third >> temp_char_fourth;
    for (int j = 0; j < kCharToBit; ++j) {
        bits.push_back((temp_char_first >> j) & 1);
    }
    for (int j = 0; j < kCharToBit; ++j) {
        bits.push_back((temp_char_second >> j) & 1);
    }
    for (int j = 0; j < kCharToBit; ++j) {
        bits.push_back((temp_char_third >> j) & 1);
    }for (int j = 0; j < kCharToBit; ++j) {
        bits.push_back((temp_char_fourth >> j) & 1);
    }
    Decode(bits, 5);
    for (uint64_t i = 0; i < 26; ++i){
        len_size_text_bit |= (bits[i] << i);
    }
    bits.clear();
    // decode length of size text
    for (uint64_t i = 0; i < len_size_text_bit; ++i){
        archive >> temp_char_first >> temp_char_second;
        for (int j = 0; j < kCharToBit; ++j) {
            bits.push_back((temp_char_first >> j) & 1);
        }
        Decode(bits, 3);
        uint8_t temp_char_size = 0;
        for (uint64_t j = 0; j < 4; ++j){
            temp_char_size |= (bits[j] << j);
        }
        bits.clear();
        for (int j = 0; j < kCharToBit; ++j) {
            bits.push_back((temp_char_second >> j) & 1);
        }
        Decode(bits, 3);
        for (uint64_t j = 0; j < 4; ++j){
            temp_char_size |= (bits[j] << (j + 4));
        }
        bits.clear();
        size_text_bit += temp_char_size;
    }
    NameArchive res = NameArchive(full_name, size_text_bit * 2 / 17, kDopBit);
    return res;

}


std::vector<bool> NumberToVectorBool(uint64_t num){
    std::vector<bool> res;

}