/**
 * DictionaryTrie.cpp houses the programs ternary trie data structure, 
 * DictionaryTrie, that will house all of the dictionary entries and
 * their frequencies. autocomplete.cpp reads in all of the entries
 * from a .txt file and calls on DictionaryTrie.cpp to store all of
 * the entries from the file. The file's header file is DictionaryTrie.hpp
 *
 * Author: Forest Jenkins
 * 	   fjenkins@ucsd.edu
 */
#include "DictionaryTrie.hpp"
#include <iostream>
#include <queue>
/*
 * TrieNode class serves as DictionaryTrie's node type. Stores a single letter
 * of the dictionary entry, a boolean representing whether or not the letter
 * is the final letter of a word, and the words frequency.
 */
class TrieNode { 
	private:
		char letter;//letter of a dictionary word
		int frequency;//use frequency of the dictionary word
	public:
		bool word;//true = last letter of a word; false = no word
		TrieNode * left; //left node of current (lesser letter value)
		TrieNode * middle; //middle node of current (greater letter)
		TrieNode * right; //right node of current (same word)
		/*
		 * TrieNode's constructor accepts a dictionary letter's char
		 * value, its integer frequency, and a boolean for whether or
		 * not its a word. Initializes nodes to NULL.
		 */
		TrieNode(const char & c, const int & f, const bool & w) 
				: letter(c), frequency(f), word(w) {
			left = middle = right = NULL;
		}
		/*
		 * getChar() returns the node's private letter type.
		 */
		char getChar() {
			return letter;
		}
		/*
		 * getFreq() returns the node's private frequency type.
		 */
		int getFreq() {
			return frequency;
		}
		/*
		 * setFreq() accepts a constant integer value and sets the 
		 * private frequency type to it.
		 */
		void setFreq(const int f) {
			frequency = f;
		}
};
/*
 * Compare struct belongs to the priority_queue initialized in 
 * predictCompletions() and compares two pair data types containing an
 * integer frequency and string. If the two frequencies are the same then set
 * the pair whose string is alphabetically lower.
 */
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

/*
 * DictionaryTrie() is the DictionaryTrie's constructor method that initializes
 * a ternary trie tree with a NULL root (empty tree)
 */
DictionaryTrie::DictionaryTrie() : root(0) {}
/*
 * insert() accepts a dictionary entry string and its integer frequency. If the
 * trie is empty, then the function sets the root of the trie to the first 
 * letter of the dictionary entry. If the entry is only one letter long, then
 * the root's word value is set to true and the method returns true. Otherwise
 * the trie is traversed alphabetically, letter per letter, with letters added
 * if they do not already exist until the final letter of the entry is found on
 * the tree. If the word already exists then the method returns false, otherwise
 * the method sets the node's word value to true and the method returns true.
 */
