#ifndef FUNCEDITOR_H
#define FUNCEDITOR_H
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
#include <stack>

using namespace std;

vector<string> CommandList;
stack<vector<string>> History; 
stack<vector<string>> Redo;
bool FileSaved = true;
string split;

class Editor{  // Everyting related to editor: from displaying it to its functionality is done in this class
public:
    int Lines;
    vector<string> EditorContent;
    int LineLength;
    int EditorSize;
    int EditorDisplaySize;
    inline static int TopPointer = 1; // each line is 20 chars, this var points at a char of each line, at the beginning, let it point to the 1st character of every line
    // Whatever char the pointer points at, that char can be replacable at every line, if u want to add in a sentence, it can be added from that char
    inline static int ScrollPointer  = 1; // The editor consists of 40 lines, and the pointer points each line one at a time, acting as a vertical scroll for the editor
    inline static int start = 1;
    inline static int end = 1;
    string SavedFile;
    Editor(int Lines, vector<string> EditorContent, string SavedFile){
        this->EditorContent = EditorContent; //The vector where all the text is going to be stored
        this->Lines = Lines; //The total number of lines already taken in the editor
        this->LineLength = 20; //This is the max length possible in each line of the editor
        this->EditorSize = 40; //The total number of lines, the editor can take
        this->EditorDisplaySize = 10;
        this->SavedFile = SavedFile;
    }

    int NoLines(){
        return this->Lines;
    }

    string GetSavedFile(){
        return this->SavedFile;
    }

    void Display(){  // This func is to draw the board
        if (split[0] != 'u' && split[0] != 'a' && split[0] != 'w' && split[0] != 'd' && split[0] != 'c' && split[0] != 's'){
            History.push(this->EditorContent);
        }
        cout << "SIZE: " << History.size() << endl;
        string Pointer = "*";
        string LineSize;
        for(unsigned int i = 1; i <= this->LineLength; i++){
            if (i == TopPointer){
                cout << setfill(' ') << setw(5 + TopPointer) << Pointer << endl;
            }

            string strDup = to_string(i);

            if (strDup.length() > 1){
                LineSize.append(1, strDup[strDup.length()-1]);
            }
            else{
                LineSize.append(to_string(i));
            }
        }

        cout << setfill(' ') << setw(5) << "";
        cout << LineSize << endl;

        end = start + this->EditorDisplaySize;
        
        for (int i = start; (signed)i < end; i++){
            if (i == ScrollPointer){
                cout << Pointer << setfill(' ') << setw(4 - 1) << i;  // 1 as in the pointer width
            }
            else {
                 cout << setfill(' ') << setw(4) << i;
            }

            if (i <= this->Lines){
                cout << "|";
                // cout << this->EditorContent.size();
                cout << this->EditorContent[i-1];
            }
            else {
                cout << " ";
            }

            cout << "\n";
        }

        cout << setfill(' ') << setw(5) << "";
        cout << LineSize << endl;
    }

    vector<string> CommandSplitter(string command){ //This func takes in the commands entered
        bool see = true;
        if (command == ""){
            if (!CommandList.empty()){
                command = CommandList.back();
                see = true;
            }
            else {
                cout << "\nNo previous command.\n";
                see = false;
            }
        }
        if (see == true){
            for (unsigned int i = 0; i < command.size(); ++i){
                command[i] = tolower(command[i]);
            }
            string Rest;
            char* cmdDup = strdup(command.c_str());
            char* ch = strchr(cmdDup, ' ');
            if (ch != NULL){
                split = cmdDup;
                Rest = split.substr(ch - cmdDup + 1); 
                split = split.substr(0, ch - cmdDup);
            }
            else {
                split = cmdDup;
                Rest = "";
            }
            //char* split = strtok(cmdDup, " "); 
            //Rest = cmdDup;

            if (split == "q"){
                Exit();
            }
            else if(split == "save"){
                if (!Rest.empty()){
                    Save(Rest);
                }
                else {
                    Save(SavedFile);
                }
            }
            else if (split == "d" || split == "a"){
                if (!Rest.empty()){
                    HorizontalScroll(split, Rest);
                }
                else{
                    HorizontalScroll(split, "N");
                }
            }
            else if (split == "s" || split == "w"){
                if (!Rest.empty()){
                    VerticalScroll(split, Rest);
                }
                else{
                    VerticalScroll(split, "N");
                }
            }
            else if (split == "cp" || split == "copy"){
                if (!Rest.empty()){
                    Copy(Rest);
                }
                else {
                    cout << "What is the name of the file I should copy to?\n";
                    cout << "FileName: ";
                    getline(cin, Rest);
                    Copy(Rest);
                }
            }
            else if (split == "i"){
                if (!Rest.empty()){
                    Insertion(Rest);
                }
                else {
                    cerr << "You can just insert a space or nothing.\n";
                }
            }
            else if (split == "u"){
                if (History.size() > 1){
                    Undo();
                    this->EditorContent = History.top();
                }
                else {
                    cout << "Cannot undo. \n\n";
                }
                // cout << "SIZE: " << History.size() << endl;
            }
            else if (split == "r"){
                if (!Redo.empty()){
                    this->EditorContent = Redo.top();
                    Redo.pop();
                }
                else {
                    cout << "Cannot redo. \n\n";
                }
            }
            else {
                cerr << "Sorry, command not recognized! ;)\n";
            }
        
            CommandList.push_back(command);
        }
        return this->EditorContent;
    }

