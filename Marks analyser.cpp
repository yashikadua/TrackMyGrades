#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <io.h>
using namespace std;
bool isIDPresent(int ID) {
    ifstream file("results.csv");
    string line;

    ostringstream oss;
    oss << ID;
    string idStr = oss.str();

    while (getline(file, line)) {
        size_t pos = line.find(',');
        if (pos != string::npos) {
            string existingID = line.substr(0, pos);
            if (existingID == idStr) {
                return true;
            }
        }
    }
    return false;
}

void newstudent() {
    string grade, remarks;
    int marks, ID, max_total, totalsub, totalmarks = 0;
    float per;
    char name[20], subname[30];

    cout << "\nEnter ID of student: ";
    cin >> ID;
    if (isIDPresent(ID)) {
        cout << "This ID already exists. Please enter a unique ID.\n";
        return;
    }

    cout << "\nEnter the name of student: ";
    cin >> name;

    cout << "\nEnter total number of subjects: ";
    cin >> totalsub;
    cout << endl;
    max_total = totalsub * 100;

    // Collect subject details
    for (int i = 1; i <= totalsub; i++) {
        cout << "Enter the name of subject " << i << ": ";
        cin >> subname;
        cout << "Enter marks of " << subname << ": ";
        cin >> marks;

        if (marks < 0 || marks > 100) {
            cout << "Invalid marks. Please enter a value between 0 and 100.\n";
            cout << "Enter marks of " << subname << ": ";
            cin >> marks;
        }
        totalmarks += marks;
    }

    per = (float)totalmarks / max_total * 100;

    // Display and calculate grade and remarks
    cout << "Student ID: " << ID;
    cout << "\nStudent Name: " << name;
    cout << "\nTotal Marks: " << totalmarks << endl;

    if (totalmarks >= 0.9 * max_total) {
        grade = "A";
        remarks = "Excellent! You performed well.";
    } else if (totalmarks >= 0.8 * max_total) {
        grade = "B";
        remarks = "Very good! You performed well.";
    } else if (totalmarks >= 0.7 * max_total) {
        grade = "C";
        remarks = "Good! Do better next time.";
    } else if (totalmarks >= 0.6 * max_total) {
        grade = "D";
        remarks = "Focus more on your studies.";
    } else {
        grade = "F";
        remarks = "Needs improvement.";
    }

    cout << "The grade of " << name << " is: " << grade << endl;
    cout << remarks << endl;

    // Save to file
    ofstream file("results.csv", ios::app);
    if (file.is_open()) {
        file << ID << "," << name << "," << totalsub << "," << totalmarks << "," << max_total << "," << remarks << "," << grade << "\n";
        file.close();
        cout << "Result saved to 'results.csv'.\n";
    } else {
        cerr << "Error opening results.csv!" << endl;
    }
}

int searchStudentByID(int searchID) {
    ifstream file("results.csv");
    string line;
    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');

        int id = std::stoi(idStr);
        if (id == searchID) {
            cout << "\n--- Student Record Found ---\n";
            cout << "ID,Name,Subjects,Total Marks,Max Marks,Remarks,Grade\n";
            cout << line << "\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "No student found with ID: " << searchID << "\n";
    }

    file.close();
    return 0;
}

void displayAllResults() {
    ifstream file("results.csv");
    string line;

    cout << "\n===== All Student Results =====\n";
    cout << "ID,Name,Subjects,Total Marks,Max Marks,Remarks,Grade\n";

    bool isEmpty = true;
    while (getline(file, line)) {
        cout << line << endl;
        isEmpty = false;
    }

    if (isEmpty) {
        cout << "No records found in the file.\n";
    }

    file.close();
}

