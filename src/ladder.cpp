#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <set>
#include <string>
#include <algorithm>

void error(string word1, string word2, string msg) {
    cerr << "Error: " << msg << " (" << word1 << ", " << word2 << ")\n";
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int len1 = str1.length();
    int len2 = str2.length();

    // If the length difference is greater than d, they are not within edit distance
    if (abs(len1 - len2) > d) return false;

    // Count how many positions the strings differ
    int diff_count = 0;
    int i = 0, j = 0;

    while (i < len1 && j < len2) {
        if (str1[i] != str2[j]) {
            diff_count++;
            if (diff_count > d) return false;

            // Skip one character in the longer string (insert or delete)
            if (len1 > len2) {
                i++;
            } else if (len2 > len1) {
                j++;
            } else {
                i++;
                j++;
            }
        } else {
            i++;
            j++;
        }
    }

    // If the remaining part of the string is greater than allowed edit distance, return false
    if (i < len1 || j < len2) {
        diff_count++;
    }

    return diff_count <= d;
}


bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        return {begin_word};
    }

    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    
    set<string> visited;
    visited.insert(begin_word);
    
    while (!ladder_queue.empty()) {
        int ladder_size = ladder_queue.size();
        for (int i = 0; i < ladder_size; i++) {
            vector<string> ladder = ladder_queue.front();
            ladder_queue.pop();
            
            string last_word = ladder.back();
            
            if (last_word == end_word) {
                return ladder;  // Return the ladder once we reach the end word
            }
            
            for (const string& word : word_list) {
                if (is_adjacent(last_word, word) && visited.find(word) == visited.end()) {
                    visited.insert(word);
                    vector<string> new_ladder = ladder;
                    new_ladder.push_back(word);
                    ladder_queue.push(new_ladder);
                }
            }
        }
    }
    
    return {};  // No ladder found
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    string word;
    while (file >> word) {
        word_list.insert(word);
    }
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found.\n";
        return;
    }

    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); ++i) {
        cout << ladder[i];
        if (i != ladder.size() - 1) {  // Avoid printing a space after the last word
            cout << " ";
        }
    }
    cout << " \n";  // Ensure there is a space before the newline
}


#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

void verify_word_ladder() {

    set<string> word_list;

    load_words(word_list, "words.txt");

    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);

    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);

    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);

    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);

    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);

    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);

}


void test_edit_distance_within() {
    // Test case 1: Equal strings (distance should be 0)
    cout << "Test 1: " << (edit_distance_within("hello", "hello", 1) ? "Passed" : "Failed") << endl;

    // Test case 2: 1 character difference (distance should be 1)
    cout << "Test 2: " << (edit_distance_within("hello", "hallo", 1) ? "Passed" : "Failed") << endl;

    // Test case 3: More than 1 character difference, with max allowed difference being 1 (should fail)
    cout << "Test 3: " << (edit_distance_within("hello", "world", 1) ? "Passed" : "Failed") << endl;

    // Test case 4: Strings with different lengths, and a maximum allowed distance of 1 (should fail)
    cout << "Test 4: " << (edit_distance_within("hello", "helloo", 1) ? "Passed" : "Failed") << endl;

    // Test case 5: More than 1 difference but within max allowed difference (should pass)
    cout << "Test 5: " << (edit_distance_within("hello", "hell", 1) ? "Passed" : "Failed") << endl;

    // Test case 6: Same length but multiple differences (should fail for max distance 1)
    cout << "Test 6: " << (edit_distance_within("hello", "hxlxo", 1) ? "Passed" : "Failed") << endl;

    // Test case 7: More than max distance allowed (should fail)
    cout << "Test 7: " << (edit_distance_within("abc", "xyz", 2) ? "Passed" : "Failed") << endl;
}

void test_is_adjacent() {
    // Test case 1: Adjacent words with one character difference
    cout << "Test 1: " << (is_adjacent("hello", "hallo") ? "Passed" : "Failed") << endl;

    // Test case 2: Non-adjacent words with more than one character difference
    cout << "Test 2: " << (is_adjacent("hello", "world") ? "Passed" : "Failed") << endl;

    // Test case 3: Words that are the same (should not be adjacent)
    cout << "Test 3: " << (is_adjacent("hello", "hello") ? "Passed" : "Failed") << endl;

    // Test case 4: Adjacent words with one character insertion
    cout << "Test 4: " << (is_adjacent("hell", "hello") ? "Passed" : "Failed") << endl;

    // Test case 5: Adjacent words with one character deletion
    cout << "Test 5: " << (is_adjacent("hello", "hell") ? "Passed" : "Failed") << endl;

    // Test case 6: Words with multiple character differences
    cout << "Test 6: " << (is_adjacent("hello", "hxlxo") ? "Passed" : "Failed") << endl;
}