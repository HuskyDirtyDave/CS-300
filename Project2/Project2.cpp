
//============================================================================
// Name		   : Project2.cpp
// Author	   : David Towne
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream> 
#include "CSVparser.hpp" 

using namespace std;
//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// define a structure to hold course information
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisite;
	Course() {}
};

// Internal structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;
	Node() {
		left = nullptr;
		right = nullptr;
	}
	Node(Course firstCourse) : Node() {
		this->course = firstCourse;
	}
};


//============================================================================
// CourseBinarySearchTree Class Definition 
//============================================================================

class CourseBinarySearchTree {
private:
	Node* root;
	void addNode(Node* node, Course course);
	void sampleSchedule(Node* node);
	void courseInformation(Node* node, string courseNumber);
public:
	CourseBinarySearchTree();
	virtual ~CourseBinarySearchTree();
	void Remove(Node* node);
	void Add(Course course);
	int NumberPrerequisiteCourses(Course course);
	void SampleSchedule();
	void CourseInformation(string courseNumber);
};

/**
 * Default constructor
 */
CourseBinarySearchTree::CourseBinarySearchTree() {
	//root is equal to nullptr
	root = nullptr;
}

/**
 * Destructor
 */
CourseBinarySearchTree::~CourseBinarySearchTree() {
	Remove(root);
}

/*
 * Remove the Binary Search Tree nodes
 * Used by the destructor
 */
void CourseBinarySearchTree::Remove(Node* node) {
	if (node) {
		Remove(node->left);
		Remove(node->right);
		delete node;
	}
}

/*
 * Add a node into the Binary Search Tree
 */
void CourseBinarySearchTree::Add(Course course) {
	// Checking if the root isn't initialized
	if (root == nullptr)
		//initialize the root with the current course info
		root = new Node(course);
	//Else send the node to the addNode method 
	   //to be added to the Binary Search Tree
	else

		this->addNode(root, course);
}


int CourseBinarySearchTree::NumberPrerequisiteCourses(Course course) {
	int count = 0;
	for (unsigned int i = 0; i < course.prerequisite.size(); i++) {
		if (course.prerequisite.at(i).length() > 0)
			count++;
	}
	return count;
}

void CourseBinarySearchTree::SampleSchedule() {
	this->sampleSchedule(root);
}

void CourseBinarySearchTree::CourseInformation(string courseNumber) {
	this->courseInformation(root, courseNumber);
}

void CourseBinarySearchTree::addNode(Node* node, Course course) {
	// If node is less than current node, insert course
	if (node->course.courseNumber.compare(course.courseNumber) > 0) {
		if (node->left == nullptr)
			node->left = new Node(course);
		else
			this->addNode(node->left, course);
	}
	// Else course is greater than or equal to current node
	else {
		if (node->right == nullptr)
			node->right = new Node(course);
		else
			this->addNode(node->right, course);
	}
}

/*
 * Displays them in order
 */
void CourseBinarySearchTree::sampleSchedule(Node* node) {
	if (node != nullptr) {
		sampleSchedule(node->left);
		cout << node->course.courseNumber << ", " << node->course.courseName << endl;
		sampleSchedule(node->right);
	}
	return;
}

/* 
 * Displays a single courses information including its prerequisite
*/
void CourseBinarySearchTree::courseInformation(Node* curr, string courseNumber) {
	while (curr != nullptr) {
		// If curr matches courseNumber
		if (curr->course.courseNumber.compare(courseNumber) == 0) {
			// Display course and get courseNumber prerequisite
			cout << endl << curr->course.courseNumber << ", " << curr->course.courseName << endl;
			unsigned int size = NumberPrerequisiteCourses(curr->course);
			cout << "Prerequisite(s): ";
			// If prerequisite exist, display each course prerequisite
			unsigned int i = 0;
			for (i = 0; i < size; i++) {
				cout << curr->course.prerequisite.at(i);
				if (i != size - 1)
					cout << ", ";
			}
			// If no prerequisite
			if (i == 0)
				cout << "None.";
			cout << endl;
			return;
		}
		// Compare courseNumber with curr courseNumber and if it smaller traverse left
		else if (courseNumber.compare(curr->course.courseNumber) < 0)
			curr = curr->left;
		// Compare courseNumber with curr courseNumber and if it larger traverse right
		else
			curr = curr->right;
	}
	// Display course was not found
	cout << "Course " << courseNumber << " not found." << endl;
}
//==============================================================================
// Static methods used for testing                                         
//==============================================================================