    void Exit(){ //The func is to exit the text editor
        string UserInput = "";
        if (FileSaved == false){
            cout << "\nYou have unaved changes.\n";
            cout << "\nAre you sure you want to quit.\n";
            cout << "(y or n): ";
            getline(cin, UserInput);
        }
        if (UserInput[0] == 'Y' || UserInput[0] == 'y' || UserInput.empty()){
            cout << endl << "GoodBye!\n";
            exit(0);
        }
    }

    void Save(string Rest){  //This func is to save all the contents in the text editor to a file
        ofstream outFS;
        outFS.open(Rest);

        if (!outFS.is_open()){
            cout << "\nUnable to save the file.\n";
        }

        else{
            for (unsigned int i = 0; i < this->EditorContent.size(); ++i){
                outFS << this->EditorContent[i] << "\r\n"; // "\r\n" provides a newline in ofstream
            }
            this->SavedFile = Rest;
            FileSaved = true;
            outFS.close();
        }
    }

    void Copy(string Rest){
        ofstream outFS;
        ofstream outFS_;
        outFS.open(Rest);

        if (!outFS.is_open()){
            cout << "\nUnable to copy the file to "<< Rest << "\n";
        }
    
        else{
            string userInput = "";
            if (FileSaved == false){
                cout << "The file you want to copy is not saved. Do you want to save it or not? (y or n): ";
                getline(cin, userInput);
            }
            if (userInput == "y" || userInput == "Y"){
                outFS_.open(SavedFile);
                for (unsigned int i = 0; i < this->EditorContent.size(); ++i){
                    outFS << this->EditorContent[i] << "\r\n"; // "\r\n" provides a newline in ofstream
                    outFS_ << this->EditorContent[i] << "\r\n";
                }
                outFS_.close();
            }

            outFS.close();
        }
    }

    void HorizontalScroll(string s, string rest){
        //string s(split);
        if (s[0] == 'd'){
            if (s.length() == 1){
                if (rest == "N"){
                    TopPointer += 1;
                }
                else {
                    TopPointer += stoi(rest);
                }
                if (TopPointer > 20){
                    TopPointer = 20;
                }
            }
        }
        else if (s[0] == 'a'){
            if (s.length() == 1){
                if (rest == "N"){
                    TopPointer -= 1;
                }
                else {
                    TopPointer -= stoi(rest);
                }
                if (TopPointer < 1){
                    TopPointer = 1;
                }
            }
        }
    }
    
    void VerticalScroll(string s, string rest){
        //string s(split);
        if (s == "s"){
            if (s.length() == 1){
                if (rest == "N"){
                    ScrollPointer += 1;
                }
                else {
                    ScrollPointer += stoi(rest);
                }
            }
            if (ScrollPointer > this->Lines){
                ScrollPointer = this->Lines;
                start = start + 1;
            }
            if (start > ScrollPointer){
                start = ScrollPointer;
            }
        }
        else if (s == "w"){
            if (s.length() == 1){
                if (rest == "N"){
                    ScrollPointer -= 1;
                }
                else {
                    ScrollPointer -= stoi(rest);
                }
            }
            if (ScrollPointer < 1){
                ScrollPointer = 1;
            }
            if (ScrollPointer < start){
                start = ScrollPointer;
            }
        }
    }

    void Insertion(string rest){ //This func is to insert the data provided in the command
        while (!Redo.empty()){
            Redo.pop();
        }
        string THELINE = this->EditorContent[ScrollPointer-1];  //This takes out the entire line which is selected for insertion
        string REMAINING = THELINE.substr(0, TopPointer - 1);

        THELINE = THELINE.substr(TopPointer-1);
        // cout << "THELINE: " << THELINE << endl;
        THELINE.replace(0, rest.length(), rest);
        // cout << "THELINE: " << THELINE << endl;
        // cout << "REMAINING: " << REMAINING << endl;

        if (!REMAINING.empty()){
            THELINE = THELINE.insert(0, REMAINING);
            // cout << "THELINE: " << THELINE << endl;
        }
        if (THELINE.length() > this->LineLength){
            bool IssueFixed = false;
            string section;
            int ActualPoint = ScrollPointer;
            string EmptyLine;
            int start = 0;
            int end = 20;
            bool endLoop = false;
            while (IssueFixed == false){
                if (ActualPoint > this->Lines){
                    EmptyLine.assign(20, ' ');
                    if (THELINE.length() >= 20){
                        section = THELINE.substr(start, end);
                        //start = start + 20;
                        //end = end + 20;
                    }
                    else {
                        end = THELINE.length();
                        section = THELINE.substr(0, end);
                        endLoop = true;
                    }
                    EmptyLine.replace(0, section.length(), section);
                    THELINE = THELINE.substr(end);
                    this->EditorContent.push_back(EmptyLine);
                    ++ActualPoint;
                    ++this->Lines;
                }
                else{
                    if (THELINE.length() >= 20){
                        section = THELINE.substr(start, end);
                        //start = start + 20;
                        //end = end + 20;
                    }
                    else {
                        end = THELINE.length();
                        section = THELINE.substr(0, end);
                        endLoop = true;
                    }
                    THELINE = THELINE.substr(end);
                    this->EditorContent[ActualPoint-1] = section;
                    ++ActualPoint;
                }
                if (endLoop == true){
                    IssueFixed = true;
                }
            }
        }
        else {
            this->EditorContent[ScrollPointer-1] = THELINE;
        }
        FileSaved = false;
    }

    void Undo(){
        //cout << "1: " << History.size() << endl;
        Redo.push(History.top());
        History.pop();
        //cout << "2: " << History.size() << endl;
    }
};

#endif