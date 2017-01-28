#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
using namespace std;

#define inf 1000
#define x 999
#define invalid 998

bool checkNeedBroadcast(bool* arr, int size) {
	for (int i = 0; i < size; ++i) {
		if (arr[i]){
			return true;
		}
	}
	return false;
}

int main(int argc, char *argv[]) {

	string inputFileName = argv[1];
    string outputFileName = argv[2];  

    ifstream inputFile( inputFileName.c_str() );
	ofstream outputFile( outputFileName.c_str() );

    string line;
    int lineIndex = 0;
    int arrSize = 0;
    int ** array;

    getline(inputFile, line);
	arrSize = atoi(line.c_str());
	array = new int*[arrSize];
	for(int i = 0; i < arrSize; ++i) {
		array[i] = new int[arrSize];
	}

    while(!inputFile.eof()){
		getline(inputFile, line);		
		int index1 = 0;
		int index2 = 0;
		int numIndex = 0;
		string str;
		while (index2 != line.length()) {
			index1 = line.find_first_not_of(" ", index2);
			index2 = line.find_first_of(" ", index1 + 1);
			if (index2 == -1){
				index2 = line.length();
			}
			str = line.substr(index1, index2 - index1);
			array[lineIndex][numIndex] = atoi(str.c_str());
			++numIndex;
		}
		++lineIndex;
    }

	int T = 0;
	// init adjecent table 
	vector<int **> arrayTable;
	int ** adjecentTable;
	adjecentTable = new int*[arrSize];
	for(int i = 0; i < arrSize; ++i) {
		adjecentTable[i] = new int[arrSize];
	}

	for (int i = 0; i < arrSize; ++i) {
		for (int j = 0; j < arrSize; ++j) {
			if (j == i) {
				adjecentTable[i][j] = 0;
			}
			else if (j!=i && array[i][j] == 0) {
				adjecentTable[i][j] = invalid;
			}
			else {
				adjecentTable[i][j] = 1;
			}
		}
	}

	// init tables
	for (int i = 0; i < arrSize; ++i) {
		int ** tempArr = new int*[arrSize];
		for(int k = 0; k < arrSize; ++k) {
			tempArr[k] = new int[arrSize];
		}

		int row[arrSize]; 
		
		for (int j = 0; j < arrSize; ++j) {
			row[j] = array[i][j];
			if (array[i][j] == 0 && i != j) {
				row[j] = inf;
			}
		}

		
		for (int j = 0; j < arrSize; ++j) {
			for (int k = 0; k < arrSize; ++k) {
				if (j == i) {
					tempArr[j][k] = x;
				}
				else if (k == i) {
					tempArr[j][k] = x;
				} 
				else if (j == k && row[j] != inf) {
					tempArr[j][k] = row[j];
				}
				else if (row[k] == inf){
					tempArr[j][k] = invalid;
				}
				else {
					tempArr[j][k] = inf;
				}
			}
		}
		arrayTable.push_back(tempArr);
	}

	// init newArrayTable 
	vector<int **> newArrayTable;
	for (int i = 0; i < arrayTable.size(); ++i) {
		int ** tempArr = new int*[arrSize];
		for(int k = 0; k < arrSize; ++k) {
			tempArr[k] = new int[arrSize];
		}

		for (int j = 0; j < arrSize; ++j) {
			for (int k = 0; k < arrSize; ++k) {
				tempArr[j][k] = arrayTable[i][j][k];
			}
		}
		newArrayTable.push_back(tempArr);
	}

	// an array to indicate which matrix need to broadcast
	bool needBroadcast[arrSize];
	bool needBroadcastNext[arrSize];
	for (int i = 0; i < arrSize; ++i) {
		needBroadcast[i] = true;
		needBroadcastNext[i] = false;
	}


	while (checkNeedBroadcast(needBroadcast, arrSize)){
		// show all matrixes in old array table
		outputFile << "T = " << T << endl;
		++ T;
		for (int i = 0; i < arrayTable.size(); ++i){
			outputFile << "From node " << i << endl;

			outputFile << "(";
			for(int j = 0; j < arrSize; ++j) {
				int temp = arrayTable[i][j][0];
				for (int k = 0; k < arrSize; ++k) {
					if (arrayTable[i][j][k] < temp) {
						temp = arrayTable[i][j][k];
					}
				}

				if (temp == invalid) {
					outputFile << "inf";
				}
				else if (i == j) {
					outputFile << 0;
				}
				else {
					outputFile << temp; 
				}

				if (j != arrSize - 1) {
					outputFile << ", ";
				}
			}
			outputFile << ")" << endl;

			for (int j = 0; j < arrSize; ++j){
				for (int k = 0; k < arrSize; ++k){
					if(arrayTable[i][j][k] == x) {
						outputFile << setw(6) << 'x';
					}
					else if (arrayTable[i][j][k] == inf) {
						outputFile << setw(6) << " ";
					}
					else if (arrayTable[i][j][k] == invalid) {
						outputFile << setw(6) << " ";
					}
					else {
						outputFile << setw(6) << arrayTable[i][j][k];
					}
				}	
				outputFile << endl;
			}
		}

		// propagate info between tables
		for (int i = 0; i < arrayTable.size(); ++i) {
			for (int j = 0; j < arrSize; ++j) {
				if (adjecentTable[i][j] == 1 && needBroadcast[i]) {
					for (int k = 0; k < arrSize; ++k) {
						for (int l = 0; l < arrSize; ++l) {
							if (arrayTable[i][k][l] != x && arrayTable[i][k][l] != inf && arrayTable[i][k][l] != invalid) {
								if (arrayTable[j][k][i] != x && arrayTable[j][k][i] != invalid) {
									// cout << "propagate from: " << i << " to " << j << " that " << i << " can go to " << k << " from " << l << " distance: " << arrayTable[i][k][l] + arrayTable[j][i][i] << endl; 
									if (arrayTable[j][k][i] > arrayTable[j][i][i] + arrayTable[i][k][l] && newArrayTable[j][k][i] > arrayTable[j][i][i] + arrayTable[i][k][l]) {
										newArrayTable[j][k][i] = arrayTable[j][i][i] + arrayTable[i][k][l];
										needBroadcastNext[j] = true;
									}
								}
							}
						}
					}
				}
			}
		}	

		// sychronize tables
		for (int i = 0; i < arrayTable.size(); ++i) {
			for (int j = 0; j < arrSize; ++j) {
				for (int k = 0; k < arrSize; ++k) {
					arrayTable[i][j][k] = newArrayTable[i][j][k];
				}
			}
		}

		for (int i = 0; i < arrSize; ++i) {
			needBroadcast[i] = needBroadcastNext[i];
			needBroadcastNext[i] = false;
		}
	}
}