bool loadCourses(string csvPath, CourseBinarySearchTree* coursesBST) {
	try {
		ifstream coursesFile(csvPath);
		if (coursesFile.is_open()) {
			while (!coursesFile.eof()) {
				/**
				 * Load a CSV file containing bids into a container
				 *
				 * @param csvPath the path to the CSV file to load
				 * @return a container holding all the courses read
				 */
				vector<string> info;
				string data;
				getline(coursesFile, data);
				while (data.length() > 0) {
						unsigned int comma = data.find(',');
					if (comma < 100) { 
							info.push_back(data.substr(0, comma));
						data.erase(0, comma + 1);
					}
					else {
						info.push_back(data.substr(0,
							data.length()));
						data = "";
					}
				}
				// Create a data structure and add to the collection of courses
				Course course;
				course.courseNumber = info[0];
				course.courseName = info[1];
				for (unsigned int i = 2; i < info.size(); i++) {
					course.prerequisite.push_back(info[i]);
				}
				coursesBST->Add(course);
			}
			coursesFile.close();
			return true;
		}
	}
	catch (csv::Error& e) {
		cerr << e.what() << endl;
	}
	return false;
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
	// Process command line argument
	string csvPath, courseId;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseId = argv[2];
		break;
	default:
		csvPath = "coures.csv";
		break;
	}
	// Define Binary Search Tree
	CourseBinarySearchTree* coursesBST = nullptr;
	cout << "\nWelcome to the course planner!\n" << endl;
	// Preventing invalid data by making user choice a string and converting first char to int 
	string choice = "0";
	int userChoice = choice[0] - '0';
	while (userChoice != 9) {
		cout << " 1. Load Data Structure" << endl;
		cout << " 2. Print Course List" << endl;
		cout << " 3. Print Course" << endl;
		cout << " 9. Exit" << endl;
		cout << "\nWhat would you like to do? ";
		cin >> choice;
		// Check if user choice is a double digit
		if (choice.length() == 1)
			userChoice = choice[0] - '0';
		else
			userChoice = 0;
		bool success = 1;
		// Handle users choice from menu
		switch (userChoice) {
			// Instantiate Binary Search Tree
			// get file path name
			//and load courses into Binary Search Tree
		case 1:
			if (coursesBST == nullptr)
				coursesBST = new CourseBinarySearchTree();
			if (csvPath.length() == 0) {
				cout << "Enter the file you wish to look up: ";
				cin >> csvPath;
			}
			// Making sure the file was able to load successfully
			success = loadCourses(csvPath, coursesBST);
			if (success)
				cout << "Courses loaded.\n" << endl;
			else
				cout << "Check the file name file not found.\n" << endl;
			csvPath = "";
			break;
			// Checking and validating Binary Search Tree
		case 2:
			//If valid print a sample schedule
			if (coursesBST != nullptr && success) {
				cout << "Here is a sample schedule:\n" << endl;
				coursesBST->SampleSchedule();
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			break;
			// Checking and validating Binary Search Tree
		case 3:
			// If valid search/display course info
			if (coursesBST != nullptr && success) {
				if (courseId.length() == 0) {
					cout << "What course do you want to know about? ";
					cin >> courseId;
					for (auto& userChoice : courseId) 
						userChoice = toupper(userChoice);
				}
				coursesBST->CourseInformation(courseId);
				cout << endl;
			}
			else
				cout << "Load courses first - option 1\n" << endl;
			courseId = "";
			break;
			// If Invalid option 
		default:
			if (userChoice != 9)
				cout << choice << " is not a valid option\n" << endl;
			break;
		}
	}
	cout << "\nThank you for using the course planner!" << endl;
	return 0;
}