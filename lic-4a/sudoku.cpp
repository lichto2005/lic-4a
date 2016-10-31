// Declarations and functions for the Sudoku project

#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>

using namespace std;

typedef int valueType; // The type of the value in a cell
const int blank = 0;  // Indicates that a cell is blank

const int squareSize = 3;  //  The number of cells in a small square
						   //  (usually 3).  The board has
						   //  squareSize^2 rows and squareSize^2
						   //  columns.

const int boardSize = squareSize * squareSize;

const int minValue = 1;
const int maxValue = 9;

int numSolutions = 0;

int squareNumber(int i, int j);
ostream &operator<<(ostream &ostr, vector<int> &v);

class board
	// Stores the entire Sudoku board
{
public:
	board(int);
	void clear();
	void initialize(ifstream &fin);
	void print();
	void printConflicts();
	bool isBlank(int, int);
	valueType getCell(int, int);
	void  setCell(int i, int j, valueType val);
	void clearCell(int i, int j);
	bool checkConflicts(int i, int j, valueType val);
	bool isSolved();
private:

	// The following matrices go from 1 to boardSize in each
	// dimension.  I.e. they are each (boardSize+1) X (boardSize+1)
	matrix<bool> conflictsRows;
	matrix<bool> conflictsCols;
	matrix<bool> conflictsSq;

	matrix<valueType> value;
};
board::board(int sqSize)
	: value(boardSize + 1, boardSize + 1),
	conflictsRows(boardSize + 1, boardSize + 1),
	conflictsCols(boardSize + 1, boardSize + 1),
	conflictsSq(boardSize + 1, boardSize + 1)
	// board constructor
{
}

void board::clear()
// clear the entire board.
{
	for (int i = 1; i < boardSize + 1; i++)
	{
		for (int j = 1; j < boardSize + 1; j++)
		{
			conflictsRows[i][j] = false;
			conflictsCols[i][j] = false;
			conflictsSq[i][j] = false;
			value[i][j] = blank;
		}
	}
}

void  board::setCell(int i, int j, valueType val)
// set cell i,j to val and update conflicts
{
	value[i][j] = val;

	conflictsRows[i][val] = true;
	conflictsCols[j][val] = true;
	conflictsSq[squareNumber(i, j)][val] = true;
}

void board::clearCell(int i, int j)
// assumes that conflict is not caused by 2 incorrect numbers
{
	valueType val = value[i][j];
	value[i][j] = blank;
	conflictsRows[i][val] = false;
	conflictsCols[j][val] = false;
	conflictsSq[squareNumber(i, j)][val] = false;

}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
	char ch;

	clear();
	for (int i = 1; i <= boardSize; i++)
		for (int j = 1; j <= boardSize; j++)
		{
			fin >> ch;

			// If the read char is not Blank
			if (ch != '.')
			{
				setCell(i, j, ch - '0');   // Convert char to int
			}
		}
}

int squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to boardSize
{
	// Note that (int) i/squareSize and (int) j/squareSize are the x-y
	// coordinates of the square that i,j is in.  

	return squareSize * ((i - 1) / squareSize) + (j - 1) / squareSize + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
	for (int i = 0; i < v.size(); i++)
		ostr << v[i] << " ";
	ostr << endl;
	return ostr;
}

ostream &operator<<(ostream &ostr, vector<bool> &v)
{
	for (int i = 1; i < v.size(); i++)
		ostr << v[i] << " ";
	ostr << endl;
	return ostr;
}

valueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
	if (i >= 1 && i <= boardSize && j >= 1 && j <= boardSize)
		return value[i][j];
	else
		throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
	if (i < 1 || i > boardSize || j < 1 || j > boardSize)
		throw rangeError("bad value in setCell");

	return getCell(i, j) == blank ? true : false;
}

void board::print()
// Prints the current board.
{
	for (int i = 1; i <= boardSize; i++)
	{
		if ((i - 1) % squareSize == 0)
		{
			cout << " -";
			for (int j = 1; j <= boardSize; j++)
				cout << "---";
			cout << "-";
			cout << endl;
		}
		for (int j = 1; j <= boardSize; j++)
		{
			if ((j - 1) % squareSize == 0)
				cout << "|";
			if (!isBlank(i, j))
				cout << " " << getCell(i, j) << " ";
			else
				cout << "   ";
		}
		cout << "|";
		cout << endl;
	}

	cout << " -";
	for (int j = 1; j <= boardSize; j++)
		cout << "---";
	cout << "-";
	cout << endl;
}

void board::printConflicts()
{
	cout << "Row conflicts:\n";
	for (int i = 1; i < boardSize + 1; i++)
		cout << i << ": " << conflictsRows[i];

	cout << "Column conflicts:\n";
	for (int i = 1; i < boardSize + 1; i++)
		cout << i << ": " << conflictsCols[i];

	cout << "Square conflicts:\n";
	for (int i = 1; i < boardSize + 1; i++)
		cout << i << ": " << conflictsSq[i];
}

bool board::checkConflicts(int i, int j, valueType val)
{
	if (i < 1 || i > boardSize || j < 1 || j > boardSize || val < 1 || val > boardSize)
		throw rangeError("bad value in checkConflicts");
	return conflictsRows[i][val] || conflictsCols[j][val] || conflictsSq[squareNumber(i, j)][val];
}

bool board::isSolved()
{
	for (int i = 1; i < boardSize + 1; i++)
	{
		for (int j = 1; j < boardSize + 1; j++)
		{
			if (value[i][j] == blank) return false;
		}
	}
	return true;
}

int main()
{
	ifstream fin;

	// Read the sample grid from the file.
	string fileName = "sudoku1.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		board b1(squareSize);

		while (fin && fin.peek() != 'Z')
		{
			b1.initialize(fin);
			b1.print();
			b1.printConflicts();
			cout << "Conflict if 1 added to 1,1: " << b1.checkConflicts(1, 1, 1) << endl;
			bool solved = b1.isSolved();
			if (solved)
				cout << "Solved!\n";
			else cout << "Not solved.\n";
		}
	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl;
		exit(1);
	}

	return 0;
}

