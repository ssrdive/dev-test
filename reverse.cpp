/*
 * File:   reverse.cpp
 * Author: Shamal Sandeep <ssdh.gdrive@gmail.com>
 * Date:   April 16, 2024
 * Brief:  Sample code to reverse words in a given string.
 */

#include <cassert>
#include <iostream>
#include <string>

/**
 * Reverses a word to the passed dest vector based left_idx and right_idx
 *
 * @param src       Source string.
 * @param dest      Destination char vector pointer to place the result.
 * @param left_idx  Start position of the src string word.
 * @param right_idx End position of the src string word.
 */
void reverse_word(const std::string& src, std::vector<char>* const dest, 
                    int left_idx, int right_idx) {
    // vec_placement is used to place the string in the dest vector in reverse
    // order when looping through the word from right to left.
    int vec_placement = left_idx;

    while (right_idx >= left_idx) {
        dest->at(vec_placement++) = src[right_idx];
        --right_idx;
    }
}

std::string reverse_words(const std::string &str)
{
    // No need to reverse if the string is of size 0 or 1
    if (str.size() == 0) return std::string("");
    if (str.size() == 1) return str;

    // Fixed size vector is used since we know the resulting string is also
    // going to be the same size as the source string. This removes the overhead
    // of string manipulation and memory is allocated only once and no resizes
    // will be done in the reversing operation.
    std::vector<char>* reversed_chars = new std::vector<char>(str.size());  

    // Memory footprint can be further reduced if we know the max size of the
    // src string beforehand. Based on that we can use uint8_t or uint16_t
    int word_start = 0;
    int word_end = 0;

    while (word_end != str.size()) {
        // We've come across a non-alpha numeric character. Need to reverse
        // the string starting from end word_start and ending at world_end.
        // Also, we can place the non-alpha numeric character in reversed_chars
        // vector in the same position as it occurs in the source string 
        // because the position of non-alpha numeric characters aren't changed
        if (!std::isalnum(str[word_end])) {
            reversed_chars->at(word_end) = str[word_end];
            reverse_word(str, reversed_chars, word_start, word_end - 1);
            word_start = word_end + 1;
        }

        ++word_end;
    }

    return std::string(reversed_chars->data(), reversed_chars->size());
}

// A sample to function to test the reverse_words function. Best approach 
// will be to use gtest, but since external linking is required for therefore,
// instead this function is used.
void run_test(const std::string& input, const std::string& expected_output) {
    std::string result = reverse_words(input);
    if (result == expected_output) {
        std::cout << "Test Passed: " << input << " -> " << result << std::endl;
    } else {
        std::cerr << "Test Failed: " << input << " -> " << result << 
                    " (Expected: " << expected_output << ")" << std::endl;
    }
}

int main()
{
    std::string test_str = "String; 2be reversed..."; 
    assert(reverse_words(test_str) == "gnirtS; eb2 desrever..."); 
    run_test("String; 2be reversed...", "gnirtS; eb2 desrever...");
    run_test(";;;String; 2be reversed...", ";;;gnirtS; eb2 desrever...");
    run_test("String; 2be reve--rsed...", "gnirtS; eb2 ever--desr...");
    return 0;
}