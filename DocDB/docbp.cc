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
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <list>

using namespace std;

// ########## HELPER FUNCTIONS ##########



class Node {
    
public:
    string filename;
    mutable unsigned int frequency;
    
    Node(string name)
    : filename(name)
    , frequency(1)
    {}
    
    ~Node() {};
    
    bool operator == (const Node &n) const {
        return n.filename == filename;
    }
};

//namespace std {

//    template <>
//    struct hash<Node>
//    {
//        std::size_t operator()(const Node& k) const
//        {
//           return hash<string> () (*(k.filename));
//        }
//    };

//}



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

/* checks if it contains only chars a-z && A-Z */
bool is_valid_word(string& s) {
    if (s.length()<3 || s.length()>30)
        return false;
    
    return is_alpha(s);
}

// Checks if two char * are equal
int check_arg(const char * arg, const char * word) {
    
    int i=0;
    while(isalpha(arg[i]) && isalpha(word[i])) {
        if (arg[i] != word[i])
            return 0;
        
        ++i;
    }
    
    return 1;
}



// ########## INDEXING FUNCTION ##########
// Creates a reverse index of the words in the given files and saves it in a file INDEX
void index_function(const char * argv[]) {
    
    vector<string> filenames;
    unordered_map <string, list<Node>> map;
    
    for (; *argv; ++argv) {
        filenames.push_back(string(*argv));
    }
    
    for (vector<string>::iterator it = filenames.begin(); it != filenames.end(); ++it) {
        string word;
        ifstream file;
        
        file.open(*it, ios::in);
        if (!file){
            cerr << "There was an error opening the file " << *it << endl;
            exit(1);
        }
        
        Node node(*(it));
        
        while (file >> word) {
            // TODO: split word on non alphabetical chars
            if (!is_valid_word(word))
                continue;
            
            lowercase(word);
            
            
            pair<unordered_map<string, list<Node>>::iterator, bool> insertion;
            insertion = map.insert(pair<string, list<Node>>(word, list<Node>()));
            
            //            auto insertion = map[word].insert(node);
            if (insertion.second == false && insertion.first->second.back().filename == node.filename) {
                ++insertion.first->second.back().frequency;
            } else {
                insertion.first->second.push_back(node);
            }
        }
        
        
        file.close();
    }
    
    ofstream index("INDEX");
    
    for (unordered_map<string, list<Node>>::const_iterator it = map.cbegin(); it != map.cend(); ++it) {
        cout << (*it).first << endl;
    }
    
    index.close();
    
    
    
    
}


// ########## MAIN ##########
int main(int argc, const char * argv[]) {
    
    // Checking if there are enough commad line parameters
    if (argc < 3)
        return 0;
    
    if (check_arg(argv[1],"index")) {
        // TODO: INDEX FUNCTION
        index_function(argv + 2);
        
    }
    else if (check_arg(argv[1],"search")) {
        // TODO: SEARCH FUNCTION
        
    }
    
    
    return 0;
}
