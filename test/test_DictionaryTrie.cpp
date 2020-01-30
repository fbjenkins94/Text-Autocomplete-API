/**
 * TODO: File HEADER
 *
 * Author:
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;
using namespace testing;

/* Empty test */
TEST(DictTrieTests, EMPTY_TEST) {
    DictionaryTrie dict;
    ASSERT_EQ(dict.find("abrakadabra"), false);
}
TEST(DictTrieTests, INSERT_FIND_ROOT_TEST) {
	DictionaryTrie d;
	d.insert("d", 10);
	d.insert("acc", 10);
	d.insert("accc", 10);
	d.insert("accd", 10);
	d.insert("fab", 10);
	d.insert("afa", 10);
	d.insert("b", 10);
	d.insert("g", 10);
	d.insert("c", 10);
	ASSERT_EQ(d.find("acc"), true);
	ASSERT_EQ(d.find("accc"), true);
	ASSERT_EQ(d.find("accd"), true);
	ASSERT_EQ(d.find("fab"), true);
	ASSERT_EQ(d.find("afa"), true);
	ASSERT_EQ(d.find("b"), true);
	ASSERT_EQ(d.find("g"), true);
	ASSERT_EQ(d.find("c"), true);
	ASSERT_EQ(d.find("d"), true);
}

TEST(DictTrieTests, INSERT_FIND_MULTIPLE) {
	DictionaryTrie d;
	d.insert("f", 1);
	d.insert("a", 2);
	d.insert("abrakadabra", 3);
	d.insert("g", 4);
	d.insert("ab", 5);
	ASSERT_EQ(d.find("f"), true);
	ASSERT_EQ(d.find("a"), true);
	ASSERT_EQ(d.find("abrakadabra"), true);
	ASSERT_EQ(d.find("g"), true);
	ASSERT_EQ(d.find("ab"), true);
	ASSERT_EQ(d.find("abc"), false);
	ASSERT_EQ(d.find("abra"), false);
}

TEST(DictTrieTests, DELETE_TRIE) {
	DictionaryTrie d;
	d.insert("abcdefghij", 10);
	d.insert("5k", 20);
	d.insert("~lkjlkj", 30);
	ASSERT_EQ(d.find("~lkjlkj"), true);
	d.~DictionaryTrie();
	ASSERT_EQ(d.find("~lkjlkj"), false);
	ASSERT_EQ(d.find("abcdefghij"), false);
}

TEST(DictTrieTests, PREDICT_COMPLETIONS_TEST) {
	DictionaryTrie d;
	ASSERT_EQ(d.insert("acc", 20), true);
	d.insert("aaa", 20);
	d.insert("acd", 20);
	d.insert("aaab", 20);
	d.insert("aaac", 20);
	d.insert("ac", 20);
	d.insert("aba", 20);
	vector<string> v = d.predictCompletions("z", 7);
	ASSERT_EQ(v.size(), 0);
	//ASSERT_EQ(v[0], "aaac");
	/**ASSERT_EQ(v[1], "aaab");
	ASSERT_EQ(v[2], "aaac");
	ASSERT_EQ(v[3], "aba");
	ASSERT_EQ(v[4], "ac");
	ASSERT_EQ(v[5], "acc");
	ASSERT_EQ(v[6], "acd");**/
}

