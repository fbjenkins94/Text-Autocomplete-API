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
#include <stack>
/*
 * TrieNode class serves as DictionaryTrie's node type. Stores a single letter
 * of the dictionary entry, a boolean representing whether or not the letter
 * is the final letter of a word, and the words frequency.
 */
class TrieNode { 
	private:
		char letter;//letter of a dictionary word
		int frequency;//use frequency of the dictionary word
		unsigned int max = 0;//max frequency in the subtree of this node
	public:
		TrieNode * left; //left node of current (lesser letter value)
		TrieNode * middle; //middle node of current (greater letter)
		TrieNode * right; //right node of current (same word)
		TrieNode * parent;//parent node helps to assign max integers
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
		/*
		 * setMax() accepts an integer value representing the maximum
		 * frequency in the subtree that this node belongs to. It is
		 * set in the insert() method upon successfully implanting a new
		 * node into the trie.
		 */
		void setMax(unsigned int f) {
			max = f;
		}
		/*
		 * getMax() simply returns the private value 'max' that 
		 * represents the maximum frequency of the subtree this node
		 * belongs to.
		 */
		unsigned int getMax() {
			return max;
		}
};
/*
 * Compare struct belongs to the priority_queue initialized in 
 * predictCompletions() and compares two pair data types containing an
 * integer frequency and string. If the two frequencies are the same then set
 * the pair whose string is alphabetically higher. Observe that this is now
 * a min heap, as lower frequencies and higher alphabetical words are now
 * stored towards the top
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
				/*
				 * New addition; while loop incrementally goes 
				 * through the parent node, etc., of the newly
				 * added node and changes the parents' max 
				 * value if this node's frequency is greater
				 * than their max values.
				 */
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
 * predictCompletions(), a vector<char> holding the string constructed from
 * traversing the DictionaryTrie up to the current node, and the number of 
 * completions that the user typed in the main method. The method recursively
 * traverses the tree in a depth-frist search and adds the current char to the
 * vector when there is no left or right node to traverse. If the node is then
 * deemed to belong to a word, then a pair is constructed using the string 
 * and the node's frequency. This is then pushed on to priority_queue.
 *
 * UPDATE: the method is now updated for run time optimization. The priority 
 * queue is now only allowed nc elements at most, representing the number
 * of completions that the user assigned. If there are nc elements in the 
 * priority queue and a better match is found, then the top element of the
 * prioirty queue, which now hold elements from lowest frequency to greatest,
 * is popped and the new element is added. Additionally, the ternary trie is
 * pruned by assigning max values to nodes and avoiding subtrees that contain
 * a lower max value than the current best in the prioirty queue, if the 
 * priority queue is already full. 
 */
