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
	d.insert("f", 10);
	ASSERT_EQ(d.find("f"), true);
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
	d.insert("a", 200);
	d.insert("a", 200);
	ASSERT_EQ(d.find("a"), true);
	d.insert("abc", 200);
	ASSERT_EQ(d.find("a"), true);
	ASSERT_EQ(d.find("ab"), false);
	ASSERT_EQ(d.find("abc"), true);
	ASSERT_EQ(d.find("a"), true);
	ASSERT_EQ(d.find("abc"), true);
	vector<string> v = d.predictCompletions("abc", 2);
	ASSERT_EQ(v[0], "HEY");
}

