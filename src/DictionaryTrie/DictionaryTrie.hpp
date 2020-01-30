/**
 * DictionaryTrie.hpp is a header file for DictionaryTrie.cpp and declares
 * the members and methods for that class. Also included is the helper class
 * TrieNode, which the DictionaryTrie will use as its node, and Compare, which
 * helps to compare pairs of dictionary strings and their use frequencies.
 *
 * Author: Forest Jenkins
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <string>
#include <utility>
#include <vector>
#include <queue>

using namespace std;
/*
 *TrieNode class will serve as the DictionaryTrie's node item.
 */
class TrieNode;
/*
 * Compare helps to compare pair items containing dictionary entries and
 * their use frequencies.
 */
struct Compare;

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
	private:
		TrieNode* root; //root of DictionaryTrie
		/*
		 * deleteAll() declaration that accepts a TrieNode pointer
		 * and deletes every node in the DictionaryTrie.
		 */
		void deleteAll(TrieNode* node);
	public:
		/*
		 *declaration of DictionaryTrie's constructor method
		 */
		DictionaryTrie();
		/*
		 *declaration of the insert() method. accepts a dictionary string
		 *'word' and the words integer frequency. Returns true if the 
		 *string was inserted properly into the ternary trie and false if
		 *not.
		 */
		bool insert(string word, unsigned int freq);
		/*
		 * declaration of find(). accepts dictionary string 'word' and 
		 * returns true if the word was found in the trie, false if not
		 */
		bool find(string word) const;
		/*
		 * declaration for predictCompletions(). accepts a string 
		 * representing the prefix for a dictionary entry and an 
		 * unsigned integer value representing the number of autocomplete
		 * strings the user desires to have returned. Returns said
		 * strings in a vector<string>.
		 */
		vector<string> predictCompletions(string prefix,
				unsigned int numCompletions);
		/*
		 * 
		 */
		vector<string> predictUnderscores(string pattern,
				unsigned int numCompletions);
		/*
		 * declaration for DictionaryTrie's destructor method. Deletes 
		 * all nodes in the trie.
		 */
		~DictionaryTrie();
};

#endif  // DICTIONARY_TRIE_HPP
