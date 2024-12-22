//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Tristan Maloy
// Version     : 1.0
// Description : CS 300 Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

//course structure
struct Course {
    string courseNumber;
    string courseName;
    vector<string> coursePreReqs;

    //default constructor
    Course() {};

    //constructor no prereqs
    Course(string aCourseNumber, string aCourseName) {
        courseNumber = aCourseNumber;
        courseName = aCourseName;
    }

    //constructor w prereqs
    Course(string aCourseNumber, string aCourseName, vector<string> aCoursePreReqs) {
        courseNumber = aCourseNumber;
        courseName = aCourseName;
        coursePreReqs = aCoursePreReqs;
    }
};

//internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    //default constructor 
    Node() {
        left = nullptr;
        right = nullptr;
    }

    //initialize with course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//Define class with data members and methods to implement BST
class BST {
private:
    Node* root;

    void addNode(Course acourse, Node* node);
    void inOrder(Node* node);
    void printCourse(Node* node, string courseNumber);

public:
    BST();
    void InOrder();
    void PrintCourse(string courseNumber);
    void insert(Course course);
};

//default constructor
BST::BST() {
    root = nullptr;
}

//Traverse tree in order
void BST::InOrder() {
    inOrder(root); //pass root to inOrder function
}

void BST::inOrder(Node* node) {
    //if node is not nullptr
    if (node != nullptr) {
        //pass left node through inOrder
        inOrder(node->left);
        //print course information
        std::cout << node->course.courseNumber << ": " << node->course.courseName << " | PreRequisites: ";
        //iterate through coursePreReq vector and print coursePreReqs
        for (int i = 0; i < node->course.coursePreReqs.size(); ++i) {
            std::cout << node->course.coursePreReqs[i] << " ";
        }
        //if vector is empty print NONE
        if (node->course.coursePreReqs.empty()) {
            std::cout << "NONE";
        }
        std::cout << endl;
        //pass right node through inOrder
        inOrder(node->right);
    }
}

//Print course information
void BST::PrintCourse(string courseNumber) { 
    printCourse(root, courseNumber); //pass root node to printCourse
}

void BST::printCourse(Node* node, string courseNumber) { 
    //while node is not nullptr
    while (node != nullptr) {
        //if courseNumber matches
        if (node->course.courseNumber.compare(courseNumber) == 0) {
            //output courseNumber and courseName
            std::cout << "Course Number: " << node->course.courseNumber << endl;
            std::cout << "Course Name: " << node->course.courseName << endl;

            //std::cout << "Prereq size: " << node->course.coursePreReqs.size() << endl; used to verify prereqs
            
            //if no prereqs output none
            if (node->course.coursePreReqs.empty()) {
                std::cout << "Course PreRequisites : None";
            }

            //iterate through coursePreReqs vector and output preReqs
            else {
                std::cout << "Course PreRequisites: ";
                for (int i = 0; i < node->course.coursePreReqs.size(); ++i) {
                    std::cout << node->course.coursePreReqs[i] << " ";
                    }
                }
            std::cout << endl;
            return;
            
        }
        //else if node->courseNumber is smaller than courseNumber and node to left is not nullptr
        else if (node->course.courseNumber.compare(courseNumber) > 0 && node->left != nullptr) {
            node = node->left;
        }
        //else if node->courseNumber is larger than courseNumber and node to right is not nullptr
        else if (node->course.courseNumber.compare(courseNumber) < 0 && node->right != nullptr) {
            node = node->right;
        }
        //else courseNumber not found
        else {
            std::cout << "INVALID COURSE NUMBER\n";
            return;
        }
    }
}

//Insert a course
void BST::insert(Course course) {
    //if root is nullptr, root is equal to new node course
    if (root == nullptr) {
        root = new Node(course);
    }
    //else add node course root
    else {
        addNode(course, root);
    }
}

//Add a course to Node
void BST::addNode(Course course, Node* node) {
    //if node is larger add to left
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        //if no left node
        if (node->left == nullptr) {
            //left node becomes new node
            node->left = new Node(course);
        }
        //else recurse down left node
        else {
            addNode(course, node->left);
        }
    }
    //else if node is smaller
    else {
        //if no right node
        if (node->right == nullptr) {
            //right node becomes new node
            node->right = new Node(course);
        }
        //else recurse down right node
        else {
            addNode(course, node->right);
        }
    }
}

//stringstream to split 
vector<string> Split(string line) {

    //convert string to stringstream
    stringstream ss(line);
    vector<string> lineTok;
    string temp = ""; //store current word up to delimiter
    while (getline(ss, temp, ',')){
        if (temp == "") { //delimiter adds "" to empty prereq 
            temp.clear(); //clear temp when temp == ""
        }
        else {
            lineTok.push_back(temp);
        }
    }

    return lineTok;
}

void loadCourses(string file, BST* courseList) { 
    ifstream inFS;
    string line;
    vector<string> strTok;
    //open file CS 300 ABCU_Advising_Program_Input.csv
    inFS.open(file);

    //if file is not open
    if (!inFS.is_open()) {
        //output error message
        std::cout << "ERROR. FILE NOT OPEN.\n";
        return;
        }

    //while not at end of file
    while (!inFS.eof()) {
        //temp structure for course
        Course course;

        getline(inFS, line);

        //split line into tok 
        strTok = Split(line);

        //catch error if line doesn't include 2 
        if (strTok.size() < 2) {
            continue;
        }

        //populate vector with course info
        course.courseNumber = strTok.at(0);
        course.courseName = strTok.at(1);

        //iterate through prereqs
        for (unsigned int i = 2; i < strTok.size(); i++) {
            course.coursePreReqs.push_back(strTok.at(i));
        }
        //pass course to insert function to add to BST
        courseList->insert(course);
    }
    //close file
    inFS.close();
    std::cout << "Courses have been loaded.\n\n";
}

int main() {


    // Define a binary search tree to hold all courses
    BST* courseList;
    courseList = new BST();
    Course course;
    bool isLoaded = false;
    string courseNumber;
    string file = "CS 300 ABCU_Advising_Program_Input.csv";

    int choice = 0;
    while (choice != 9) {
        std::cout << "Menu:" << endl;
        std::cout << "  1. Load Courses" << endl;
        std::cout << "  2. Display All Courses" << endl;
        std::cout << "  3. Find and Print Course" << endl;
        std::cout << "  9. Exit" << endl;
        std::cout << "Enter choice: ";
        cin >> choice;
        std::cout << "\n";

        switch (choice) {

        case 1:

            //load data from file CS 300 ABCU_Advising_Program_Input.csv
            if (isLoaded == false) {
                loadCourses(file, courseList);
                isLoaded = true;
                break;
            }
            else {
                std::cout << "Data already loaded.\n\n";
                break;
            }

        case 2:

            //sort and print in order
            courseList->InOrder();
            std::cout << "\n";
            break;

        case 3:

            std::cout << "INPUT Course Number: ";
            //course number input
            cin >> courseNumber;
            std::cout << endl;
            transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);
            //pass courseNumber through PrintCourse()
            courseList->PrintCourse(courseNumber);
            std::cout << endl;
            break;

        case 9:
            std::cout << "EXITING PROGRAM" << endl;
            break;

        default:
            std::cout << "INVALID. TRY AGAIN USING 1, 2, 3, OR 9 TO QUIT.\n";
            cin.clear(); //clear cin to prevent infinite loop when a non numeric character gets input
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignores bad character input
            std::cout << endl;
            break;
        }
    }

    std::cout << "Good bye." << endl;

    return 0;
}