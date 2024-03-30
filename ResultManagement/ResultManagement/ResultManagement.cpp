#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

// Structure for storing student records
struct StudentRecord {
    string name;
    map<string, int> subjects; // Map to store subjects and their scores
};

// Function to read student records from a file
vector<StudentRecord> readRecordsFromFile(const string& filename) {
    ifstream file(filename);
    vector<StudentRecord> records;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        StudentRecord record;
        ss >> record.name;
        string subject;
        int score;
        while (ss >> subject >> score) {
            record.subjects[subject] = score;
        }
        records.push_back(record);
    }
    return records;
}

// Function to write student records to a file
void writeRecordsToFile(const string& filename, const vector<StudentRecord>& records) {
    ofstream file(filename);
    for (const auto& record : records) {
        file << record.name;
        for (const auto& subject : record.subjects) {
            file << " " << subject.first << " " << subject.second;
        }
        file << endl;
    }
}

// Function to authenticate users
bool authenticateUser(const string& username, const string& password, const map<string, string>& credentials) {
    auto it = credentials.find(username);
    return it != credentials.end() && it->second == password;
}

// Function to update student scores for a specific subject
void updateScore(vector<StudentRecord>& records, const string& studentName, const string& subject, int newScore) {
    for (auto& record : records) {
        if (record.name == studentName) {
            record.subjects[subject] = newScore;
            break;
        }
    }
}

// Function to compute average result for a specific student
double computeAverage(const StudentRecord& record) {
    double sum = 0;
    for (const auto& subject : record.subjects) {
        sum += subject.second;
    }
    return sum / record.subjects.size();
}

// Function to display all records
void displayRecords(const vector<StudentRecord>& records) {
    cout << "Student Records:\n";
    cout << setw(20) << "Name";
    for (const auto& record : records) {
        cout << setw(15) << record.name;
    }
    cout << endl;
    for (const auto& subject : records[0].subjects) {
        cout << setw(20) << subject.first;
        for (const auto& record : records) {
            cout << setw(15) << record.subjects.at(subject.first);
        }
        cout << endl;
    }
}

// Function to search for a specific record
void searchRecord(const vector<StudentRecord>& records, const string& studentName) {
    for (const auto& record : records) {
        if (record.name == studentName) {
            cout << "Record found for " << studentName << ":\n";
            cout << "Subjects:\n";
            for (const auto& subject : record.subjects) {
                cout << subject.first << ": " << subject.second << endl;
            }
            cout << "Average Score: " << computeAverage(record) << endl;
            return;
        }
    }
    cout << "Record not found.\n";
}

int main() {

  
    // Credentials
    map<string, string> credentials;
    credentials["admin"] = "admin";

    // Read student records from file
    vector<StudentRecord> records = readRecordsFromFile("Records.txt");

    // Main menu
    cout << "Welcome to Result Management System\n";
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (!authenticateUser(username, password, credentials)) {
        cout << "Invalid username or password. Exiting...\n";
        return 1;
    }
 
    char choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Update student score\n";
        cout << "2. View all records\n";
        cout << "3. Search for a record\n";
        cout << "4. Store records in a text file\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case '1': {
            string studentName, subject;
            int newScore;
            cout << "Enter student name: ";
            cin >> studentName;
            cout << "Enter subject: ";
            cin >> subject;
            cout << "Enter new score: ";
            cin >> newScore;
            updateScore(records, studentName, subject, newScore);
            cout << "Score updated successfully.\n";
            break;
        }
        case '2':
            displayRecords(records);
            break;
        case '3': {
            string studentName;
            cout << "Enter student name to search: ";
            cin >> studentName;
            searchRecord(records, studentName);
            break;
        }
        case '4':
            writeRecordsToFile("Records.txt", records);
            cout << "Records stored in file successfully.\n";
            break;
        case '5':
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != '5');

    return 0;
}
