/**
 * Test file unit tests the methods in DictionaryTrie.cpp.
 * NOTE: more tests were made using custom dictionary files from the command 
 *  	 line
 * Author: Forest Jenkins
 * 	   fjenkins@ucsd.edu
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
//TEST INSERTING AND FINDING 1-LETTER ROOT
TEST(DictTrieTests, INSERT_FIND_ROOT_TEST) {
	DictionaryTrie d;
	d.insert("d", 10);
	ASSERT_EQ(d.find("d"), true);
}
//TEST INSERTING AND FINDING LONGER ROOT
TEST(DictTrieTests, INSERT_FIND_ROOT_2_TEST) {
	DictionaryTrie d;
	d.insert("dad", 1);
	ASSERT_EQ(d.find("dad"), true);
}
//TEST INSERTING AND FINDING MANY, MANY WORDS
TEST(DictTrieTests, INSERT_FIND_MULTIPLE) {
	DictionaryTrie d;
	d.insert("f", 1);
	d.insert("a", 2);
	d.insert("abrakadabra", 3);
	d.insert("g", 4);
	d.insert("ab", 5);
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
	ASSERT_EQ(d.find("f"), true);
	ASSERT_EQ(d.find("a"), true);
	ASSERT_EQ(d.find("abrakadabra"), true);
	ASSERT_EQ(d.find("g"), true);
	ASSERT_EQ(d.find("ab"), true);
	ASSERT_EQ(d.find("abc"), false);
	ASSERT_EQ(d.find("abra"), false);
}
//TEST INSERTING WORDS AND THEN DELETING TREE
TEST(DictTrieTests, DELETE_TRIE_TEST) {
	DictionaryTrie d;
	d.insert("abcdefghij", 10);
	d.insert("5k", 20);
	d.insert("~lkjlkj", 30);
	ASSERT_EQ(d.find("~lkjlkj"), true);
	d.~DictionaryTrie();
	ASSERT_EQ(d.find("~lkjlkj"), false);
	ASSERT_EQ(d.find("abcdefghij"), false);
}
//TEST FINDING AUTOCOMPLETIONS/ALPHABETIZATION
TEST(DictTrieTests, PREDICT_COMPLETIONS__ALPHABET_TEST) {
	DictionaryTrie d;
	ASSERT_EQ(d.insert("acc", 20), true);
	d.insert("aaa", 20);
	d.insert("acd", 20);
	d.insert("aaab", 20);
	d.insert("aaac", 20);
	d.insert("ac", 20);
	d.insert("aba", 20);
	d.insert("aaaa", 20);
	vector<string> v = d.predictCompletions("z", 7);
	ASSERT_EQ(v.size(), 0);
	vector<string> v2 = d.predictCompletions("a", 8);
	ASSERT_EQ(v2[0], "acd");
	ASSERT_EQ(v2[1], "acc");
	ASSERT_EQ(v2[2], "ac");
	ASSERT_EQ(v2[3], "aba");
	ASSERT_EQ(v2[4], "aaac");
	ASSERT_EQ(v2[5], "aaab");
	ASSERT_EQ(v2[6], "aaaa");
	ASSERT_EQ(v2[7], "aaa");
}
//TEST FINDING MUTLIPLE UNDERSCORED AUTOCOMPLETED STRINGS
TEST(DictTrieTests, PREDICT_UNDERSCORES_TEST) {
	DictionaryTrie d;
	d.insert("abbb", 40);
	d.insert("abba", 1);
	d.insert("aaca", 90);
	d.insert("bb", 1);
	d.insert("ba", 1);
	d.insert("bc", 1);
	d.insert("d", 1);
	d.insert("c", 1);
	d.insert("z", 1);
	d.insert("y", 1);
	d.insert("acaa", 65);
	d.insert("accc", 60);
	d.insert("a*&a", 70);
	d.insert("a+#a", 80);
	d.insert("azaa", 100);
	d.insert("zaaa", 90);

	vector<string> v = d.predictUnderscores("_", 2);
	ASSERT_EQ(v.size(), 2);
	ASSERT_EQ(v[1], "c");
	ASSERT_EQ(v[0], "d");

	vector<string> y = d.predictUnderscores("__", 10);
	ASSERT_EQ(y.size(), 3);
	ASSERT_EQ(y[2], "ba");
	ASSERT_EQ(y[1], "bb");
	ASSERT_EQ(y[0], "bc");

	vector<string> z = d.predictUnderscores("____", 9);
	ASSERT_EQ(z.size(), 9);
	ASSERT_EQ(z[0], "abba");
	ASSERT_EQ(z[1], "abbb");
	ASSERT_EQ(z[2], "accc");
	ASSERT_EQ(z[3], "acaa");
	ASSERT_EQ(z[4], "a*&a");
	ASSERT_EQ(z[5], "a+#a");
	ASSERT_EQ(z[6], "zaaa");
	ASSERT_EQ(z[7], "aaca");
	ASSERT_EQ(z[8], "azaa");

	vector<string> x = d.predictUnderscores("a__a", 1);
	ASSERT_EQ(x.size(), 1);
	ASSERT_EQ(x[0], "azaa");

	vector<string> a = d.predictUnderscores("_bb_", 100);
	ASSERT_EQ(a.size(), 2);
	ASSERT_EQ(a[0], "abba");
	ASSERT_EQ(a[1], "abbb");
}
//send insert, find, and dfs empty strings
TEST(DictTrieTests, RETURN_FALSE_EMPTY_TESTS) {
	DictionaryTrie d;
	ASSERT_EQ(d.find(""), false);
	ASSERT_EQ(d.find("HEY! "), false);
	ASSERT_EQ(d.insert("", 10), false);

	vector<string> v = d.predictCompletions("", 1);
	ASSERT_EQ(v.size(), 0);

	vector<string>y = d.predictUnderscores("__", 1);
	ASSERT_EQ(y.size(), 0);
}