void deleteOrUpdateStudent(int targetID) {
    ifstream file("results.csv");
    ofstream temp("temp.csv");
    string line;
    bool found = false;

    while (getline(file, line)) {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        int id = stoi(idStr);

        if (id == targetID) {
            found = true;

            cout << "Student found: " << line << "\n";
            cout << "Do you want to delete (D) or update (U) this entry? ";
            char option;
            cin >> option;

            if (option == 'D' || option == 'd') {
                cout << "Record deleted.\n";
                continue; // Skip writing to temp file (deletes record)
            } else if (option == 'U' || option == 'u') {
                string name, remarks, grade;
                int subjects, total, max;

                cout << "Enter new name: ";
                cin >> name;
                cout << "Enter total subjects: ";
                cin >> subjects;
                cout << "Enter total marks: ";
                cin >> total;
                max = subjects * 100;

                if (total >= 0.9 * max) {
                    grade = "A"; remarks = "Excellent!";
                } else if (total >= 0.8 * max) {
                    grade = "B"; remarks = "Very Good!";
                } else if (total >= 0.7 * max) {
                    grade = "C"; remarks = "Good";
                } else if (total >= 0.6 * max) {
                    grade = "D"; remarks = "Needs Improvement";
                } else {
                    grade = "F"; remarks = "Failed";
                }

                temp << id << "," << name << "," << subjects << "," << total << "," << max << "," << remarks << "," << grade << "\n";
                cout << "Record updated.\n";
                continue;
            }
        } else {
            temp << line << "\n";
        }
    }

    file.close();
    temp.close();

    remove("results.csv");
    rename("temp.csv", "results.csv");

    if (!found) {
        cout << "No student found with ID: " << targetID << "\n";
    }
}

void exportin() {
    ifstream file("results.csv");
    vector<vector<string>> data;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        vector<string> row;

        while (getline(ss, item, ',')) {
            row.push_back(item);
        }
        data.push_back(row);
    }
    file.close();

    char format;
    cout << "Export as JSON (J) or XML (X)? ";
    cin >> format;

    if (format == 'J' || format == 'j') {
        ofstream jsonFile("results.json");
        jsonFile << "[\n";
        for (size_t i = 0; i < data.size(); ++i) {
            jsonFile << "  {\n";
            jsonFile << "    \"ID\": \"" << data[i][0] << "\",\n";
            jsonFile << "    \"Name\": \"" << data[i][1] << "\",\n";
            jsonFile << "    \"Subjects\": \"" << data[i][2] << "\",\n";
            jsonFile << "    \"TotalMarks\": \"" << data[i][3] << "\",\n";
            jsonFile << "    \"MaxMarks\": \"" << data[i][4] << "\",\n";
            jsonFile << "    \"Remarks\": \"" << data[i][5] << "\",\n";
            jsonFile << "    \"Grade\": \"" << data[i][6] << "\"\n";
            jsonFile << "  }";
            if (i != data.size() - 1) jsonFile << ",";
            jsonFile << "\n";
        }
        jsonFile << "]\n";
        jsonFile.close();
        cout << "Exported to results.json\n";
    } else if (format == 'X' || format == 'x') {
        ofstream xmlFile("results.xml");
        xmlFile << "<Students>\n";
        for (auto &row : data) {
            xmlFile << "  <Student>\n";
            xmlFile << "    <ID>" << row[0] << "</ID>\n";
            xmlFile << "    <Name>" << row[1] << "</Name>\n";
            xmlFile << "    <Subjects>" << row[2] << "</Subjects>\n";
            xmlFile << "    <TotalMarks>" << row[3] << "</TotalMarks>\n";
            xmlFile << "    <MaxMarks>" << row[4] << "</MaxMarks>\n";
            xmlFile << "    <Remarks>" << row[5] << "</Remarks>\n";
            xmlFile << "    <Grade>" << row[6] << "</Grade>\n";
            xmlFile << "  </Student>\n";
        }
        xmlFile << "</Students>\n";
        xmlFile.close();
        cout << "Exported to results.xml\n";
    } else {
        cout << "Invalid format choice.\n";
    }
}

int main() {
    int menu;
    string choice;
    
    do {
        cout << "\n>>>>>>>>>>Student Marks Analyzer<<<<<<<<<<\n";
        cout << "1. Add New Student Result\n";
        cout << "2. Search Student by ID\n";
        cout << "3. Display All Results\n";
        cout << "4. Delete/Update Student Entry\n";
        cout << "5. Export to JSON or XML\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> menu;

        switch (menu) {
            case 1:
                newstudent();
                break;
            case 2: {
                int searchID;
                cout << "Enter ID to search: ";
                cin >> searchID;
                searchStudentByID(searchID);
                break;
            }
            case 3:
                displayAllResults();
                break;
            case 4: {
                int updateID;
                cout << "Enter ID to delete/update: ";
                cin >> updateID;
                deleteOrUpdateStudent(updateID);
                break;
            }
            case 5:
                exportin();
                break;
     		case 6:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
       		}

        cout << "Do you want to enter another student? (y/n): ";
        cin >> choice;
    } while (choice == "y" || choice == "Y");
	if(choice == "n" || choice=="N")
	{
		cout<<"Exiting program. Goodbye!\n";
	}
    return 0;
}
