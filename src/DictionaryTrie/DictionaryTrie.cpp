/**
 * TODO: File Header
 *
 * Author:
 */
#include "DictionaryTrie.hpp"
#include <iostream>
#include <queue>

class TrieNode { 
	private:
		char letter;
		int frequency;
	public:
		bool word;
		TrieNode * left;
		TrieNode * middle;
		TrieNode * right;

		TrieNode(const char & c, const int & f, const bool & w) 
				: letter(c), frequency(f), word(w) {
			left = middle = right = NULL;
		}
		char getChar() {
			return letter;
		}
		int getFreq() {
			return frequency;
		}	
};


/* TODO */
DictionaryTrie::DictionaryTrie() : root(0) {}

/* TODO */
bool DictionaryTrie::insert(string word, unsigned int freq) {
	int i = 0;
	int k = word.length()-1;

	if(root == NULL) {
		if(i != k) {
			root = new TrieNode(word[i], freq, false);
			++i;	
		}
		else {
			root = new TrieNode(word[i], freq, true);
			return true;
		}
	}

	TrieNode * n = root;
	while(i <= k) {
		if(word[i] == n->getChar()) { //if string char == node letter
			if(i==k && !(n->word)) { //if were at the end of the word and node != word
				n->word = true;	//then turn node into a word
				return true;
			}
			if(i==k && n->word) { //if were at the end of the word and node == word
				return false;	//return false for duplicate word
			}
			if(!n->middle) {
				n->middle = new TrieNode(word[i+1], 0, false);
				n = n->middle;
				++i;
			}
			else {
				n = n->middle;
				++i;
			}

		}
		else if(word[i] < n->getChar()) {
			if(n->left) {
				n = n->left;
			}
			else {
				n->left = new TrieNode(word[i], 0, false);
				n = n->left;
			}
		}
		else if(word[i] > n->getChar()) {
			if(n->right) {
				n = n->right;
			}
			else {
				n->right = new TrieNode(word[i], 0, false);
				n = n->right;
			}
		}
	}

	return false;
}

/* TODO */
bool DictionaryTrie::find(string word) const {
	TrieNode * n = root;
	int i = 0;
	int k = word.length()-1;
	
	while(i <= k) {
		if(!n) {
			return false;
		}
		if(word[i] == n->getChar()) {
			if(i == k) {
				return n->word;
			}
			n = n->middle;
			++i;
		}
		else if(word[i] < n->getChar()) {
			n = n->left;
		}
		else if(word[i] > n->getChar()) {
			n = n->right;
		}
	}
	return false;
}

void dfs(TrieNode* n, priority_queue<pair<unsigned int, string>> & pq, 
		string s, unsigned int & numCompletions) {
	if(n) {
		s += n->getChar();
		if(n->word) {
			pair<unsigned int, string> p;
			p = make_pair(n->getFreq(), s);
			pq.push(p);
		}
		dfs(n->left, pq, s, numCompletions);
		dfs(n->right, pq, s, numCompletions);
		dfs(n->middle, pq, s, numCompletions);
	}	
}

/* TODO */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
	TrieNode * n = root;
	priority_queue<pair<unsigned int, string>> *pq = new priority_queue<pair<unsigned int, string>>();
	/**
	 *check if prefix is empty or tree is empty
	 */
	if(prefix == "" || n == NULL) {
		vector<string> v(0);
		return v;
	}
	/**
	 *find the end of the prefix in the tree
	 */
	int i = 0;
	int j = prefix.length()-1;
	while(i <= j) {
		if(n==NULL) { //if the prefix does not exist in the tree then return null vector
			vector<string> v{"no"};
			return v;
		}
		if(prefix[i] == n->getChar()) {
			if(i == j) { //if were at the end of the prefix and
			/**	if(n->word) {	//it is a word in the tree, then insert it
					pair<unsigned int, string> p;
					p = make_pair(n->getFreq(), prefix);
					pq->push(p);
				}**/
				vector<string> v{"HEY"};
				return v;
			}
			n = n->middle;
			++i;
		}
		else if(prefix[i] < n->getChar()) {
			n = n->left;
		}
		else if(prefix[i] > n->getChar()) {
			n = n->right;
		}
	}
/**	string s = "HEY";
	dfs(n, pq, s, numCompletions);
	int k = pq.size();
	if(numCompletions <= k) {
		vector<string> v(numCompletions);
		for(int i = 0; i < numCompletions; i++) {
			v.push_back(pq.top().second);
			pq.pop();
		}
		return v;
	}
	else {
		vector<string> v(k);
		for(int i = 0; i < k; i++) {
			v.push_back(pq.top().second);
			pq.pop();
		}
		return v;
	}**/

}

/* TODO */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}

/* TODO */
DictionaryTrie::~DictionaryTrie() {
	TrieNode * node = root;
	root = NULL;
	deleteAll(node);
}

/**
 *000000000 REMEMBER TO ADD THIS IN .hpp 000000000
 */

void DictionaryTrie::deleteAll(TrieNode * node) {
	if(node != NULL) {
		delete node;
	}
	return;
}

char DictionaryTrie::findRoot() {
	return root->getChar();
}
