// creates all functions needed to successfully implement a search query
// each function has a header above to define what it does
// this is included in main.cpp to make everything look cleaner

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;

// takes in a string and returns the string without any leading
// or trailing punctuation. Punctuation is allowed in the middle
// of the string
string cleanToken(string s) {

    // gets first index that isn't a punctuation
    size_t start = 0;
    while (start < s.length() && ispunct(s[start])) {
        start++;
    }

    // gets last index that isn't a punctuation
    size_t end = s.length() - 1;
    while (end >= start && ispunct(s[end])) {
        end--;
    }

    bool containsLetter = false;
    for (int i = start; i <= end; i++) {   
        if (isalpha(s[i])) {
            s[i] = tolower(s[i]);
            containsLetter = true;
        }
    }

    if (!containsLetter) {
        return "";
    }

    return s.substr(start, end - start + 1);
}

//goes through the text and stores all cleaned up and unique words found in text
// and stores it in a set. The function will then return that set
set<string> gatherTokens(string text) {
    set<string> tokens;

    istringstream inputStream(text);
    string eachToken;

    while (inputStream >> eachToken) {
        string tokenClean = cleanToken(eachToken);
        if (!tokenClean.empty()) {
            tokens.insert(tokenClean);
        }
    }

    return tokens;

}

// builds index of tokens associated with URLs
// takes in the filename and map to store tokens and URLs as key-value pairs
// returns number of pairs 
int buildIndex(string filename, map<string, set<string>>& index) {

    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return 0;
    }

    int documentsProcessed = 0;

    string line, url, content;
    bool validUrl = true;

    // checks until end of file
    while (getline(inputFile, line)) {
        // makes sure line is the url line, not the url that is inside the content
        if(validUrl) {
            url = line;
        }
        else {
            // for lines with content, calls gatherTokens function
            content = line;
            set<string> newTokens = gatherTokens(content);

            // sets key-value pairs 
            for (const string& token : newTokens) {
                index[token].insert(url);
            }

            documentsProcessed++;
        }

        // toggles the lines for url and content
        validUrl = !validUrl;
    }
    inputFile.close();

    return documentsProcessed;
}

// finds matches for search query in the index
// returns set of URLs that match the query
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    istringstream queries(sentence);

    string term;
    char modifier;
    set<string> matches;

    // gets all queries and sets the modifiers
    while (queries >> term) {
        if (term[0] == '+') {
            modifier = '+';
            term = term.substr(1);
        }
        else if (term[0] == '-') {
            modifier = '-';
            term = term.substr(1);
        }
        else {
            modifier = ' ';
        }

        term = cleanToken(term);

        // gets back URLs that correspond to cleaned term
        set<string> matchingTerms = index[term];

        // handles intersection of current matches
        if (modifier == '+') {
            set<string> intersect;
            set_intersection(matches.begin(), matches.end(), matchingTerms.begin(), matchingTerms.end(), inserter(intersect, intersect.begin()));
            matches = intersect;
        }
        // handles differences with current matches
        else if (modifier == '-') {
            set<string> diff;
            set_difference(matches.begin(), matches.end(), matchingTerms.begin(), matchingTerms.end(), inserter(diff, diff.begin()));
            matches = diff;
        }
        // sets default to the union of current matches
        else {
            set<string> combinedResult;
            set_union(matches.begin(), matches.end(), matchingTerms.begin(), matchingTerms.end(), inserter(combinedResult, combinedResult.begin()));
            matches = combinedResult;
        }
    }
      
    return matches; 
}

// parameter- name of file to search through
// builds an inverted index and prints number of pages processed
// finds matching pages and URLS to search query
void searchEngine(string filename) {

    // creates map for inverted index
    map<string, set<string>> index;

    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << buildIndex(filename, index) << " pages containing " << index.size() << " unique terms" << endl;
    
    cout << endl << "Enter query sentence (press enter to quit): " << endl;
    string userQuery;

    getline(cin, userQuery);
    while (userQuery != "") {
        // finds all matching URLs to the userQuery
        set<string> matchingUrls = findQueryMatches(index, userQuery);
        
        cout << "Found " << matchingUrls.size() << " matching pages" << endl;

        // prints all URLs out that matched with the search query
        for (const string& url : matchingUrls) {    
            cout << url << endl;
        }

        cout << endl << "Enter query sentence (press enter to quit): " << endl;
        getline(cin, userQuery);
    }
    cout << "Thank you for searching!" << endl;
}