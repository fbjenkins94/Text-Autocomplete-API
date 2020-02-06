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
		unsigned int max = 0;
	public:
		TrieNode * left; //left node of current (lesser letter value)
		TrieNode * middle; //middle node of current (greater letter)
		TrieNode * right; //right node of current (same word)
		TrieNode * parent;
		/*
		 * TrieNode's constructor accepts a dictionary letter's char
		 * value, its integer frequency, and a boolean for whether or
		 * not its a word. Initializes nodes to NULL.
		 */
		TrieNode(const char & c, const int & f, const int & m) 
				: letter(c), frequency(f), max(m) {
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
		void setMax(unsigned int f) {
			max = f;
		}
		unsigned int getMax() {
			return max;
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
		if(a.first == b.first) return a.second < b.second;
		return a.first > b.first;
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
			root = new TrieNode(word[i], 0, 0);
		}
		else { //if entry is one letter long, set root to be a word
			root = new TrieNode(word[i], freq, 0);
			return true;
		}
	}

	TrieNode * n = root;
	while(i <= wordIndex) {
		if(word[i] == n->getChar()) { //if string char == node letter
			if(i==wordIndex && !(n->getFreq())) { 
				//if were at the end of the word and node != word
				n->setFreq(freq);
				//if(n->getMax()==0) n->setMax(freq);
				//n = n->parent;
				while(freq > n->getMax()) {
					n->setMax(freq);
					if(n != root) n = n->parent;
				}
				return true;
			}
			if(i==wordIndex && n->getFreq()) { 
				//if were at the end of the word and node == word
				return false;	//return false for duplicate word
			}
			if(!n->middle) {//if there is no middle node
				//set middle node to the next char in the word
				n->middle = new TrieNode(word[i+1], 0, 0);
				n->middle->parent = n;
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
				n->left = new TrieNode(word[i], 0, 0);
				n->left->parent = n;
				n = n->left;
			}
		}//same as the previous else if loop, but with greater letter
		else if(word[i] > n->getChar()) {
			if(n->right) {
				n = n->right;
			}
			else {
				n->right = new TrieNode(word[i], 0, 0);
				n->right->parent = n;
				n = n->right;
			}
		}
	}
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
				return n->getFreq();
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
		vector<char> & v, unsigned int nc) {
	if(n) { //if node != NULL
		if(n->left) {
			if(pq.size() < nc || pq.top().first <= n->left->getMax()) {
				dfs(n->left, pq, v, nc); //traverse the left subtree
			}
		}
		if(n->right) {
			if(pq.size() < nc || pq.top().first <= n->right->getMax()) {
				dfs(n->right, pq, v, nc); //traverse the left subtree
			}
		}

		v.push_back(n->getChar());//add the nodes's char to the prefix
		if(n->middle) {
			if(pq.size() < nc || pq.top().first <= n->middle->getMax()) {
				dfs(n->middle, pq, v, nc); //traverse the left subtree
			}
		}
		if(n->getFreq()) {//if it is a word
			if(pq.size() < nc) {
				string s = "";
				for(int i = 0; i < v.size(); i++) {
					s += v[i];
				}
				pair<int, string> p;
				p = make_pair(n->getFreq(), s); //add it to a pair
				pq.push(p);//push the word and frequency onto queue
			}
			else if(pq.top().first <= n->getFreq()) {
				string s = "";
				for(int i = 0; i < v.size(); i++) {
					s += v[i];
				}
				if(pq.top().first < n->getFreq()) {
					pq.pop();
					pair<int, string> p;
					//add it to a pair
					p = make_pair(n->getFreq(), s); 
					//push the word and frequency onto queue
					pq.push(p);
				}
				else if(s < pq.top().second) {
					pq.pop();
					pair<int, string> p;
					//add it to a pair
					p = make_pair(n->getFreq(), s); 
					//push the word and frequency onto queue
					pq.push(p);
				}
			}
		}
		v.pop_back();
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
	vector<char> cv = vector<char>();
	while(i <= wordLength) {
		if(n==NULL) { //if the prefix does not exist in the tree then return null vector
			vector<string> v(0);
			return v;
		}
		if(prefix[i] == n->getChar()) {
			cv.push_back(n->getChar());
			if(i == wordLength) { //if were at the end of the prefix and
				if(n->getFreq()) {	//it is a word in the tree, then insert it
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
	dfs(n, pq, cv, numCompletions);//now find all autocompleted words 
	vector<string> v = vector<string>();
	int k = pq.size();
	//if there are more (or equaL) autocompleted words than desired...
	for(int i = 0; i < k; i++) {
		if(!pq.empty()) {//don't cause segfault
			v.push_back(pq.top().second);
			pq.pop();
		}
	}
	return v;
}

void dfsUnderscores(TrieNode* n, 
		priority_queue<pair<int, string>, 
		vector<pair<int, string>> , Compare> & pq, 
		vector<char> & v, string & word, unsigned int nc) {
	if(n) { //if node != NULL
		if(v.size() < word.length()) {
			if(word[v.size()] == '_') {
				if(n->left) {
					if(pq.size() < nc ||
					pq.top().first <= n->left->getMax()) {
						dfsUnderscores(n->left, pq, v, word, nc);
					}
				}
				if(n->right) {
					if(pq.size() < nc ||
					pq.top().first <= n->right->getMax()) {
						dfsUnderscores(n->right, pq, v, word, nc);
					}
				}

				v.push_back(n->getChar());
				if(n->middle) {
					if(pq.size() < nc ||
					pq.top().first <= n->middle->getMax()) {
						dfsUnderscores(n->middle, pq, v, word, nc);
					}
				}
			}
			else if(word[v.size()] == n->getChar()) {
				v.push_back(n->getChar());
				if(n->middle) {
					if(pq.size() < nc ||
					pq.top().first <= n->right->getMax()) {
						dfsUnderscores(n->middle, pq, v, word, nc);
					}
				}
			}
			else if(n->getChar() < word[v.size()]) {
				if(n->right) {
					if(pq.size() < nc ||
					pq.top().first <= n->right->getMax()) {
						dfsUnderscores(n->right, pq, v, word, nc);
					}
				}
				v.push_back(n->getChar());
			}
			else if(word[v.size()] < n->getChar()) {
				if(n->left) {
					if(pq.size() < nc ||
					pq.top().first <= n->left->getMax()) {
						dfsUnderscores(n->left, pq, v, word, nc);
					}
				}
				v.push_back(n->getChar());
			}
		}
		if(v.size() == word.length() && n->getFreq() != 0) {
			if((word[v.size()-1] == '_') || word[v.size()-1] == v[v.size()-1]) {
				if(pq.size() < nc) {
					string s = "";
					for(int i = 0; i < v.size(); i++) {
						s += v[i];
					}
					pair<int, string> p;
					p = make_pair(n->getFreq(), s); //add it to a pair
					pq.push(p);//push the word and frequency onto queue
				}
				else if(pq.top().first <= n->getFreq()) {
					string s = "";
					for(int i = 0; i < v.size(); i++) {
						s += v[i];
					}
					if(pq.top().first < n->getFreq()) {
						pq.pop();
						pair<int, string> p;
						//add it to a pair
						p = make_pair(n->getFreq(), s); 
						//push the word and frequency onto queue
						pq.push(p);
					}
					else if(s < pq.top().second) {
						pq.pop();
						pair<int, string> p;
						//add it to a pair
						p = make_pair(n->getFreq(), s); 
						//push the word and frequency onto queue
						pq.push(p);
					}
				}	
			}
		}
		if(v.size()) {
			v.pop_back();
		}
	}
}
/*
 * 
 */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
	TrieNode * n = root;
 	priority_queue<pair<int, string>, vector<pair<int, string>> , Compare> pq; 

	if(pattern == "" || n == NULL) {
		vector<string> v{};
		return v;
	}

	vector<char> cv = vector<char>();
	cout<<"HERE 1"<<endl;
	dfsUnderscores(n, pq, cv, pattern, numCompletions);
	cout<<"HERE 2"<<endl;
	vector<string> v = vector<string>();
	int k = pq.size();
	//if there are more (or equaL) autocompleted words than desired...
	for(int i = 0; i < k; i++) {
		if(!pq.empty()) {//don't cause segfault
			v.push_back(pq.top().second);
			pq.pop();
		}
	}
	return v;
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
