// Shaan Kohli
// This program integrates a search query, where all matching URLs are shown to a user inputted query
// All of the implementation is done through search.h

#include "search.h"
using namespace std;

// PROTOTYPES
bool testCleanToken();
bool testGatherTokens();
bool testBuildIndex();
bool testFindQueryMatches();
bool testSearchEngine();

int main() {
   
    string filename;
    getline(cin, filename);
    searchEngine(filename);
    
    return 0;
}

// Tries a few different strings and makes sure it cleans it properly(removes leading and trailing punct)
bool testCleanToken() {

    string ans1 = "hello";
    string ans2 = "he.l.l??o";

    int pass = 0, fail = 0;

    ans1 == cleanToken("hello") ? ++pass : ++fail;
    ans1 == cleanToken("hello...") ? ++pass : ++fail;
    ans1 == cleanToken("...!hello....") ? ++pass : ++fail;
    ans2 == cleanToken("...he.l.l??o") ? ++pass : ++fail;
    ans2 == cleanToken("...he.l.l??o...!!") ? ++pass : ++fail;

    return 0 == fail;
}

// makes sure gatherTokens properly gets all unique words
bool testGatherTokens() {

    int pass = 0, fail = 0;

    set<string> tokens = gatherTokens("to be or not to be");
    set<string> answers = {"to", "be", "or", "not"};
    tokens.size() == answers.size() ? ++pass : ++fail;
    tokens == answers ? ++pass : ++fail;

    set<string> tokens1 = gatherTokens("there once was a bear that there is a bear who knows a bear");
    set<string> answers1 = {"there", "once", "was", "a", "bear", "that", "is", "who", "knows"};
    tokens1.size() == answers1.size() ? ++pass : ++fail;
    tokens1 == answers1 ? ++pass : ++fail;

    set<string> tokens2 = gatherTokens("how how how how who who who is how who is many");
    set<string> answers2 = {"how", "who", "is", "many"};
    tokens2.size() == answers2.size() ? ++pass : ++fail;
    tokens2 == answers2 ? ++pass : ++fail;

    return 0 == fail;
}

// makes sure buildIndex gets the proper amount of indexed pages and unique words
bool testBuildIndex() {

    string file1 = "cplusplus.txt";
    string file2 = "stackoverflow.txt";
    string file3 = "tiny.txt";
    string file4 = "uiccs-news.txt";
    string file5 = "wiki-uni.txt";

    map<string, set<string>> index1;
    map<string, set<string>> index2;
    map<string, set<string>> index3;
    map<string, set<string>> index4;
    map<string, set<string>> index5;

    int docsProcessed1 = buildIndex(file1, index1);
    int docsProcessed2 = buildIndex(file2, index2);
    int docsProcessed3 = buildIndex(file3, index3);
    int docsProcessed4 = buildIndex(file4, index4);
    int docsProcessed5 = buildIndex(file5, index5);

    if (docsProcessed1 == 86 && index1.size() == 1498 && docsProcessed2 == 13 && index2.size() == 384 && docsProcessed3 == 4 && index3.size() == 20 && docsProcessed4 == 85 && index4.size() == 7032 && docsProcessed5 == 148 && index5.size() == 1874) {
        return true;
    }

    return false;
}

// makes sure findQueryMatches handles modifiers correctly
bool testFindQueryMatches() {
    map<string, set<string>> index;

    index["tasty"].insert("url1");
    index["healthy"].insert("url1");
    index["mushrooms"].insert("url2");
    index["simple"].insert("url3");
    index["cheap"].insert("url3");

    string sentence1 = "tasty +healthy";
    set<string> matches1 = findQueryMatches(index, sentence1);

    string sentence2 = "tasty -mushrooms";
    set<string> matches2 = findQueryMatches(index, sentence2);

    string sentence4 = "tasty +healthy +simple +cheap";
    set<string> matches4 = findQueryMatches(index, sentence4);
    
    if (matches1.size() == 1 && matches2.size() == 1 && matches4.size() == 0) {
        return true;
    }

    return false;
}

// Puts everything together to see if all functions work and gets back proper list of URLs 
bool testSearchEngine() {

    string file1 = "cplusplus.txt";

    map<string, set<string>> index;

    buildIndex(file1, index);
  
    string userQuery1 = "vector";
    string userQuery2 = "vector +container";
    string userQuery3 = "vector +container -pointer";

    set<string> myResult1 = findQueryMatches(index, userQuery1);
    set<string> myResult2 = findQueryMatches(index, userQuery2);
    set<string> myResult3 = findQueryMatches(index, userQuery3);

    set<string> expected1 = {"https://www.cplusplus.com/reference/array/array/", "https://www.cplusplus.com/reference/bitset/bitset/", "https://www.cplusplus.com/reference/forward_list/forward_list/","https://www.cplusplus.com/reference/list/list/", "https://www.cplusplus.com/reference/queue/priority_queue/", "https://www.cplusplus.com/reference/stack/stack/", "https://www.cplusplus.com/reference/vector/vector-bool/", "https://www.cplusplus.com/reference/vector/vector/"};
    set<string> expected2 = {"https://www.cplusplus.com/reference/array/array/", "https://www.cplusplus.com/reference/forward_list/forward_list/", "https://www.cplusplus.com/reference/list/list/", "https://www.cplusplus.com/reference/queue/priority_queue/", "https://www.cplusplus.com/reference/stack/stack/", "https://www.cplusplus.com/reference/vector/vector-bool/", "https://www.cplusplus.com/reference/vector/vector/"};
    set<string> expected3 = {"https://www.cplusplus.com/reference/array/array/", "https://www.cplusplus.com/reference/forward_list/forward_list/", "https://www.cplusplus.com/reference/list/list/", "https://www.cplusplus.com/reference/queue/priority_queue/", "https://www.cplusplus.com/reference/stack/stack/", "https://www.cplusplus.com/reference/vector/vector/"};

    if (myResult1 == expected1 && myResult2 == expected2 && myResult3 == expected3) {
        return true;
    }
    return false;
}