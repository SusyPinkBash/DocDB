//
//  docbd.cc1
//  DocDB
//
//  Created by Susanna Ardigo'
//  Copyright © 2019 Susanna Ardigo'. All rights reserved.
//


#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdlib>

using namespace std;

// ########## HELPER FUNCTIONS ##########



class Entry {
public:
    string * keyword;
    const char * filename;
    unsigned int frequency;
    
    Entry(string * keyword, const char * filename, unsigned int frequency) {
        keyword = keyword;
        filename = filename;
        frequency = frequency;
    };
    
    ~Entry();
    
private:
    
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

/* changes a string to lowercase */
bool is_alpha(string s) {
    for(std::string::iterator it = s.begin(); it != s.end(); ++it) {
        if (!((*it >= 97 && *it <= 122) || (*it >= 65 && *it <= 90)))
            return false;
    }
    return true;
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
    
    
    for (int a=2; a<argc; ++a) {
        const char * filename = argv[a];
        
        string s;
        ifstream file;
        file.open(filename, ios::in);
        // file.open(argv[a], ios::in);
        if (!file){
            cerr << "There was an error opening the file " << filename << endl;
            exit(1);
        }
        while(file >> s) {
            
            if (is_alpha(s)) {
                s = lowercase(s);
                cout << s << endl;
                
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
