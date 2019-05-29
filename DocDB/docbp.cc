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
#include <map>
#include <unordered_map>
#include <vector>

using namespace std;

// ########## HELPER FUNCTIONS ##########



class Node {

private:

public:
    string keyword;
    string filename;
    unsigned int frequency;
 
    Node(string word, string name)
    : keyword(word)
    , filename(name)
    , frequency(1)
    {}

    ~Node() {};
    
};



// ########## HELPER FUNCTIONS ##########
/* sets a string to lowercase */
string lowercase(string s) {
    for(std::string::iterator it = s.begin(); it != s.end(); ++it) {
        if (*it >= 65 && *it <= 90)
            *it += 32;
    }
    return s;
}

/* checks if it contains only chars a-z && A-Z */
bool is_alpha(string s) {
    for(std::string::iterator it = s.begin(); it != s.end(); ++it) {
        if (!((*it >= 97 && *it <= 122) || (*it >= 65 && *it <= 90)))
            return false;
    }
    return true;
}

/* checks if it contains only chars a-z && A-Z */
bool is_valid_word(string s) {
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
void index_function(int argc, const char * argv[]) {
    
    map <string, vector<Node>> map;
    //    unordered_map<string, vector<Node>> map;
    
    
    for (int a=2; a<argc; ++a) {
        //        const char * filename = argv[a];
        
        string word;
        ifstream file;
        file.open(argv[a], ios::in);
        // file.open(argv[a], ios::in);
        if (!file){
            cerr << "There was an error opening the file " << argv[a] << endl;
            exit(1);
        }
        while(file >> word) {
            if (!is_valid_word(word))
                continue;
            
            word = lowercase(word);
            
            if (map.count(word) == 0) {
                //                string key(word);
                //                Node entry(word, argv[a], 1);
                Node entry(word, argv[a]);
//                cout << entry.frequency << endl;
                vector<Node> nodes = {entry};
                map.insert(pair<string, vector<Node>>(word, nodes));
                //                map.insert({key, nodes});
                
            }
            else {
                auto node = map.find(word);
                vector<Node> nodes = node->second;
                bool found = false;
//                for (vector<Node>::iterator it = nodes.begin(); it != nodes.end(); ++it) {
////                    cout << it->keyword <<it->filename << endl;
//                    if (it->filename == argv[a]) {
//                        cout << "before: " << it->frequency << endl;
//
//                        ++(*it).frequency;
//
//
//                        found = true;
//                        cout << "after: " << it->frequency << endl;
//                    }
//                }
                for (int i=0; i<nodes.size(); ++i) {
                    //                    cout << it->keyword <<it->filename << endl;
                    if (nodes[i].filename == argv[a]) {
                        cout << "before: " << nodes[i].frequency << endl;
//                        unsigned int * counter = &nodes[i].frequency;
//                        ++(*counter);
//                        nodes[i].frequency = *counter;
                        found = true;
                        cout << "after: " << nodes[i].frequency << endl;
                    }
                }
                
                if (!found) {
                    Node entry(word, argv[a]);
                    nodes.push_back(entry);
                }
                
                //                for (auto it = nodes.begin(); it != nodes.end(); ++it) {
                //                    cout << it->filename << endl;
                //
                ////                    if (it->filename == filename) {
                ////////                        cout << "before: " << it->frequency << endl;
                ////////                        int counter = it->frequency;
                ////////                        ++counter;
                ////////                        found = true;
                ////////                        cout << "after: " << it->frequency << endl;
                ////                    }
                ////
                ////                    cout << it->filename << endl;
                ////
                //                }
                //                vector<Node>::iterator it = find_if(nodes.begin(), nodes.end(), <#_Predicate __pred#>)
                
            }
            
        }
        
        file.close();
    }
    
    
    
    //    INDEX << "Some random test" << endl;
    //    INDEX.close();
    //    map.~multimap();
    
}


// ########## MAIN ##########
int main(int argc, const char * argv[]) {
    
    // Checking if there are enough commad line parameters
    if (argc < 3)
        return 0;
    
    if (check_arg(argv[1],"index")) {
        // TODO: INDEX FUNCTION
        index_function(argc, argv);
        
    }
    else if (check_arg(argv[1],"search")) {
        // TODO: SEARCH FUNCTION
        
    }
    
    
    return 0;
}