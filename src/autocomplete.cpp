/* Forest Jenkins
 * A14747597
 * fjenkins@ucsd.edu
 * sources: piazza (peer and personal questions), lecture and discussion
 * 	slides, class textbook, Focus on Data Structures by Richard L. 
 * 	Stegman, cppreference.com for insight into pairs, priority queues,
 * 	strings, and their functions. 
 * 
 * autocomplete.cpp is the main file of the program. The program mimics the 
 * text autocomplete feature that one would encounter in a search engine such as
 * google. autocomplete populates a ternary trie data structure from the file 
 * DictionaryTrie.cpp with entries from a dictionary file, which the structure
 * then sorts in alphabetical order. When the user is probed for a prefix, the
 * file then connects to the DictionaryTrie's predictCompletions function in the
 * DictoinaryTrie file. Said function populates a vector<string> structure with
 * autocompleted strings and then returns them here. 
 */
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "DictionaryTrie.hpp"
#include "util.hpp"

using namespace std;

/* Check if a given data file is valid */
bool fileValid(const char* fileName) {
    ifstream in;
    in.open(fileName, ios::binary);

    // Check if input file was actually opened
    if (!in.is_open()) {
        cout << "Invalid input file. No file was opened. Please try again.\n";
        return false;
    }

    // Check for empty file
    in.seekg(0, ios_base::end);
    unsigned int len = in.tellg();
    if (len == 0) {
        cout << "The file is empty. \n";
        return false;
    }
    in.close();
    return true;
}

/* IMPORTANT! You should use the following lines of code to match the correct
 * output:
 *
 * cout << "This program needs exactly one argument!" << endl;
 * cout << "Reading file: " << file << endl;
 * cout << "Enter a prefix/pattern to search for:" << endl;
 * cout << "Enter a number of completions:" << endl;
 * cout << completion << endl;
 * cout << "Continue? (y/n)" << endl;
 *
 * arg 1 - Input file name (in format like freq_dict.txt)
 */
int main(int argc, char** argv) {
    const int NUM_ARG = 2;
    if (argc != NUM_ARG) {
        cout << "Invalid number of arguments.\n"
             << "Usage: ./autocomplete <dictionary filename>" << endl;
        return -1;
    }
    if (!fileValid(argv[1])) return -1;

    DictionaryTrie* dt = new DictionaryTrie();

    // Read all the tokens of the file in order to get every word
    cout << "Reading file: " << argv[1] << endl;

    ifstream in;
    in.open(argv[1], ios::binary);
    string word;

    Utils::loadDict(*dt, in);
    in.close();

    char cont = 'y';
    unsigned int numberOfCompletions;
    vector<string> v = vector<string>();
    bool underscore;
    while (cont == 'y') {
	underscore = false;
        cout << "Enter a prefix/pattern to search for:" << endl;
        getline(cin, word);
        cout << "Enter a number of completions:" << endl;
        cin >> numberOfCompletions;
	for(int i = 0; i < word.length(); i++) {
		if(word[i] == '_') {
			underscore = true;
			break;
		}
	}
	if(underscore == false) {
		v = dt->predictCompletions(word, numberOfCompletions);
		for(int i = v.size()-1; i >= 0; --i) {
			cout<<v[i]<<endl;
		}
	}
	else {
		v = dt->predictUnderscores(word, numberOfCompletions);
		for(int i = v.size()-1; i >= 0; --i) {
			cout<<v[i]<<endl;
		}
	}
        cout << "Continue? (y/n)" << endl;
        cin >> cont;
        cin.ignore();
    }
    dt->~DictionaryTrie();
    delete dt;
    return 0;
}
