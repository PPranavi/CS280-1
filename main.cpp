//
//  main.cpp
//  Project1
//
//  Created by Pranavi Parsi on 9/22/19.
//  Copyright © 2019 Pranavi Parsi. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
using namespace std;

int main(int argc, char*argv[]) {
    // if there are no initial commands
    if (argv[1]==NULL)
        cout << "MISSING COMMAND" << endl;
    
    // if a command is entered
    else {
        // if the first command is to use rot13 substitution
        if (strcmp(argv[1], "-r") == 0) {
            
            // a "dictionary" with upper case letters
            char arrUpper[26];
            for (int i=0; i<26; i++) {
                arrUpper[i] = char(65+i);
            }
            
            // a "dictionary" with lower case letters
            char arrLower[26];
            for (int i=0; i<26; i++) {
                arrLower[i] = char(97+i);
            }
            
            // if input is to be read from a file
            if (argv[2]!=NULL) {
                string file_name = argv[2];
                ifstream file;
                file.open(file_name);
                // checks to see if file will open
                if (!file.is_open()) {
                    cout << file_name + " FILE COULD NOT BE OPENED" << endl;
                }
                // if file can be opened
                else if (file.is_open()) {
                    string phrase;
                    // gets the input from file line by line
                    while (getline(file, phrase)) {
                        string message = "";
                        // parses through every character of the line
                        for(int i=0; i<phrase.length(); i++) {
                            int index = 0;
                            // checks to see if character is uppercase
                            if ((int)phrase[i]>64 && (int)phrase[i]<91) {
                                index = (int)phrase[i]-65;
                                message += arrUpper[((index+13)%26)];
                            }
                            // checks to see if character is lowercase
                            else if ((int)phrase[i]>96 && (int)phrase[i]<123) {
                                index = (int)phrase[i]-97;
                                message += arrLower[((index+13)%26)];
                            }
                            // the character is not a letter
                            else
                                message += phrase[i];
                        }
                        cout << message << endl;
                    }
                    file.close();
                }
            }
            
            // if input is read from standard input
            else if (argv[2]==NULL){
                // input reads an entire line
                string phrase;
                // while there is a line to be read
                while (getline(cin, phrase) || !cin.eof()) {
                    string message = "";
                    // parses through every character of the line
                    for(int i=0; i<phrase.length(); i++) {
                        int index = 0;
                        // checks to see if character is uppercase
                        if ((int)phrase[i]>64 && (int)phrase[i]<91) {
                            index = (int)phrase[i]-65;
                            message += arrUpper[((index+13)%26)];
                        }
                        // checks to see if character is lowercase
                        else if ((int)phrase[i]>96 && (int)phrase[i]<123) {
                            index = (int)phrase[i]-97;
                            message += arrLower[((index+13)%26)];
                        }
                        // the character is not a letter
                        else
                            message += phrase[i];
                    }
                    cout << message << endl;
                }
            }
            
            //if there are more than three arguments passed into main (includes name of file)
            if (argc>3)
                cout << "TOO MANY ARGUMENTS" << endl;
        }
        //generates a random dictionary
        else if (strcmp(argv[1], "-g") == 0) {
            if (argc>2)
                cout << "TOO MANY ARGUMENTS" << endl;
            else {
                //generates array with pseduo ascii code indexes
                int arr[26];
                int arrsize= 26;
                for(int i=0; i<26; i++ )
                    arr[i] = i;

                srand(time(NULL) );
                for(int i=0; i<26; i++ ) {
                    int val=0; // holds a random index number between 0 and array size-1 inclusive
                    char letter = 0;
                    char c = 0;
                    while (c==letter){
                        val = rand()%arrsize;
                        letter = (97+i);
                        c = 97+arr[val];
                    }
                    cout << string(1,letter) + c << endl;
                    arr[val] = arr[arrsize-1]; // removes that index element from the list of index elements
                    arrsize--;
                }
                cout << endl;
            }
        }
        
        //to encrypt a message using the ceasar cipher
        else if (strcmp(argv[1], "-e") == 0) {
            char arrLower[26];
            char arrUpper[26];
            // checks to see if a dictionary file was given
            if (argv[2]==NULL)
                cout << "NO DICTIONARY GIVEN" << endl;
            else if (argv[2]!=NULL) {
                string dictionary = argv[2];
                ifstream file;
                file.open(dictionary);
                
                // checks to see if file will open
                if (!file.is_open()) {
                    cout << dictionary + " DICTIONARY COULD NOT BE OPENED" << endl;
                    return EXIT_SUCCESS;
                }
                // if file can open
                else if (file.is_open()) {
                    char exists[26]; // an array which holds cipher text letters initiallized to null
                    for (int i=0; i<26; i++) {
                        exists[i]=(char)0;
                    }
                    
                    int index=0;
                    string phrase;
                    int i=0;
                    while (!file.eof() && i<26) {
                        // accesses plaintext letter
                        file >> phrase;
                        istringstream iss (phrase);
                        char character1 = ' ';
                        char character2 = ' ';
                        iss >> noskipws;
                        iss >> character1 >> ws >> character2;
                        
                        // int value of ciphertext letter
                        index = ((int)character2)-97;
                        
                        // checks to see if every pair has two letters
                        if (character2 == ' ') {
                            string output = "FORMATTING ERROR ";
                            output += character1;
                            cout <<  output  << endl;
                            return EXIT_SUCCESS;
                        }
                        // checks to ensure plaintext letter isn't matched to itself in dictionary
                        else if (character1 == character2) {
                            string output = "MAPPING ERROR ";
                            output += character1;
                            cout <<  output  << endl;
                            return EXIT_SUCCESS;
                        }
                        // checks to see if ciphertext letter already ecxists
                        else if (exists[index]!=0) {
                            char c = character2;
                            string output = "DUPLICATE CIPHERTEXT ";
                            output+=c;
                            cout << output << endl;
                            return EXIT_SUCCESS;
                        }
                        // ensures that ciphertext doesnt exists and checks for alphabetical order
                        else if (exists[index]==0 && character1==char(97+i)) {
                            arrLower[i] = character2; // copies ciphertext letter to plaintext letter position
                            exists[index] = character2;
                            i++;
                        }
                        // if out of alphabetical order, prints that letter
                        else if (character1!=char(97+i)) {
                            char c = char(97+i);
                            string output = "MISSING LETTER ";
                            output+=c;
                            cout << output << endl;
                            return EXIT_SUCCESS;
                        }
                    }
                    file.close();
                    
                    // changes all lower case letter to upper case and stores in new list
                    for (int i=0; i<26; i++) {
                        arrUpper[i] = (arrLower[i] - 32);
                    }
                }
                
                if (argv[3]!=NULL) {
                    string file_name = argv[3];
                    ifstream file;
                    file.open(file_name);
                    
                    // checks to see if file can be opened
                    if (!file.is_open()) {
                        cout << file_name + " FILE COULD NOT BE OPENED" << endl;
                    }
                    // in the open file
                    else if (file.is_open()) {
                        string phrase;
                        // each line is a phrase
                        while (getline(file, phrase)) {
                            string message = "";
                            // goes through every character in the phrase one by one
                            for(int i=0; i<phrase.length(); i++) {
                                int index = 0;
                                // if the letter is a capital letter, then the upper list is used for encryption
                                if ((int)phrase[i]>64 && (int)phrase[i]<91) {
                                    index = (int)phrase[i]-65;
                                    message += arrUpper[index];
                                }
                                // if the letter is a lowercase letter, then the lower list is used for encryption
                                else if ((int)phrase[i]>96 && (int)phrase[i]<123) {
                                    index = (int)phrase[i]-97;
                                    message += arrLower[index];
                                }
                                // if the character is not a letter, it is simply added to the message
                                else
                                    message += phrase[i];
                             }
                             cout << message << endl;
                         }
                         file.close();
                    }
                }
                else if (argv[3]==NULL){
                    // input reads an entire line
                    string phrase;
                    // while there is a line to be read
                    while (getline(cin, phrase) || !cin.eof()) {
                        string message = "";
                        // parses through every character of the line
                        for(int i=0; i<phrase.length(); i++) {
                            int index = 0;
                            // checks to see if character is uppercase
                            if ((int)phrase[i]>64 && (int)phrase[i]<91) {
                                index = (int)phrase[i]-65;
                                message += arrUpper[index];
                            }
                            // checks to see if character is lowercase
                            else if ((int)phrase[i]>96 && (int)phrase[i]<123) {
                                     index = (int)phrase[i]-97;
                                     message += arrLower[index];
                                 }
                                 // the character is not a letter
                                 else
                                     message += phrase[i];
                             }
                        cout << message << endl;
                    }
                }
            }
            
            if (argc>4)
                cout << "TOO MANY ARGUMENTS" << endl;
        }
        
        //to decrypt a message using the ceasar cipher
        else if (strcmp(argv[1], "-d") == 0) {
            char arrLower[26];
            char arrUpper[26];
            // checks to see if a dictionary file was given
            if (argv[2]==NULL)
                cout << "NO DICTIONARY GIVEN" << endl;
            else if (argv[2]!=NULL) {
                string dictionary = argv[2];
                ifstream file;
                file.open(dictionary);
                
                // if the dictionary cannot be opened
                if (!file.is_open()) {
                    cout << dictionary + " DICTIONARY COULD NOT BE OPENED" << endl;
                    return EXIT_SUCCESS;
                }
                // from the open dictionary
                else if (file.is_open()) {
                    char exists[26]; // stores existsing ciphertext letters
                    for (int i=0; i<26; i++) {
                        exists[i]=(char)0;
                    }
                    int index=0;
                    string phrase;
                    int i=0;
                    while (!file.eof() && i<26) {
                        // accesses plaintext letter
                        file >> phrase;
                        istringstream iss (phrase);
                        char character1 = ' ';
                        char character2 = ' ';
                        iss >> noskipws;
                        iss >> character1 >> ws >> character2;
                        
                        index = ((int)character2)-97;
                        // checks to see if every pair has two letters
                        // checks to see if every pair has two letters
                        if (character2 == ' ') {
                            string output = "FORMATTING ERROR ";
                            output += character1;
                            cout <<  output  << endl;
                            return EXIT_SUCCESS;
                        }
                        // checks to ensure plaintext letter isn't matched to itself in dictionary
                        else if (character1 == character2) {
                            string output = "MAPPING ERROR ";
                            output += character1;
                            cout <<  output  << endl;
                            return EXIT_SUCCESS;
                        }
                        // checks to see if ciphertext letter already exists
                        else if (exists[index]!=0) {
                            char c = character2;
                            string output = "DUPLICATE CIPHERTEXT ";
                            output+=c;
                            cout << output << endl;
                            return EXIT_SUCCESS;
                        }
                        // ensures that the ciphertextletter doesnt already exists and ensures alphabetical order
                        else if (exists[index]==0 && character1==char(97+i)) {
                            arrLower[character2-97] = character1; // copies plaintext letter to ciphertext letter position
                            exists[index] = character2;
                            i++;
                        }
                        // if the dictionary is not in aphabetical order
                        else if (character1!=char(97+i)) {
                            char c = char(97+i);
                            string output = "MISSING LETTER ";
                            output+=c;
                            cout << output << endl;
                            return EXIT_SUCCESS;
                        }
                    }
                    file.close();
                    
                    // changes all lower case letter to upper case and stores in new list
                    for (int i=0; i<26; i++) {
                        arrUpper[i] = (arrLower[i] - 32);
                    }
                }
                
                if (argv[3]!=NULL) {
                    string file_name = argv[3];
                    ifstream file;
                    file.open(file_name);
                    
                    // checks to see if file can be opened
                    if (!file.is_open()) {
                        cout << file_name + " FILE COULD NOT BE OPENED" << endl;
                    }
                    // in the open file
                    else if (file.is_open()) {
                        string phrase;
                        // each line is a phrase
                        while (getline(file, phrase)) {
                            string message = "";
                            // goes through every character in the phrase one by one
                            for(int i=0; i<phrase.length(); i++) {
                                int index = 0;
                                // if the letter is a capital letter, then the upper list is used for decryption
                                if ((int)phrase[i]>64 && (int)phrase[i]<91) {
                                    index = (int)phrase[i]-65;
                                    message += arrUpper[index];
                                }
                                // if the letter is a lowercase letter, then the lower list is used for decryption
                                else if ((int)phrase[i]>96 && (int)phrase[i]<123) {
                                    index = (int)phrase[i]-97;
                                    message += arrLower[index];
                                }
                                // if the character is not a letter, it is simply added to the message
                                else
                                    message += phrase[i];
                             }
                             cout << message << endl;
                         }
                         file.close();
                    }
                }
                else if (argv[3]==NULL){
                    // input reads an entire line
                    string phrase;
                    // while there is a line to be read
                    while (getline(cin, phrase) || !cin.eof()) {
                        string message = "";
                        // parses through every character of the line
                        for(int i=0; i<phrase.length(); i++) {
                            int index = 0;
                            // checks to see if character is uppercase
                            if ((int)phrase[i]>64 && (int)phrase[i]<91) {
                                index = (int)phrase[i]-65;
                                message += arrUpper[index];
                            }
                            // checks to see if character is lowercase
                            else if ((int)phrase[i]>96 && (int)phrase[i]<123) {
                                index = (int)phrase[i]-97;
                                message += arrLower[index];
                            }
                            // the character is not a letter
                            else
                                message += phrase[i];
                        }
                        cout << message << endl;
                    }
                }
            }
            if (argc>4)
                cout << "TOO MANY ARGUMENTS" << endl;
        }
        
        // if command line argument isnt in code
        else {
            string command = argv[1];
            cout << command + " NOT A VALID COMMAND" << endl;
        }
    }
}
