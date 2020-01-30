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
		void setFreq(const int f) {
			frequency = f;
		}
		
};

struct  Compare{
public:
	bool operator() (pair<int, string> & a, pair<int, string> & b) {
		if(a.first == b.first) {
			if(a.second.compare(b.second) <0 ) return false;
			else if(b.second.compare(a.second) < 0) return true;
		}
		return a.first < b.first;
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
			root = new TrieNode(word[i], 0, false);
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
				n->setFreq(freq);
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

void dfs(TrieNode* n, 
		priority_queue<pair<int, string>, 
		vector<pair<int, string>> , Compare> & pq, 
		string s) {
	if(n) {
		dfs(n->left, pq, s);
		dfs(n->right, pq, s);
		s += n->getChar();
		dfs(n->middle, pq, s);
		if(n->word) {
			pair<int, string> p;
			p = make_pair(n->getFreq(), s);
			pq.push(p);
		}
	}	
}

/* TODO */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
	TrieNode * n = root;
 	priority_queue<pair<int, string>, vector<pair<int, string>> , Compare> pq; 
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
			vector<string> v(0);
			return v;
		}
		if(prefix[i] == n->getChar()) {
			if(i == j) { //if were at the end of the prefix and
				if(n->word) {	//it is a word in the tree, then insert it
					pair<int, string> p;
					p = make_pair(n->getFreq(), prefix);
					pq.push(p);
				}
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

	dfs(n, pq, prefix);

	vector<string> v = vector<string>();
	if(numCompletions <= pq.size()) {
		for(int i = 0; i < numCompletions; i++) {
			if(!pq.empty()) {
				v.push_back(pq.top().second);
				pq.pop();
			}
		}
	return v;
	}
	else {
		int r = pq.size();
		for(int i = 0; i < r; i++) {
			if(!pq.empty()) {
				v.push_back(pq.top().second);
				pq.pop();
			}
		}
	return v;
	}

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
