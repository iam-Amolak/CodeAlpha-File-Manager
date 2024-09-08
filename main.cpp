#if defined(__APPLE__) || defined(__MACH__)
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<cstdio>
#include <dirent.h>
#include<string>
#include <unistd.h>
#include "sys/stat.h"
using namespace std;

string getOsName(void);
void print_welcomeMsg(void);
void pause_own(void);
string create_file(string);
string rewrite_file(string);
string view_file(string);
string copy_file(string, string);
string delete_file(string);
string open_directory(string);
bool isempty(ifstream&);

int main(){

    int choice; 
    bool exit = false;

    print_welcomeMsg();

    while(!exit){
        system("clear");
        cout<<"\n************************************************\n\n";
        cout<<"               FILE MANAGEMENT SYSTEM              \n";
        cout<<"\n************************************************\n\n";
        const size_t size = 1024; 
        char buffer[size]; 
        string current_directory;
        if (getcwd(buffer, size) != NULL) {} 
        else {
            cerr << "Error getting current working directory" << endl;
        }
        string current_path = buffer;
        if(getOsName() == "Mac OSX" || getOsName() == "Linux"){
            current_directory = current_path.substr(current_path.find_last_of("/") + 1, current_path.length());
        }
        else if(getOsName() == "Windows 32-bit" || getOsName() == "Windows 64-bit"){
            current_directory = current_path.substr(current_path.find_last_of("\\") + 1, current_path.length());
        }
        cout<<"_______________________________________________________________\n";
        cout<<endl<<"  Current Directory : "<<current_directory<<endl;
        cout<<"_______________________________________________________________\n"<<endl;
        
        cout<<" 1. Create new directory\n";
        cout<<" 2. Create new file\n";
        cout<<" 3. View file\n";
        cout<<" 4. Open directory\n";
        cout<<" 5. Close directory\n";
        cout<<" 6. List all files in current directory\n";
        cout<<" 7. Rewrite file\n";
        cout<<" 8. Copy file\n";
        cout<<" 9. Delete directory / file\n";
        cout<<" 10. EXIT\n";
        cout<<"\nEnter your choice : ";
        cin>>choice;
        
        if(choice == 1){
            const size_t size = 1024; 
            char buffer[size];        
            string dir_name;

            if (getcwd(buffer, size) != NULL) {

            } 
            else {
                cerr << "Error getting current working directory" << endl;
            }

            cout<<"\nEnter directory name : ";
            cin.ignore();
            getline(cin, dir_name);
            const char *name = dir_name.c_str();
            strcat(buffer, "/");
            strcat(buffer, name);
            int ret = mkdir(buffer , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            if(ret == 0){
                cout<<endl<<"Directory created successfully"<<endl;
                cin.clear();
                pause_own();
                continue;
            }
            else if(ret == -1){
                cerr << "\nError :  " << strerror(errno) << endl;
            }
            pause_own();
        }
        else if(choice == 2){
            string fname, edit_choice, return_value;
            cout<<"\n\nEnter file name : ";
            cin.ignore();
            getline(cin, fname);
            return_value = create_file(fname);
            if(return_value == "\nFile already exists...\n"){
                cout<<return_value;
                pause_own();
            }
            else{
                cout<<return_value;
                pause_own();
            }
        }
        else if(choice == 3){
            string fname, edit_choice;
            cout<<"\n\nEnter file name : ";
            cin.ignore();
            getline(cin, fname);
            cout<<view_file(fname);
            pause_own();
        }
        else if(choice == 4){    
            string dir_name;
            cout<<"\n\nEnter directory name : ";
            cin.ignore();
            getline(cin, dir_name);
            const char *c = dir_name.c_str();
            strcat(buffer, "/");
            strcat(buffer, c); 
            int res = chdir(buffer);
            if(res == -1){
                cout<<endl<<"Directory does not exist..."<<endl;
                pause_own();
                continue;
            }
            cout<<endl<<"Directory opened successfully..."<<endl;
            pause_own();
            continue;
        }
        else if(choice == 5){
            string new_directory_path;
            if(getOsName() == "Mac OSX" || getOsName() == "Linux"){
                new_directory_path = current_path.substr(0, current_path.find_last_of("/"));
            }
            else if(getOsName() == "Windows 32-bit" || getOsName() == "Windows 64-bit"){
                new_directory_path = current_path.substr(0, current_path.find_last_of("\\"));
            }
            int res = chdir(new_directory_path.c_str());
            if(res == -1){
                cout<<endl<<"Directory does not exist..."<<endl;
                pause_own();
                continue;
            }
            cout<<endl<<"Directory closed successfully..."<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pause_own();
            continue;
        }
        else if(choice == 6){
            DIR *dir = opendir(current_path.c_str());
            struct dirent *dp;
            struct stat statbuf;
            while(dir){
                if ((dp = readdir(dir)) != NULL){
                    if(dp->d_name[0] != '.' && dp->d_name[strlen(dp->d_name) - 1] != '~'){
                        cout<<endl<<dp->d_name<<endl;
                    }
                }else{
                    break;
                }
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pause_own();
        }
        else if(choice == 7){
            string fname;
            cout<<"\n\nEnter file name : ";
            cin>>fname;
            struct stat s;
            char arr[current_path.length() + 1];
            strcpy(arr, current_path.c_str());
            strcat(arr, "/");
            strcat(arr, fname.c_str());
            if(stat(arr, &s) == 0){
                if(s.st_mode & S_IFREG){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout<<rewrite_file(fname);
                }
                else{
                    cout<<endl<<"ERROR ! ! It is not a file...\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
            pause_own();
        }
        else if(choice == 8){
            string source_file, destination_file;
            cout<<"\nEnter the name of source file : ";
            cin.ignore();
            getline(cin, source_file);
            cout<<"\nEnter the name of destination file : ";
            getline(cin, destination_file);
            struct stat s;
            char arr[current_path.length() + 1];
            strcpy(arr, current_path.c_str());
            strcat(arr, "/");
            strcat(arr, source_file.c_str());
            if(stat(arr, &s) == 0){
                if(s.st_mode & S_IFREG){
                cout<<copy_file(source_file, destination_file);
                }
                else{
                    cout<<endl<<"ERROR ! ! It is not a file...\n";
                }
            }
            pause_own();
        }
        else if(choice == 9){
            string fname;
            cout<<endl<<"Enter file/directory name : ";
            cin.ignore();
            getline(cin, fname);
            int status = remove(fname.c_str());
            if(status == 0){
                cout<<endl<<"File/Directory removed Successfully...\n";
                pause_own();
            }
            else{
                cout<<endl<<"Some error occurred...\n";
                pause_own();
            }
        }
        else if(choice == 10){
            exit = true;
            continue;
        }
        else{
            cout<<endl<<"Invalid input. Select again..."<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pause_own();
        }
    }

    return 0;
}

string getOsName()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #endif
}                      

void print_welcomeMsg(){
	cout<<"\n\n";
	cout<<"  [][][] [][][] []     [][][]\n";
	cout<<"  []       []   []     []\n";
	cout<<"  [][]     []   []     [][]\n";
	cout<<"  []       []   []     [] \n";
	cout<<"  []     [][][] [][][] [][][]\n\n";
	cout<<"  []    []   []   []]  []   []   [][][][] [][][][] []    [] [][][][] []]   [] [[][][][]]\n";
	cout<<"  []}[]{[] []  [] [][] [] []  [] []       []       []}[]{[] []       [] [] []     []\n";
	cout<<"  [] [] [] [][][] [] [][] [][][] [] [][]] [][][][] [] [] [] [][][][] []  [][]     []\n";
	cout<<"  []    [] []  [] []  []] []  [] []    [] []       []    [] []       []   []]     []\n";
	cout<<"  []    [] []  [] []   [] []  [] [][][][] [][][][] []    [] [][][][] []    []     []\n\n";
	cout<<"  [][][][] []    [] [][][][] [[][][][]] [][][][] []}[]{[]\n";
	cout<<"  []         [] []  []           []     []       [] [] []\n";
	cout<<"  [][][][]    []    [][][][]     []     [][][][] []    []\n";
	cout<<"        []    []          []     []     []       []    []\n";
	cout<<"  [][][][]    []    [][][][]     []     [][][][] []    []\n\n";
    cout<<"Press any key to continue...";
    getchar();
    system("clear");
}

void pause_own()
{
    string dummy;
    cout << "\nPress Enter key to continue . . .";
    getline(cin, dummy);
}

string create_file(string file_name){
    fstream fout;
    fout.open(file_name, ios::in);
    if(!fout){
        fout.open(file_name, ios::out);
    }
    else{
        fout.close();
        return "\nFile already exists...\n";
    }
    fout.close();
    return "\nFile created Successfully...\n";
}

string rewrite_file(string file_name){
    fstream file;
    string input;
    bool moveOn = true;
    file.open(file_name, ios::in);
    if(!file){
        file.close();
        return "\nFile does not exist !!!\n";
    }
    else{
        file.close();
        file.open(file_name, ios::in | ios::out);
        cout<<endl<<view_file(file_name);
        cout<<endl<<"Enter text : "<<endl;
        getline(cin, input);
        file.close();
        delete_file(file_name);
        file.open(file_name, ios::out);
        file.seekp(0, ios::beg);
        file.write(input.c_str(), input.size());
        file.close();
        return "\nFile edited successfully...\n";
    }
}

bool isempty(ifstream& pFile)
{
    return pFile.peek() == ifstream::traits_type::eof();
}

string view_file(string file_name){
    ifstream fin;
    fin.open(file_name, ios::in);
    if(fin.fail() || fin.bad()){
        fin.close();
        return "\nSome error occured !!!\n";
    }
    fin.seekg(0);
    char ch;
    cout<<endl<<endl<<"***********************File Data***********************"<<endl<<endl;
    while (fin)
    {
        if(isempty(fin)){
            break;
        }
        fin.get(ch);
        cout<<ch;
    }
    return "\n\n-----End of File-----\n";
}

string delete_file(string file_name){
    int status = remove(file_name.c_str());
    if(status == 0){
        return "\nFile removed Successfully...\n";
    }
    else{
        return "\nSome error occurred...\n";
    }
}

string copy_file(string sname, string dname){
    ifstream fin;
    fstream fout;
    string data;
    fin.open(sname, ios::in);
    if(!fin || fin.fail() || fin.bad()){
        return "\n\nSome error occurred\n";
    }
    fout.open(dname, ios::in);
    if(!fout){
        fout.open(dname, ios::out);
        while(getline(fin, data)){
            fout<<data<<endl;
        }
        fout.close();
        fin.close();
        return "\nCopy Finished...\n";
    }
    else{
        fout.close();
        delete_file(dname);
        fout.open(dname, ios::out);
        while(getline(fin, data)){
            fout<<data<<endl;
        }
        fout.close();
        fin.close();
        return "\nCopy Finished...\n";
    }
}
#elif _WIN32
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<cstdio>
#include <dirent.h>
#include<string>
#include<limits>
#include "sys/stat.h"
#include <string.h>
using namespace std;

string getOsName(void);
void print_welcomeMsg(void);
void pause_own(void);
string create_file(string);
string rewrite_file(string);
string view_file(string);
string copy_file(string, string);
string delete_file(string);
string delete_directory_windows(string);
bool isempty(ifstream&);

int main(){

    int choice; 
    bool exit = false;

    print_welcomeMsg();

    while(!exit){
        if(getOsName() == "Mac OSX" || getOsName() == "Linux"){
            system("clear");
        }
        else if(getOsName() == "Windows 32-bit" || getOsName() == "Windows 64-bit"){
            system("CLS");
        }

        cout<<"\n************************************************\n\n";
        cout<<"               FILE MANAGEMENT SYSTEM              \n";
        cout<<"\n************************************************\n\n";
        const size_t size = 1024; 
        char buffer[size]; 
        string current_directory;
        if (getcwd(buffer, size) != NULL) {} 
        else {
            cerr << "Error getting current working directory" << endl;
        }
        string current_path = buffer;
        if(getOsName() == "Mac OSX" || getOsName() == "Linux"){
            current_directory = current_path.substr(current_path.find_last_of("/") + 1, current_path.length());
        }
        else if(getOsName() == "Windows 32-bit" || getOsName() == "Windows 64-bit"){
            current_directory = current_path.substr(current_path.find_last_of("\\") + 1, current_path.length());
        }
        cout<<"_______________________________________________________________\n";
        cout<<endl<<"  Current Directory : "<<current_directory<<endl;
        cout<<"_______________________________________________________________\n"<<endl;
        
        cout<<" 1. Create new directory\n";
        cout<<" 2. Create new file\n";
        cout<<" 3. View file\n";
        cout<<" 4. Open directory\n";
        cout<<" 5. Close directory\n";
        cout<<" 6. List all files in current directory\n";
        cout<<" 7. Rewrite file\n";
        cout<<" 8. Copy file\n";
        cout<<" 9. Delete directory / file\n";
        cout<<" 10. EXIT\n";
        cout<<"\nEnter your choice : ";
        cin>>choice;
        
        if(choice == 1){
            const size_t size = 1024; 
            char buffer[size];        
            string dir_name;

            if (getcwd(buffer, size) != NULL) {

            } 
            else {
                cerr << "Error getting current working directory" << endl;
            }

            cout<<"\nEnter directory name : ";
            cin.ignore();
            getline(cin, dir_name);
            const char *name = dir_name.c_str();
            strcat(buffer, "/");
            strcat(buffer, name);
            int ret = mkdir(buffer);
            if(ret == 0){
                cout<<endl<<"Directory created successfully"<<endl;
                cin.clear();
                pause_own();
                continue;
            }
            else if(ret == -1){
                cerr << "\nError :  " << strerror(errno) << endl;
            }
            pause_own();
        }
        else if(choice == 2){
            string fname, edit_choice, return_value;
            cout<<"\n\nEnter file name : ";
            cin.ignore();
            getline(cin, fname);
            return_value = create_file(fname);
            if(return_value == "\nFile already exists...\n"){
                cout<<return_value;
                pause_own();
            }
            else{
                cout<<return_value;
                pause_own();
            }
        }
        else if(choice == 3){
            string fname, edit_choice;
            cout<<"\n\nEnter file name : ";
            cin.ignore();
            getline(cin, fname);
            cout<<view_file(fname);
            pause_own();
        }
        else if(choice == 4){    
            string dir_name;
            cout<<"\n\nEnter directory name : ";
            cin.ignore();
            getline(cin, dir_name);
            const char *c = dir_name.c_str();
            strcat(buffer, "/");
            strcat(buffer, c); 
            int res = chdir(buffer);
            if(res == -1){
                cout<<endl<<"Directory does not exist..."<<endl;
                pause_own();
                continue;
            }
            cout<<endl<<"Directory opened successfully..."<<endl;
            pause_own();
            continue;
        }
        else if(choice == 5){
            string new_directory_path;
            if(getOsName() == "Mac OSX" || getOsName() == "Linux"){
                new_directory_path = current_path.substr(0, current_path.find_last_of("/"));
            }
            else if(getOsName() == "Windows 32-bit" || getOsName() == "Windows 64-bit"){
                new_directory_path = current_path.substr(0, current_path.find_last_of("\\"));
            }
            int res = chdir(new_directory_path.c_str());
            if(res == -1){
                cout<<endl<<"Directory does not exist..."<<endl;
                pause_own();
                continue;
            }
            cout<<endl<<"Directory closed successfully..."<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pause_own();
            continue;
        }
        else if(choice == 6){
            DIR *dir = opendir(current_path.c_str());
            struct dirent *dp;
            struct stat statbuf;
            while(dir){
                if ((dp = readdir(dir)) != NULL){
                    if(dp->d_name[0] != '.' && dp->d_name[strlen(dp->d_name) - 1] != '~'){
                        cout<<endl<<dp->d_name<<endl;
                    }
                }else{
                    break;
                }
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pause_own();
        }
        else if(choice == 7){
            string fname;
            cout<<"\n\nEnter file name : ";
            cin.ignore();
            getline(cin, fname);
            struct stat s;
            char arr[current_path.length() + 1];
            strcpy(arr, current_path.c_str());
            strcat(arr, "/");
            strcat(arr, fname.c_str());
            if(stat(arr, &s) == 0){
                if(s.st_mode & S_IFREG){
                cout<<rewrite_file(fname);
                }
                else{
                    cout<<endl<<"ERROR ! ! It is not a file...\n";
                }
            }
            pause_own();
        }
        else if(choice == 8){
            string source_file, destination_file;
            cout<<"\nEnter the name of source file : ";
            cin.ignore();
            getline(cin, source_file);
            cout<<"\nEnter the name of destination file : ";
            getline(cin, destination_file);
            struct stat s;
            char arr[current_path.length() + 1];
            strcpy(arr, current_path.c_str());
            strcat(arr, "/");
            strcat(arr, source_file.c_str());
            if(stat(arr, &s) == 0){
                if(s.st_mode & S_IFREG){
                cout<<copy_file(source_file, destination_file);
                }
                else{
                    cout<<endl<<"ERROR ! ! It is not a file...\n";
                }
            }
            pause_own();
        }
        else if(choice == 9){
            string fname;
            cout<<endl<<"Enter file/directory name : ";
            cin.ignore();
            getline(cin, fname);
            if(getOsName() == "Mac OSX"){
                cout<<delete_file(fname);
                pause_own();
            }
            else if(getOsName() == "Windows 32-bit" || getOsName() == "Windows 64-bit"){
                struct stat s;
                char arr[current_path.length() + 1];
                strcpy(arr, current_path.c_str());
                strcat(arr, "\\");
                strcat(arr, fname.c_str());
                if(stat(arr, &s) == 0){
                    if(s.st_mode & S_IFREG){
                        cout<<delete_file(fname);
                        pause_own();
                    }
                    else{
                        cout<<delete_directory_windows(arr);
                        pause_own();
                    }
                }
            }
            continue;
        }
        else if(choice == 10){
            exit = true;
            continue;
        }
        else{
            cout<<endl<<"Invalid input. Select again..."<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pause_own();
        }
    }

    return 0;
}

string getOsName()
{
    #ifdef _WIN32
    return "Windows 32-bit";
    #elif _WIN64
    return "Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return "Mac OSX";
    #elif __linux__
    return "Linux";
    #endif
}                      

void print_welcomeMsg(){
	cout<<"\n\n";
	cout<<"  [][][] [][][] []     [][][]\n";
	cout<<"  []       []   []     []\n";
	cout<<"  [][]     []   []     [][]\n";
	cout<<"  []       []   []     [] \n";
	cout<<"  []     [][][] [][][] [][][]\n\n";
	cout<<"  []    []   []   []]  []   []   [][][][] [][][][] []    [] [][][][] []]   [] [[][][][]]\n";
	cout<<"  []}[]{[] []  [] [][] [] []  [] []       []       []}[]{[] []       [] [] []     []\n";
	cout<<"  [] [] [] [][][] [] [][] [][][] [] [][]] [][][][] [] [] [] [][][][] []  [][]     []\n";
	cout<<"  []    [] []  [] []  []] []  [] []    [] []       []    [] []       []   []]     []\n";
	cout<<"  []    [] []  [] []   [] []  [] [][][][] [][][][] []    [] [][][][] []    []     []\n\n";
	cout<<"  [][][][] []    [] [][][][] [[][][][]] [][][][] []}[]{[]\n";
	cout<<"  []         [] []  []           []     []       [] [] []\n";
	cout<<"  [][][][]    []    [][][][]     []     [][][][] []    []\n";
	cout<<"        []    []          []     []     []       []    []\n";
	cout<<"  [][][][]    []    [][][][]     []     [][][][] []    []\n\n";
    cout<<"Press any key to continue...";
    getchar();
    system("clear");
}

void pause_own()
{
    string dummy;
    cout << "\nPress Enter key to continue . . .";
    getline(cin, dummy);
}

string create_file(string file_name){
    fstream fout;
    bool exists = false;
    fout.open(file_name, ios::in);
    const size_t size = 1024; 
    char buffer[size]; 
    string current_directory;
    if (getcwd(buffer, size) != NULL) {} 
    else {
        cerr << "Error getting current working directory" << endl;
    }
    string current_path = buffer;
    DIR *dir = opendir(current_path.c_str());
    struct dirent *dp;
    struct stat statbuf;
    while(dir){
        if ((dp = readdir(dir)) != NULL){
            if(dp->d_name == file_name){
                exists = true;
                break;
            }
        }else{
            break;
        }
    }
    if(!fout && exists == false){
        fout.open(file_name, ios::out);
    }
    else{
        fout.close();
        return "\nFile already exists...\n";
    }
    fout.close();
    return "\nFile created Successfully...\n";
}

string rewrite_file(string file_name){
    fstream file;
    string input;
    bool moveOn = true;
    file.open(file_name, ios::in);
    if(!file){
        file.close();
        return "\nFile does not exist !!!\n";
    }
    else{
        file.close();
        file.open(file_name, ios::in | ios::out);
        cout<<endl<<view_file(file_name);
        cout<<endl<<"Enter text : "<<endl;
        getline(cin, input);
        file.close();
        delete_file(file_name);
        file.open(file_name, ios::out);
        file.seekp(0, ios::beg);
        file.write(input.c_str(), input.size());
        file.close();
        return "\nFile edited successfully...\n";
    }
}

string view_file(string file_name){
    ifstream fin;
    fin.open(file_name, ios::in);
    if(fin.fail() || fin.bad()){
        fin.close();
        return "\nSome error occured !!!\n";
    }
    fin.seekg(0);
    char ch;
    cout<<endl<<endl<<"***********************File Data***********************"<<endl<<endl;
    while (fin)
    {
        if(isempty(fin)){
            break;
        }
        fin.get(ch);
        cout<<ch;
    }
    return "\n\n-----End of File-----\n";
}

string copy_file(string sname, string dname){
    ifstream fin;
    fstream fout;
    string data;
    fin.open(sname, ios::in);
    if(!fin || fin.fail() || fin.bad()){
        return "\n\nSome error occurred\n";
    }
    fout.open(dname, ios::in);
    if(!fout){
        fout.open(dname, ios::out);
        while(getline(fin, data)){
            fout<<data<<endl;
        }
        fout.close();
        fin.close();
        return "\nCopy Finished...\n";
    }
    else{
        fout.close();
        delete_file(dname);
        fout.open(dname, ios::out);
        while(getline(fin, data)){
            fout<<data<<endl;
        }
        fout.close();
        fin.close();
        return "\nCopy Finished...\n";
    }
}

string delete_file(string file_name){
    int status = remove(file_name.c_str());
    if(status == 0){
        return "\nFile removed Successfully...\n";
    }
    else{
        return "\nSome error occurred...\n";
    }
}

string delete_directory_windows(string dname){
    int status = rmdir(dname.c_str());
    if(status == 0){
        return "\nDirectory removed Successfully...\n";
    }
    else{
        return "\nSome error occurred...\n";
    }
}

bool isempty(ifstream& pFile)
{
    return pFile.peek() == ifstream::traits_type::eof();
}
#endif