bool DictionaryTrie::insert(string word, unsigned int freq) {
	int i = 0;
	int wordIndex = word.length()-1;

	if(root == NULL) {
		if(i != wordIndex) { //if entry is more than one letter long, continue
			root = new TrieNode(word[i], 0, false);
		}
		else { //if entry is one letter long, set root to be a word
			root = new TrieNode(word[i], freq, true);
			return true;
		}
	}

	TrieNode * n = root;
	while(i <= wordIndex) {
		if(word[i] == n->getChar()) { //if string char == node letter
			if(i==wordIndex && !(n->word)) { 
				//if were at the end of the word and node != word
				n->word = true;	//then turn node into a word
				n->setFreq(freq);
				n = NULL;
				return true;
			}
			if(i==wordIndex && n->word) { 
				//if were at the end of the word and node == word
				n = NULL;
				return false;	//return false for duplicate word
			}
			if(!n->middle) {//if there is no middle node
				//set middle node to the next char in the word
				n->middle = new TrieNode(word[i+1], 0, false);
				n = n->middle;
				++i;
			}
			else {//otherwise, go to the middle node
				n = n->middle;
				++i;
			}

		} //if the strings current letter is alphabetically less than 
		else if(word[i] < n->getChar()) { // the node's char
			if(n->left) { //then go to the left node if it exists
				n = n->left;
			}
			else {//create the left node if it does not exist
				n->left = new TrieNode(word[i], 0, false);
				n = n->left;
			}
		}//same as the previous else if loop, but with greater letter
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
	n = NULL;
	return false;//return false if the word could not be added
}
/*
 * find() accepts a dictionary entry and returns true or false based
 * on whether or not it is a word in the ternary trie. The algorithm is
 * pretty much the same as insert()'s.
 */
bool DictionaryTrie::find(string word) const {
	TrieNode * n = root;
	int i = 0;
	int wordLength = word.length()-1;
	
	while(i <= wordLength) {
		if(!n) {
			return false;
		}
		if(word[i] == n->getChar()) {
			if(i == wordLength) {
				bool h = n->word;
				n = NULL;
				return h;
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
/*
 * dfs() accepts a root TrieNode, the priority queue initialied in 
 * predictCompletions(), and a string representing the string constructed from
 * traversing the DictionaryTrie up to the current node. The method recursively
 * traverses the tree in a depth-frist search and adds the current char to the
 * string when there is no left or right node to traverse. If the node is then
 * deemed to belong to a word, then a pair is constructed using the string 
 * and the node's frequency. This is then pushed on to priority_queue.
 */
void dfs(TrieNode* n, 
		priority_queue<pair<int, string>, 
		vector<pair<int, string>> , Compare> & pq, 
		string s) {
	if(n) { //if node != NULL
		dfs(n->left, pq, s); //traverse the left subtree
		dfs(n->right, pq, s);//traverse the right subtree
		s += n->getChar();//add the nodes's char to the prefix
		dfs(n->middle, pq, s);//traverse the middle subtree
		if(n->word) {//if it is a word
			pair<int, string> p;
			p = make_pair(n->getFreq(), s); //add it to a pair
			pq.push(p);//push the word and frequency onto queue
		}
	}
}
/*
 * predict Completions() accepts a string representing the prefix that wil be
 * autocompleted, adn the desired number of autocompletions to appear to the
 * user. The function first makes sure the prefix and tree are not empty, 
 * and then traverses the tree until the prefix is found. If these first two
 * tests fail, the function returns an empty vector<string>, otherwise, the 
 * function then populates a priority_queue with frequency-ordered, alphabet-
 * ized autocompletions, whose strings are then pushed onto the vector<string>
 * structure that is returned. 
 */
vector<string> DictionaryTrie::predictCompletions(string prefix,
                                                  unsigned int numCompletions) {
	TrieNode * n = root;
	//queue holds pairs of integer frequencies and autocompleted dictionary
	//entries, the underlying container type (vector<...>), and the 
	//custom compare structure defined at the top of the file
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
	int wordLength = prefix.length()-1;
	while(i <= wordLength) {
		if(n==NULL) { //if the prefix does not exist in the tree then return null vector
			vector<string> v(0);
			return v;
		}
		if(prefix[i] == n->getChar()) {
			if(i == wordLength) { //if were at the end of the prefix and
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
	}//end of the prefix has been found in DictionaryTrie
	dfs(n, pq, prefix);//now find all autocompleted words 
	n = NULL;
	vector<string> v = vector<string>();
	//if there are more (or equaL) autocompleted words than desired...
	if(numCompletions <= pq.size()) {
		for(int i = 0; i < numCompletions; i++) {
			if(!pq.empty()) {//don't cause segfault
				v.push_back(pq.top().second);
				pq.pop();
			}
		}
	return v;
	}
	//if there are less autocompleted words than desired
	else {
		int r = pq.size();
		for(int i = 0; i < r; i++) {
			if(!pq.empty()) {//don't cause segfault
				v.push_back(pq.top().second);
				pq.pop();
			}
		}
	return v;
	}

}
/*
 * 
 */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
    return {};
}
/*
 * ~DictionaryTrie() is the DictionaryTrie's destructor method. In order
 * to avoid a SIGSEV error, the method initializes a pointer that points
 * to the root node and then passes it off to the deleteAll() helper function
 * that effectively deletes every node in the dictionaryTrie.
 */
DictionaryTrie::~DictionaryTrie() {
	TrieNode * node = root;
	root = NULL;
	deleteAll(node);
}
/**
 * deleteAll() is the helper function to the destructor. It recursively deletes
 * every TrieNode in the DictionaryTrie. Return void.
 */
void DictionaryTrie::deleteAll(TrieNode * node) {
	if(node) {
		deleteAll(node->left);
		deleteAll(node->right);
		deleteAll(node->middle);
		delete node;
	}
}
