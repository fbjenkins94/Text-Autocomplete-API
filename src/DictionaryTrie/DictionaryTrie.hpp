/**
 * TODO: File Header
 *
 * Author:
 */
#ifndef DICTIONARY_TRIE_HPP
#define DICTIONARY_TRIE_HPP

#include <string>
#include <utility>
#include <vector>

using namespace std;

class TrieNode;

/**
 * The class for a dictionary ADT, implemented as either
 * a mulit-way trie or a ternary search tree.
 */
class DictionaryTrie {
	protected:
		TrieNode* root;
		// TODO: add private members and helper methods here

	public:
		/* TODO: add function header */
		DictionaryTrie();

		/* TODO: add function header */
		bool insert(string word, unsigned int freq);

		/* TODO: add function header */
		bool find(string word) const;

		/* TODO: add function header */
		vector<string> predictCompletions(string prefix,
				unsigned int numCompletions);

		/* TODO: add function header */
		vector<string> predictUnderscores(string pattern,
				unsigned int numCompletions);

		/* TODO: add function header */
		~DictionaryTrie();

		char findRoot();
	private:
		void deleteAll(TrieNode * node);
};

#endif  // DICTIONARY_TRIE_HPP
