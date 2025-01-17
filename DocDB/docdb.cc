//
//  docbd.cc
//  DocDB
//
//  Created by Susanna Ardigo'
//  Copyright © 2019 Susanna Ardigo'. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include <list>
#include <sstream>
using namespace std;

// ########## HELPER FUNCTIONS ##########



class Node {
    
public:
    unsigned file_id;
    mutable unsigned int frequency;
    
    Node(unsigned file_id)
    : file_id(file_id)
    , frequency(1)
    {}
    
    
    Node(unsigned file_id, unsigned int frequency)
    : file_id(file_id)
    , frequency(frequency)
    {}
    
    ~Node() {};
    
    bool operator == (const Node &n) const {
        return n.file_id == file_id;
    }
    
    bool operator<(const Node &n) const {
        return frequency > n.frequency;
    }
};


// ########## HELPER FUNCTIONS ##########
/* sets a string to lowercase */
void lowercase(string& s) {
    for(std::string::iterator it = s.begin(); it != s.end(); ++it) {
        if (*it >= 65 && *it <= 90)
            *it += 32;
    }
}

/* checks if it contains only chars a-z && A-Z */
bool is_alpha(const string& s) {
    for(std::string::const_iterator it = s.cbegin(); it != s.cend(); ++it) {
        if (!((*it >= 97 && *it <= 122) || (*it >= 65 && *it <= 90)))
            return false;
    }
    return true;
}

/* checks if its lengths is in between 3 and 30 and if contains only chars a-z && A-Z */
bool is_valid_word(string& s) {
    if (s.length()<3 || s.length()>30)
        return false;
    
    return is_alpha(s);
}




// ########## REQUESTED FUNCTIONS ##########
/* Reads the given files and creates an index in a file called INDEX. The index is a reverse index of words,
 meaning that the index maps each word to a list of files. A word is a maximal contigu- ous sequence of
 alphabetical characters adjacent to any non-alphabetical characters (or to the beginning or end of file).
 Only words that are between 3 and 30 characters in length are considered in the indexing and search */
void index_function(int argc, const char * argv[]) {
    
    unordered_map <string, list<Node> > map;
    unsigned file_id = 0;
    ofstream index("INDEX");
    index << argc -  2 << endl;
    
    for (; *argv; ++argv) {
        string word;
        ifstream file;
        
        file.open(*argv, ios::in);
        if (!file){
            cerr << "There was an error opening the file " << *argv << endl;
            exit(1);
        }
        index << string(*argv) << " ";
        
        Node node(file_id++);
        
        while (file >> word) {
            // replace non alpha chars
            replace(word.begin(), word.end(), '-', ' ');
            stringstream ss(word);
            string s;
            
            while (ss >> s) {
                if (!is_valid_word(s))
                    continue;
                lowercase(s);
                pair<unordered_map<string, list<Node> >::iterator, bool> insertion;
                insertion = map.insert(pair<string, list<Node> >(s, list<Node>()));
                if (insertion.second == false && insertion.first->second.back() == node) {
                    ++insertion.first->second.back().frequency;
                } else {
                    insertion.first->second.push_back(node);
                }
            }
        }
        file.close();
    }
    
    index << endl;
    for (unordered_map<string, list<Node> >::const_iterator it = map.cbegin(); it != map.cend(); ++it) {
        index << it->first << " " << it->second.size();
        for (list<Node>::const_iterator i = it->second.cbegin(); i != it->second.cend(); ++i)
            index << " " << i->file_id << " " << i->frequency;
        index << endl;
    }
    index.close();
}



/* Reads the INDEX file, creates a vector of filenames and an unordered_map of all the keywords */
void read_index(unordered_map <string, list<Node> >& map, vector<string>& file_names)
{
    ifstream index("INDEX");
    string word, file_name;
    unsigned int frequency, file_id, file_count;
    size_t len;
    index >> file_count;
    for (unsigned i = 0; i < file_count; ++i) {
        index >> file_name;
        file_names.push_back(file_name);
    }
    
    while (index >> word >> len) {
        pair<unordered_map<string, list<Node> >::iterator, bool> insertion;
        insertion = map.insert(pair<string, list<Node> >(word, list<Node>()));
        for (size_t i = 0; i < len; ++i) {
            index >>  file_id >> frequency;
            insertion.first->second.push_back(Node(file_id, frequency));
        }
    }
    
    index.close();
}



/* Given an unordered_map and char * of keywords returns a list of files that have all the keywords and the frequency  */
void intersection_function(unordered_map <string, list<Node> >& map, list<Node>& out, const char *argv[]){
    string s(*argv++);
    lowercase(s);
    unordered_map<string, list<Node> >::iterator entry = map.find(s);
    if (entry != map.end())
        for (list<Node>::const_iterator it = entry->second.cbegin(); it != entry->second.cend(); ++it)
            out.push_back(*it);
    
    while (*argv) {
        s = string(*argv++);
        if (!is_valid_word(s))
            continue;
        lowercase(s);
        list<Node> other;
        entry = map.find(s);
        if (entry != map.end())
            for (list<Node>::const_iterator it = entry->second.cbegin(); it != entry->second.cend(); ++it)
                other.push_back(*it);
        list<Node>::iterator first1 = out.begin();
        for (list<Node>::iterator first2 = other.begin(); first1 != out.end() && first2 != other.end();) {
            if (first1->file_id > first2->file_id) {
                ++first2;
            }
            else if (first2->file_id > first1->file_id) {
                first1 = out.erase(first1);
            }
            else {
                first1->frequency += first2->frequency;
                ++first1;
                ++first2;
            }
        }
        out.erase(first1, out.end());
    }
}



/* Reads the index from a file called INDEX and outputs, on the standard output, a list of all the
 files (filenames) that contain all the keywords passed on the command line. The keyword match must
 be case-insensitive. If no indexed file contains all the given keywords, the program does not output
 anything. The names of the matching files must be printed in the original order passed to the indexing
 function. The search function may only read the index file. */
void search_function(const char *argv[]){
    unordered_map <string, list<Node> > map;
    vector<string> file_names;
    list<Node> out;
    read_index(map, file_names);
    intersection_function(map, out, argv);
    for (list<Node>::const_iterator it = out.cbegin(); it != out.cend(); ++it)
        cout << file_names[it->file_id] << endl;
}


/* In this case, the program runs the search function as search_function, except that, instead of sorting the output files in the original indexing order, it outputs the matching files in decreasing order of total count of matching words. */
void searchP_function(const char *argv[]){
    unordered_map <string, list<Node> > map;
    vector<string> file_names;
    list<Node> out;
    read_index(map, file_names);
    intersection_function(map, out, argv);
    out.sort();
    for (list<Node>::const_iterator it = out.cbegin(); it != out.cend(); ++it)
        cout << file_names[it->file_id] << endl;
}



// ########## MAIN ##########
int main(int argc, const char * argv[]) {
    
    // Checking if there are enough commad line parameters
    if (argc < 3)
        return 0;
    
    if (strcmp(argv[1],"index") == 0)
        index_function(argc, argv + 2);
        
    else if (strcmp(argv[1],"search") == 0) {
        if (strcmp(argv[2],"-p") == 0)
            searchP_function(argv + 3);
        else
            search_function(argv + 2);
        
    }
    
    return 0;
}
