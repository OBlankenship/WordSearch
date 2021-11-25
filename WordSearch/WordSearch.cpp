//  1. Read an input file containing a matrix of chars, and a list of words.
//  2. Search the matrix for each word in the word list and report the following:
//    a. If word is found or not found in the matrix
//    b. The position at which the word as found in human readable coordinates (1 based indexing)
//    c. The direction in which the word was found


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <conio.h> 
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>
using namespace std;

//Variable Declaration
string filename;
ifstream inputfile;
ofstream outputfile;
vector<string> filecontents;
vector<int> dimensions;
vector<string> wordList;
const int maxRows = 100;
const int maxCols = 100;
char wordMatrix[maxRows][maxCols];
int FoundPositions[maxRows][maxCols];
string directions[8] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW" };
int xCoordinates[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int yCoordinates[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
vector<string> foundWords;
vector<string> notFoundWords;
vector<string> foundDirection;
vector<int> foundDirectionInts;
vector<int> foundX;
vector<int> foundY;
int LastWordPosition;
bool MultipleWordsEnabled = true;
bool ShowWordList = false;

//Function Prototypes
void PromptUser();

//Functions
void MultipleWordController() { //Controls debug option for reporting multiple instances of a found word (Default: True)
	if (MultipleWordsEnabled == true) {
		MultipleWordsEnabled = false;
		cout << "Multiple instances of a word will NOT be reported!";
		Sleep(2000);
	}
	else {
		MultipleWordsEnabled = true;
		cout << "Multiple instances of a word WILL be reported!";
		Sleep(2000);
	}
} //MultipleWordController

void ShowWordListController() { //Controls debug option for showing the word list during operation (Default: False)
	if (ShowWordList == false) {
		ShowWordList = true;
		cout << "Word list WILL be reported";
		Sleep(2000);
	}
	else {
		ShowWordList = false;
		cout << "Word list will NOT be reported";
		Sleep(2000);
	}
} //ShowWordListController

void DebugMode() {
	system("cls");
	cout << "Debug Menu. Press the corresponding key for the desired menu option. \nPress any other key to return to file input prompt." << endl;
	cout << "1. Search Comedy Movies Full\n2. Search Comedy Movies Partial\n3. Disable/Enable reporting multiple found instances of a given word.\n4. Print/Hide word list during operation.\n";
	switch (_getch()) {
	case '1':
		filename = "ComedyMovies-Final2.txt";
		break;
	case '2':
		filename = "ComedyMovies-Partial1.txt";
		break;
	case '3':
		MultipleWordController();
		break;
	case '4':
		ShowWordListController();
		break;
	}
	system("cls");
} //DebugMode

void OpenFile() {
	while (inputfile.is_open() == false) {
		cout << "Please Input File to Read, type quit to quit, or type debug to enter debug menu: ";
		cin >> filename;
		if (filename == "debug" || filename == "Debug") {
			DebugMode();
		}
		if (filename == "quit" || filename == "Quit") {
			exit(0);
		}
		inputfile.open(filename);
		if (inputfile.is_open() == false && filename != "debug" && filename != "Debug") {
			cout << "File name " << filename << " not found!\n";
		}
	}
} //OpenFile

void ReadFile() {
	string line;
	while (getline(inputfile, line))
	{
		char nextChar = line[0];
		if (((nextChar == '#' || line.length() == 0))) {
		}
		else {
			filecontents.push_back(line);
		}
	}
} //ReadFile

void AssignRowsCols() {
	string value;
	istringstream input(filecontents[0]);
	while (getline(input, value, ' ')) {
		dimensions.push_back(stoi(value));
	}
} //AssignRowsCols

void FillMatrix() {
	for (int i = 1; i <= dimensions[0]; i++) {
		for (int j = 0; j < dimensions[1]; j++) {
			wordMatrix[i - 1][j] = filecontents[i][j];
		}
	}
} //FillMatrix

void PrintDimensions() {
	cout << endl;
	cout << "Number of Rows: " << dimensions[0] << endl;
	cout << "Number of Columns: " << dimensions[1] << endl;
} //PrintDimensions

void PrintMatrix() {
	cout << "\nMatrix Contents: " << endl;
	for (int i = 0; i <= dimensions[0]; i++) {
		cout << endl;
		for (int j = 0; j < dimensions[1]; j++) {
			cout << wordMatrix[i][j];
		}
	}
} //PrintMatrix

void PrintPositions() {
	cout << "\nFilled Positions: " << endl;
	for (int i = 0; i <= dimensions[0] - 1; i++) {
		cout << endl;
		for (int j = 0; j < dimensions[1]; j++) {
			cout << FoundPositions[i][j];
		}
	}
	cout << endl;
} //PrintPositions

void GenerateWordLists() {
	cout << endl;
	int startingPosition = dimensions[0] + 1;
	for (int i = startingPosition; i < filecontents.size(); i++) {
		wordList.push_back(filecontents[i]);
	}
} //GenerateWordLists

void PrintWordLists() {
	cout << "List of Words to Search For: \n" << endl;
	for (int i = 0; i < wordList.size(); i++) {
		cout << wordList[i] << endl;
	}
} //PrintWordLists

void SetPosition(int length) {
	int Direction = foundDirectionInts.back();
	int FoundX = foundX.back();
	int FoundY = foundY.back();
	for (int i = 0; i < length; i++) {
		FoundPositions[FoundX + (xCoordinates[Direction] * i)][FoundY + (yCoordinates[Direction] * i)] ++;
	}
} //SetPosition

bool FindConsecutiveLetter(int CurrentX, int CurrentY, string CurrentWord, int PositionInWord, int direction) {
	if (PositionInWord >= CurrentWord.length()) {
		foundDirection.push_back(directions[direction]);
		foundDirectionInts.push_back(direction);
		return true;
	}
	int xSearch = CurrentX + xCoordinates[direction];
	int ySearch = CurrentY + yCoordinates[direction];
	if (wordMatrix[xSearch][ySearch] == CurrentWord[PositionInWord]) {
		if (FindConsecutiveLetter(xSearch, ySearch, CurrentWord, PositionInWord + 1, direction) == true) {
			return true;
		}
	}
	return false;
} //FindConsecutiveLetter

bool FindSecondLetter(int CurrentX, int CurrentY, string CurrentWord, int PositionInWord) {
	for (int i = 0; i < 8; i++) {
		int xSearch = CurrentX + xCoordinates[i];
		int ySearch = CurrentY + yCoordinates[i];
		if (wordMatrix[xSearch][ySearch] == CurrentWord[PositionInWord]) {
			if (FindConsecutiveLetter(xSearch, ySearch, CurrentWord, PositionInWord + 1, i) == true) {
				return true;
			}
		}
	}
	return false;
} //FindSecondLetter

bool FindFirstLetter(string CurrentWord) {
	bool FoundWord = false;
	for (int i = 0; i <= dimensions[0]; i++) {
		for (int j = 0; j < dimensions[1]; j++) {
			if (wordMatrix[i][j] == CurrentWord[0]) {
				if (FindSecondLetter(i, j, CurrentWord, 1) == true) {
					foundX.push_back(i);
					foundY.push_back(j);
					foundWords.push_back(CurrentWord);
					SetPosition(CurrentWord.length());
					FoundWord = true;
					if (MultipleWordsEnabled == false) {
						return FoundWord;
					}
				}
			}
		}
	}
	return FoundWord;
} //FindFirstLetter

string RemoveSpace(string CurrentWord) {
	CurrentWord.erase(remove(CurrentWord.begin(), CurrentWord.end(), ' '), CurrentWord.end());
	return CurrentWord;
} //RemoveSpace

void SearchController() {
	for (int i = 0; i < wordList.size(); i++) {
		string CurrentWord = wordList[i];
		string CurrentWordNoSpace;
		CurrentWordNoSpace = RemoveSpace(CurrentWord);
		if (FindFirstLetter(CurrentWordNoSpace) == true) {

		}
		else {
			notFoundWords.push_back(CurrentWord);
		}
	}
} //SearchController

string StringToUpper(string UserEntry) {
	transform(UserEntry.begin(), UserEntry.end(), UserEntry.begin(), toupper);
	return UserEntry;
}//StringToUpper

void DisplayCustomResult() {
	for (int i = LastWordPosition; i < foundWords.size(); i++) {
		cout << foundWords[i] << " at " << foundX[i] + 1 << " " << foundY[i] + 1 << " facing " << foundDirection[i] << endl;
		LastWordPosition = i + 1;
	}
} //DisplayCustomResult

void CustomSearchController(string UserEntry) {
	string CurrentWordUpper;
	CurrentWordUpper = StringToUpper(UserEntry);
	string CurrentWordNoSpace;
	CurrentWordNoSpace = RemoveSpace(CurrentWordUpper);
	if (FindFirstLetter(CurrentWordNoSpace) == true) {
		DisplayCustomResult();
	}
	else {
		cout << "Word Not Found";
	}
} //CustomSearchController

void PrintResults() {
	cout << "\nFound Words: " << endl;
	for (int i = 0; i < foundWords.size(); i++) {
		cout << foundWords[i] << " at " << foundX[i] + 1 << " " << foundY[i] + 1 << " facing " << foundDirection[i] << endl;
		LastWordPosition = i + 1;
	}
	cout << "\nNot Found Words: " << endl;
	for (int i = 0; i < notFoundWords.size(); i++) {
		cout << notFoundWords[i] << endl;
	}
} //PrintResults

void CustomSearch() {
	cout << "Please enter the word to search for: ";
	string UserEntry;
	cin >> UserEntry;
	CustomSearchController(UserEntry);
	PromptUser();
} //CustomSearch

void PromptUser() { //Prompts user for custom search terms
	cout << "\nWould you like to search for another word within the matrix? (Y/N): \n";
	switch (_getch()) {
	case 'Y':
	case 'y':
		CustomSearch();
		break;
	case 'N':
	case 'n':
		cout << "Thank you for using Matrix Search!\n";
		break;
	default:
		cout << "\nInvalid Entry! Please enter Y/N to indicate if you would like to search for another word.\n";
		PromptUser();
	}
} //PromptUser

void OpenOutput(string outfile) {
	outputfile.open(outfile);
} //OpenOutput

void CloseOutput() {
	outputfile.close();
}

void GeneratePPM1() {
	// This function generates a file called PPMOutput1 that contains a PPM image where the brightness of blue is 
	// determined by the number of movies that have letters found in that position. Positions where letters were not found are black.
	OpenOutput("PPMOutput1.ppm");
	cout << "Exporting PPM File..." << endl;
	srand(time(NULL));
	outputfile << "P3" << endl;
	outputfile << dimensions[1] << " " << dimensions[0] << " 255" << endl;
	for (int i = 0; i <= dimensions[0] - 1; i++) {
		for (int j = 0; j < dimensions[1]; j++) {
			if (FoundPositions[i][j] == 1) {
				int PositionValue = FoundPositions[i][j];
				outputfile << 0 << " " << 0 << " " << "64" << endl;
			}
			else if (FoundPositions[i][j] == 2) {
				outputfile << 0 << " " << 0 << " " << "128" << endl;
			}
			else if (FoundPositions[i][j] >= 3) {
				outputfile << 0 << " " << 0 << " " << "255" << endl;
			}
			else {
				outputfile << "0 0 0" << endl;
			}
		}
	}
	CloseOutput();
} //GeneratePPM1

void GeneratePPM2() {
	// This function generates a file called PPMOutput2 that contains a PPM image where positions where a letter was found
	// are randomly colored. Positions where letters were not found are black.
	OpenOutput("PPMOutput2.ppm");
	cout << "Exporting PPM File..." << endl;
	srand(time(NULL));
	outputfile << "P3" << endl;
	outputfile << dimensions[1] << " " << dimensions[0] << " 255" << endl;
	for (int i = 0; i <= dimensions[0] - 1; i++) {
		for (int j = 0; j < dimensions[1]; j++) {
			if (FoundPositions[i][j] > 0) {
				int PositionValue = FoundPositions[i][j];
				outputfile << rand() % 255 << " " << rand() % 255 << " " << rand() % 255 << endl;
			}
			else {
				outputfile << "0 0 0" << endl;
			}
		}
	}
	CloseOutput();
} //GeneratePPM2

void CloseFile() {
	inputfile.close();
} //CloseFile

int main()
{
	OpenFile();
	ReadFile();
	AssignRowsCols();
	PrintDimensions();
	FillMatrix();
	PrintMatrix();
	GenerateWordLists();
	if (ShowWordList == true) {
		PrintWordLists();
	}
	SearchController();
	PrintResults();
	PromptUser();
	PrintPositions();
	GeneratePPM1();
	GeneratePPM2();
	CloseFile();
}
