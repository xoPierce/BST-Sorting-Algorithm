//============================================================================
// Name        : CS300Project2.cpp
// Author      : Jeffery Pierce Waren
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold bid information
struct Course {
    string number;                  // unique identifier
    string title;                   // unique course title
    vector<Course> prereqs;         // vector to hold prereq Course objects
    vector<string> tempPrereqs;     // temp vector to hold prereqs before validation
};

// Internal structure for tree node
struct Node {
    Course course;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
            Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void inOrder(Node* node);       
    void verifyPrereqs(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();          
    void VerifyPrereqs();
    void Insert(Course course);
    bool IsTreeEmpty();
    Course Search(string courseNumber);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}


// Checks if the bst is empty
bool BinarySearchTree::IsTreeEmpty() {
    bool isEmpty = true;
    if (root != nullptr) {
        isEmpty = false;
    }
    return isEmpty;
}


/**
 * Traverse the tree in order to verify tempPrereqs are an actual Course object that exists in the tree
 */
void BinarySearchTree::VerifyPrereqs() {
    // call verifyPrereqs fuction and pass root 
    verifyPrereqs(root);
}


/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    inOrder(root);
}


/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course);
    }
    else {
        // add Node root and bid
        this->addNode(root, course);
    }
}


/**
 * Search for a bid
 */
Course BinarySearchTree::Search(string courseNumber) {
    // set current node equal to root
    Node* current = root;
    
    // keep looping downwards until bottom reached or matching course number is found
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.number.compare(courseNumber) == 0) {
            return current->course;
        }

        // if course number is smaller than current node then traverse left
        else if (courseNumber.compare(current->course.number) < 0) {
            current = current->left;
        }
        // else course number is larger so traverse right
        else {
            current = current->right;
        }
    }
    // If a matching course not found return a empty course
    Course course;
    return course;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.number.compare(course.number) > 0 && node != 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
            return;
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    else if (node != nullptr && node->course.number.compare(course.number) < 0){
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
            return;
        }
        else {
            // recurse down the left node
            this->addNode(node->right, course);
        }
    }
}


/*
* Verifies that all of the temporary prereqs exist as Course objects within the bst before 
* adding them to the prereqs vector
*/ 
void BinarySearchTree::verifyPrereqs(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //vefifyPrereqs not left
        verifyPrereqs(node->left);
        // iterates through all tempPrereqs
        for (int i = 0; i < node->course.tempPrereqs.size(); i++) {
            // creates a blank course to assign each temp prereq to
            Course course; 
            // searches if the tempPrereq exists within the bst as a course object
            course = Search(node->course.tempPrereqs[i]);
            // if the course exists 
            if (!course.number.empty()) {
                // add the course to the prereq vector containing course objects
                node->course.prereqs.push_back(course);
            }
        }
        // Clears the element in tempPrereqs
        node->course.tempPrereqs.clear();
        //verifyPrereqs right
        verifyPrereqs(node->right);
    }
}


// prints all of the course numbers and titles alphabetically
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder not left
        inOrder(node->left);
        cout << node->course.number << ": " << node->course.title << endl;
        //InOder right
        inOrder(node->right);
    }
}


//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayCourse(Course course) {
    cout << course.number << ": " << course.title << endl;        
    return;
}


