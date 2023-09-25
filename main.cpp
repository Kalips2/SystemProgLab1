#include <iostream>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <map>

#define TEST_FOLDER "C:\\Users\\User\\CLionProjects\\SysProgLab1\\tests"
#define RESULT_FILE "C:\\Users\\User\\CLionProjects\\SysProgLab1\\result.txt"
#define MAX_WORD_LENGTH 30
#define END_OF_WORD '\0'

typedef enum {
    INIT,
    WORD_READING,
    SEPARATORS_READING
} states;

void processFilesInsideDir(const std::string &pathToDir);

void processFile(const std::string &path);

int main() {
    processFilesInsideDir(TEST_FOLDER);
    return 0;
}

void processFilesInsideDir(const std::string &pathToDir) {
    for (const auto &entry: std::filesystem::directory_iterator(pathToDir)) {
        if (entry.is_directory()) {
            continue;
        }
        processFile(entry.path().string());
    }
}

void processFile(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << path << std::endl;
        return;
    }

    char word[MAX_WORD_LENGTH];
    int current_index = 0;
    states current_state = INIT;

    bool is_alpha;
    char c = END_OF_WORD;
    std::map<std::string, int> wordCountMap;

    do {
        bool end_of_file = !file.get(c) && file.eof();
        if (end_of_file) c = END_OF_WORD;

        is_alpha = std::isalpha(c);

        if (current_state == INIT) current_state = is_alpha ? WORD_READING : SEPARATORS_READING;

        if (is_alpha && current_index < MAX_WORD_LENGTH) {
            if (current_state == SEPARATORS_READING) current_state = WORD_READING;
            word[current_index] = c;
            current_index++;
        } else if (!is_alpha && current_state == WORD_READING) {
            word[current_index] = END_OF_WORD;

            std::string currentWord(word);
            wordCountMap[currentWord]++;

            current_index = 0;
            current_state = SEPARATORS_READING;
        } else if (current_index >= MAX_WORD_LENGTH) {
            std::cerr << "Word too long. System won't process it." << std::endl;
            return;
        }
    } while (!file.eof());


    std::ofstream result_file(RESULT_FILE, std::ios::app); // Append mode
    if (!result_file.is_open()) {
        std::cerr << "Output file opening failed" << std::endl;
        return;
    }

    result_file << "For file - " << path << std::endl;
    for (const auto &pair: wordCountMap) {
        result_file << pair.first << ": " << pair.second << std::endl;
    }

    result_file.close();
}