void dfs(TrieNode* n, 
		priority_queue<pair<int, string>, 
		vector<pair<int, string>> , Compare> & pq, 
		vector<char> & v, unsigned int nc) {
	if(n) { //if node != NULL
		if(n->left) {//avoid adding to the stack if the node is null
			//only traverse if the priority queue is not full or if
			//the left subtree contains a higher frequency than 
			//current best
			if(pq.size() < nc || pq.top().first <= n->left->getMax()) {
				dfs(n->left, pq, v, nc); //traverse the left subtree
			}
		}
		if(n->right) {//avoid adding to the stack if the node is null
			//only traverse if the priority queue is not full or if
			//the right subtree contains a higher frequency than 
			//current best
			if(pq.size() < nc || pq.top().first <= n->right->getMax()) {
				dfs(n->right, pq, v, nc); //traverse the left subtree
			}
		}

		v.push_back(n->getChar());//add the nodes's char to the prefix 
		if(n->middle) {//avoid adding to the stack if the node is null
			//only traverse if the priority queue is not full or if
			//the middle subtree contains a higher frequency than 
			//current best
			if(pq.size() < nc || pq.top().first <= n->middle->getMax()) {
				dfs(n->middle, pq, v, nc); //traverse the left subtree
			}
		}
		if(n->getFreq()) {//if it is a word
			if(pq.size() < nc) {//if the queue is not full, then 
				//just add the string with no worries.
				string s = "";
				for(int i = 0; i < v.size(); i++) {
					s += v[i];
				}
				pair<int, string> p;
				p = make_pair(n->getFreq(), s); //add it to a pair
				pq.push(p);//push the word and frequency onto queue
			}
			else if(pq.top().first <= n->getFreq()) {//if queue=full
				string s = "";
				for(int i = 0; i < v.size(); i++) {
					s += v[i];
				}
				/*
				 *if the queue is full and the lowest frequency
				 in the queue is less than the current node,
				 then replace the lowest frequency string in the
				 queue with the new string
				 */
				if(pq.top().first < n->getFreq()) {
					pq.pop();
					pair<int, string> p;
					//add it to a pair
					p = make_pair(n->getFreq(), s); 
					//push the word and frequency onto queue
					pq.push(p);
				}
				/*
				 * if the lowest value in the queue is equal to
				 * the current string but the current string is
				 * alphabetically lower, then replace the top
				 * element of the queue with the new string.
				 */
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
		v.pop_back();//pop vector<char> when subtree is fully traversed
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
	//the for loop now only takes priority queue's size into account since
	//it was trimmed in the optimized dfs method
	for(int i = 0; i < k; i++) {
		if(!pq.empty()) {//don't cause segfault
			v.push_back(pq.top().second);
			pq.pop();
		}
	}
	return v;
}
/*
 * dfsUnderscores() accepts accepts a root node pointer, priority queue to hold
 * data from the dictionary trie, a string s representing the word traversed
 * in the dictionary trie, a string word representing the underscored word that
 * we are autocompleting, and an integer nc representing the number of 
 * autocompleted words that will be stored in the priority queue at a given .
 * time. The method works much like the original dfs(), but this function takes
 * the characters of the word to be autocompleted into account, depending on
 * whether the present character is an underscore, or if the current character
 * is less than, more than, or equal to the current node we are passing. Returns
 * void but returns a populated priority queue.
 */
void dfsUnderscores(TrieNode* n, 
		priority_queue<pair<int, string>, 
		vector<pair<int, string>> , Compare> & pq, 
		string s, string & word, int nc) {
	if(n) { //if node != NULL
		if(s.length() < word.length()) {//if our word is not autocompleteded
			//if the current char in word is an underscore...
			if(word[s.length()] == '_') {
				//traverse the left subtree to fill the underscore
				if(n->left) dfsUnderscores(n->left, pq, s, word, nc);
				//traverse the right subtree
				if(n->right) dfsUnderscores(n->right, pq, s, word, nc);
				//no more left or right subtree = add this node
				s += n->getChar();
				//if there is a middle subtree and if our 
				//traversed string is not as long as the word to
				//be autocompleted then..
				if(s.length() < word.length() && n->middle) {
					dfsUnderscores(n->middle, pq, s, word, nc);
				}
			}
			//if the char we are considering the word to be 
			//autocompleted is equal to the char in the node then..
			else if(word[s.length()] == n->getChar()) {
				s += n->getChar();//add teh char to the string
				//traverse the middle path under the same
				//conditions as before
				if(s.length() < word.length() && n->middle) {
					dfsUnderscores(n->middle, pq, s, word, nc);
				}
			}
			//else if the word's value is greater than the node's
			else if(n->getChar() < word[s.length()]) {
				//traverse right subtree
				if(n->right) dfsUnderscores(n->right, pq, s, word, nc);
			}
			//else if the word's value is less than the node's
			else if(word[s.length()] < n->getChar()) {
				//traverse the left subtree
				if(n->left) dfsUnderscores(n->left, pq, s, word, nc);
			}
		}
		//if our words are of equal length and the node whose char we
		//just added to our autocompleted string is a word...
		if(s.length() == word.length() && n->getFreq()) {
			//if our queue is smaller than the number of completions
			//then just add the data worry free
			if(pq.size() < nc) {					
				pair<int, string> p;
				p = make_pair(n->getFreq(), s); //add it to a pair
				pq.push(p);//push the word and frequency onto queue
			}	
			//else if our queue is full and the string of lowest
			//frequency has a frequency less than or equal to the 
			//current autocompleted string being considered...		
			else if(pq.top().first <= n->getFreq()) {
				//if our queue has a frequency less than this
				//one being considered, pop the low frequency
				//string from the queue and add this one
				if(pq.top().first < n->getFreq()) {
					pq.pop();
					pair<int, string> p;
					//add it to a pair
					p = make_pair(n->getFreq(), s); 
					//push the word and frequency onto queue
					pq.push(p);
				}
				//if the strings have the same frequcny, then
				//give the queue the one in alphabetical order
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
}
/*
 * predictUnderscores() accepts a string representing the underscored word to
 * be autocompleted, and an integer value representing the number of
 * autocompletions to present to the user. The method returns a vector<string>
 * structured filled with autocompleted strings in a min-heap fashion.
 */
std::vector<string> DictionaryTrie::predictUnderscores(
    string pattern, unsigned int numCompletions) {
	TrieNode * n = root;
 	priority_queue<pair<int, string>, vector<pair<int, string>> , Compare> pq; 

	if(pattern == "" || n == NULL) {//sanity test. 
		vector<string> v{};
		return v; //return empty string if tree is empty or pattern is
	}

	vector<char> cv = vector<char>();
	string b = "";
	dfsUnderscores(n, pq, b, pattern, numCompletions);

	vector<string> v = vector<string>();
	int k = pq.size();
	//since the priority queue is taylored to the number of completions, we
	//use a for loop with an upper bound equal to priority_queue.size().
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
