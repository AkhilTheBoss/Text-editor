#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include "FuncEditor.h"

using namespace std;

string SavedFile;

void CinCleanup(){
    fflush(stdin);
}

void CoutCleanup(){
    fflush(stdout);
}

void Commands(int line_count, vector<string> FileInfo){ // This func is what links with this page and FuncEditor.h
    string command;
    bool Continue = true;
    while (Continue){
        Editor *edit = new Editor(line_count, FileInfo, SavedFile);
        edit->Display();
        cout << "Enter command: ";
        CinCleanup();
        getline(cin, command);
        FileInfo = edit->CommandSplitter(command);
        line_count = edit->NoLines();
        SavedFile = edit->GetSavedFile();
    }
}

void InputFileRead(string FileName){ // This func will read all the content from the file specified in the command line arg

    ifstream inFS;
    string Line;
    vector<string> FileInfo;
    int line_count = 0;
    inFS.open(FileName);
    string Content;

    if (!inFS.is_open()){
        cerr << "Unable to open the file: " << FileName << endl;
        exit(-1);
    }
    
    while (getline(inFS, Line)){
        ++line_count;
        // Content.assign(20, ' ');
        if (Line.length() > 20){
            Line = Line.substr(0, 20);
        }
        else {
            Line.resize(20);
        }
        CoutCleanup();
        //cout << "Line1: " << Line << endl;
        // Content.replace(0, Line.length(), Line);
        // cout << "Content: " << Content << endl;
        if (Line.length() < 20){
            // cout << "LESS\n";
            //string spaces = (20- Line.length(), " ");
            //Line.assign(" ", 20);
            CoutCleanup();
            //cout << "Line2: " << Line << endl;
            // Line.append(5, ' ');
            // Line.append(" ");
            CoutCleanup();
            //cout << "Line3: " << Line << endl;
            // cout << "LineLength: " << Line.length() << endl;
        }
        else {
            // cout << "MORE\n";
        }
        CoutCleanup();
        //cout << "Line4: " << Line << endl;
        Content = Line;
        // cout << Line << endl;
        FileInfo.push_back(Content);
        //cout << "CONTENT: " << Content << endl;
    }
    inFS.close();

    Commands(line_count, FileInfo);
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cerr << "Too few command-line args\n";
        return -1;
    }
    if (argc > 2){
        cerr << "Too many command-line args\n";
        return -2;
    }
    SavedFile = argv[1];
    InputFileRead(argv[1]);
    return 0;
}