#include <ctype.h>
#include <list>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <string>


// User specifies max words on the command line. This is the upper limit of what they can specify.
#define MAX_MAX_WORDS 10


using std::list;
using std::set;
using std::string;


struct TreeNode
{
    TreeNode *children[26];
    char const *word;

    TreeNode() { memset(this, 0, sizeof(TreeNode)); }
};


// Globals
TreeNode        *g_root;
set <string>    g_solutions; // Each element of the set is a solution, eg "a a ring velodrome"
int             g_maxWords;


int GetLetterIndex(char c)
{
    int rv = tolower(c) - 'a';
    if (rv < 0 || rv >= 26) {
        rv = -1;
    }
    return rv;
}


int AddWord(TreeNode *node, char const *word)
{
    if (strlen(word) == 0)
        return 0;

    int numNodesCreated = 0;
    for (char const *c = word; *c != '\0'; c++) {
        if (*c == '\'') {
            continue;
        }
        int letterIndex = GetLetterIndex(*c);
        if (letterIndex < 0) {
            printf("Dictionary contains invalid word '%s'\n", word);
            exit(1);
        }

        if (!node->children[letterIndex]) {
            node->children[letterIndex] = new TreeNode;
            numNodesCreated++;
        }

        node = node->children[letterIndex];
    }

    node->word = strdup(word);

    return numNodesCreated;
}


bool LoadDictionary(char const *dictPath)
{
    FILE *dict = fopen(dictPath, "r");
    if (!dict) {
        printf("Couldn't open dictionary file '%s'\n", dictPath);
        return false;
    }

    g_root = new TreeNode;
    int numNodes = 1;
    int numWords = 0;

    while (!feof(dict)) {
        char wordBuf[128];
        fgets(wordBuf, 128, dict);
        char *newLine = strchr(wordBuf, '\n');
        if (newLine) {
            *newLine = '\0';
        }
        numNodes += AddWord(g_root, wordBuf);
        numWords++;
    }

    printf("Dictionary loaded\n%d words added\n\n", numWords);
    return true;
}


int NumLettersRemaining(int letterCounts[26])
{
    int num = 0;
    for (int i = 0; i < 26; i++) {
        num += letterCounts[i];
    }
    return num;
}


void AddSolution(char const *words[MAX_MAX_WORDS], int numWords)
{
    // Sort the specified solution
    list <char const *> sortedWords;
    for (int i = 0; i < numWords; i++) {
        list <char const *>::const_iterator j = sortedWords.begin();
        while (j != sortedWords.end() && strcmp(*j, words[i]) < 0) {
            j++;     
        }
        sortedWords.insert(j, 1, words[i]);
    }

    // Convert sorted list into a string
    string outString;
    for (int i = 0; i < numWords; i++) {
        outString.append(sortedWords.front());
        sortedWords.pop_front();
        outString.append(" ");
    }

    // Search to see if the solution already exists
    if (g_solutions.count(outString) == 0) {
        puts(outString.c_str());
        g_solutions.insert(outString);
    }
}


void WalkTree(TreeNode *node, int letterCounts[26], char const *words[], int wordCount)
{
    if (node->word) {
        if (NumLettersRemaining(letterCounts) == 0) {
            // Complete anagram found
            words[wordCount] = node->word;
            wordCount++;
            AddSolution(words, wordCount);
        }
        else if (wordCount < (g_maxWords - 1)) {
            // Anagram incomplete. Start tree walk again from root, but with
            // matching letters removed.
            words[wordCount] = node->word;
            wordCount++;
            WalkTree(g_root, letterCounts, words, wordCount);
            wordCount--;
        }
    }

    // Explore every child recursively
    for (int i = 0; i < 26; i++) {
        if (letterCounts[i] && node->children[i]) {
            letterCounts[i]--;
            WalkTree(node->children[i], letterCounts, words, wordCount);
            letterCounts[i]++;
        }
    }
}


int main(int argc, char const *argv[])
{
    //
    // Parse command line

    if (argc < 3) {
        puts("Usage: anagram <dict> <max words> <string>");
        puts("  dict      - path to dictionary file. File should be ASCII and one word per line.");
        puts("  max words - limit the maximum number of words in the solution.");
        puts("  string    - the string to find anagrams of. Must not contain spaces.");
        return 0;
    }

    g_maxWords = atoi(argv[2]);
    if (g_maxWords < 1 || g_maxWords > MAX_MAX_WORDS) {
        printf("Bad args: max words must be in range 1 to %d\n", MAX_MAX_WORDS);
        return -1;
    }

    
    //
    // Load Dictionary

    if (!LoadDictionary(argv[1]))
        return -1;

    //
    // Find anagrams

    char const *word = argv[3];
    
    int letterCounts[26];
    memset(letterCounts, 0, sizeof(int) * 26);
    for (char const *c = word; *c != '\0'; c++) {
        int letterIndex = GetLetterIndex(*c);
        if (letterIndex < 0) {
            printf("Word contains bad character\n");
            break;
        }

        letterCounts[letterIndex]++;
    }

    char const *words[MAX_MAX_WORDS];
    WalkTree(g_root, letterCounts, words, 0);

    printf("\nNum solutions = %d\n", g_solutions.size());
}