// Opens a csv file and creates Course objects from the file data to insert into a binary search tree
void loadCourses(string csvFile, BinarySearchTree* bst) {
    ifstream inputFile;
    // opens the file to read 
    inputFile.open(csvFile);
    string line = "";
    int coursesLoaded = 0;

    // iterates through each line within the csv file
    while (getline(inputFile, line)) {
        vector<string> v;
        stringstream ss(line);

        // seperates each column in to seperate substrings
        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
        }

        // Checks if the data is correctly fomatted. Has to have a course number and title to be added as a Course object
        if (v[0].compare("") == 0 || v[1].compare("") == 0) {
            cout << "Invalid course present in the data file. Either a course number or title is missing." << endl;
        }

        // If the data is in the correct format a Course will be inserted in to the bst
        else {
            // Blank course object created
            Course course;

            // Adds any data beyond 1 as a temporary prereq to be validated later on
            for (int i = 2; i < v.size(); i++) {

                // Checks if there is a string at each index
                if (v[i].compare("") == 1) {
                    // Adds the temp prereq to the course object
                    course.tempPrereqs.push_back(v[i]);
                }
            }
            course.number = v[0];       // set the course number to index 0
            course.title = v[1];        // sets the course title to index 1
            
            // inserts the newly created course object into the bst
            bst->Insert(course);
        }
        line = "";
        // Adds 1 after each course is attempted to be loaded
        coursesLoaded += 1;
    }
    // closes the file
    inputFile.close();
    // calls method to verify that the temp prereqs exist within the bst as course objects
    bst->VerifyPrereqs();
    cout << "Total number of courses attemped to load in to the course planner: " << coursesLoaded << endl;
}


/**
 * The one and only main() method
 */
//int main(int argc, char* argv[]) {
//    string csvPath, courseNumberKey;
//
//    // Define a binary search tree to hold all bids
//    BinarySearchTree* bst;
//    bst = new BinarySearchTree();
//    Course course;
//
//    int choice = 0;
//    while (choice != 9) {
//        cout << "- - - - - - - - - - - - - - - - -" << endl;
//        cout << "   Welcome to the course planner." << endl << endl;
//        cout << "       1. Load Data Structure." << endl;
//        cout << "       2. Print Course List." << endl;
//        cout << "       3. Print Course." << endl;
//        cout << "       9. Exit." << endl;
//        cout << "- - - - - - - - - - - - - - - - -" << endl;
//        cout << "   What would you like to do? ";
//        cin >> choice;
//
//        switch (choice) {
//
//        case 1:
//            cout << endl << "What file would you like to load in to the course planner? " << endl;
//            cin >> csvPath;           
//            //csvPath = "ABCU_Advising_Program_Input.csv";              // used during testing
//            loadCourses(csvPath, bst);
//            // if the tree is not empty
//            if (bst->IsTreeEmpty() == false) {
//                cout << csvPath << " has been successfully loaded." << endl << endl;
//            }
//            // if the tree is empty
//            else {
//                cout << csvPath << " is an invalid file name or path. Please try again." << endl;
//            }
//            break;
//
//        case 2:
//            cout << endl;
//            cout << "- - - - - - - - - - - - - - - - -" << endl;
//            cout << "Here is a sample schedule: " << endl << endl;
//            // prints all course objects within the bst in alphabetical order
//            bst->InOrder();
//            break;
//
//        case 3:
//            cout << "What course do you want to know about? " << endl;
//            // gets user input on what class to search for
//            cin >> courseNumberKey;
//            // calls method to search within the bst with the course number
//            course = bst->Search(courseNumberKey);
//
//            // if the course is found
//            if (!course.number.empty()) {
//                cout << "- - - - - - - - - - - - - - - - -" << endl;
//                displayCourse(course);
//                // if there are any prereqs 
//                if (course.prereqs.size() > 0) {
//                    cout << "Prerequisites:" << endl;
//                    for (int i = 0; i < course.prereqs.size(); i++) {
//                        cout << "   ";
//                        displayCourse(course.prereqs[i]);
//                    }
//                }
//                // if there are no prereqs associated with the course
//                else {
//                    cout << "Prerequisites: None" << endl;
//                }
//            }
//            // if the course is not found
//            else {
//                cout << "Course Number " << courseNumberKey << " not found." << endl;
//            }
//            break;
//
//        case 9:
//            break;
//
//        // if no other valid switch cases are chosen
//        default:
//            cout << choice << " is not a valid option." << endl << endl;
//        }
//    }
//
//    cout << "Thank you for using the course planner!" << endl;
//
//	return 0;
//}