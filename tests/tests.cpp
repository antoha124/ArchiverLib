#include <Archive/Argpars.h>
#include <Archive/coding.h>
#include <gtest/gtest.h>

std::vector<bool> SetBits(size_t number, uint8_t additional_bits, uint8_t for_encode) {
    uint64_t lenght = ((1 << additional_bits) - for_encode * additional_bits - 1);
    std::vector<bool> result(lenght , 0);

    for (uint64_t i = 0; i < lenght; ++i) {
        result[i] = ((number & (1 << i)) >> i);
    }
    return result;
}

uint64_t GetNumber(std::vector<bool>& bits) {
    uint64_t result = 0;

    for (int64_t i = 0; i < bits.size() - 1; ++i) {
        result |= (bits[i] << i);
    }
    return result;
}

TEST(Parse, name) {
    char* argv[] = {"archive", "-c", "--file=ARCHIVE", "text.txt"};
    Argpars test = Argpars();
    test.Parse(4, argv);
    ASSERT_EQ(test.GetArchName(), "ARCHIVE");
    ASSERT_TRUE(test.GetFlagCreate());
}

TEST(Parse, name_flag) {
    char* argv[] = {"archive", "-c", "-f", "ARCHIVE", "text.txt"};
    Argpars test = Argpars();
    test.Parse(5, argv);
    ASSERT_EQ(test.GetArchName(), "ARCHIVE");
    ASSERT_TRUE(test.GetFlagCreate());
}

TEST(Parse, get_files) {
    char* argv[] = {"archive", "-c", "-f", "ARCHIVE", "text1.txt", "text2.txt"};
    Argpars test = Argpars();
    test.Parse(6, argv);
    ASSERT_EQ(test.GetListing().size(), 2);
}

TEST(Parse, get_flags) {
    char* argv[] = {"archive", "-x", "-f", "ARCHIVE", "file1.txt"};
    Argpars test = Argpars();
    test.Parse(4, argv);

    ASSERT_TRUE(test.GetExtract());
}

TEST(ArchiveTest, TestDecodeHamming_0) {
    std::vector<bool> bits = SetBits(153, 4, 0);
    EncodingNumber(bits,4);
    Decode(bits, 4);
    ASSERT_EQ(GetNumber(bits), 153);
}

TEST(ArchiveTest, TestDecodeHamming_1) {
    std::vector<bool> bits = SetBits(4652, 5, 0);
    EncodingNumber(bits, 5);
    bits[3] = !bits[3];
    Decode(bits, 5);
    ASSERT_EQ(GetNumber(bits), 4652);
}
