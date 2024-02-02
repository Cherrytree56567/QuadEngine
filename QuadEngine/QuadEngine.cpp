#include <iostream>
//#include <dirent.h>
#include <SHA256.h>
#include <filesystem>
#include <string>
#include <vector>

/*
* Goal: Make a working Antivirus Engine in C++ that uses SHA256 Hashes to scan Files.
* 
* Tasks:
* - Implement a SHA256 Scanning Algorithm. ✅
* - Open files
* - Open Directories and Sub dirs.
* - make a function to open and read each file recursivly.
* - Read text file as database in current location as db.quad. ✅
* - get all hashes from db in array. ✅
* - Get hash from file each file and match it against each hash in array.
*/

/*
* Simple Function to get the SHA256 hash from a file path.
* Args: const std::string (FilePath)
* Return: std::string (SHA256 hash)
*/
std::string GetSHA256fromFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Error: Failed to open file : File is open!.\n";
        exit(-1);
    }

    std::vector<unsigned char> fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return picosha2::hash256_hex_string(fileContent);
}

/*
* Simple Function to read db.quad and return an array of std::string full of hashes
* Args: const char* dbFile
* Return: std::vector<std::string> hashes
*/
std::vector<std::string> ReadDataBase(const char* dbFile) {
    std::ifstream file(dbFile);
    std::vector<std::string> has;

    if (!file.is_open()) {
        std::cout << "Error: Failed to open Database file : File is open!.\n";
        exit(-2);
    }

    std::string line;
    while (std::getline(file, line)) {
        has.push_back(line);
    }

    file.close();
    return has;
}

/*
* Simple Function to Scan File or Directory and return a vector of std::string' with file names.
* Args: const char* dbFile, const char* FileOrDirectory
* Return: std::vector<std::string> virus
*/
std::vector<std::string> QuadScan(const char* dbFile, const char* FileOrDirectory) {
    std::vector<std::string> db = ReadDataBase(dbFile);
    std::vector<std::string> virus;

    for (const auto& p : std::filesystem::recursive_directory_iterator(FileOrDirectory)) {
        if (!std::filesystem::is_directory(p)) {
            for (auto& aRule : db) {
                if (aRule == GetSHA256fromFile(p.path().string())) {
                    virus.push_back(p.path().string());
                }
            }
        }
    }
    return virus;
}

/*

// define a recursive function to traverse through the dir and sub-dirs
void readDir(const std::string& name, std::vector<std::string>& v) {
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp) != NULL)){
        if (dp->d_type == DT_DIR) {
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
                readDir(name + "/" + std::string(dp->d_name), v);
            }
        } else {
            v.push_back(name + "/" + std::string(dp->d_name));
        }
    }
    closedir(dirp);
}*/

// call the function and print the results
int main() {
    /*std::vector<std::string> v;
    readDir(".", v);
    for (auto s : v) {
        std::cout << s << std::endl;
    }*/

    char* dir = new char[300];

    std::cout << "Welcome to QuadScan!\nPlease enter a directory to Scan: ";
    std::cin.getline(dir, 300);
    std::vector<std::string> viru = QuadScan("C:\\Users\\ronit\\Desktop\\QuadEngine\\QuadEngine\\db.quad", dir);

    for (auto& vir : viru) {
        std::cout << "virus Found: " << vir << std::endl;
    }
    std::cin;
    return 0;
}