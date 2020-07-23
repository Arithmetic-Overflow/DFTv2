#define _USE_MATH_DEFINES
 
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <chrono>

using namespace std;
using namespace std::chrono;


// euclidian distance
// takes two vectors with x and y coordinates
// returns the distance between them
float dist(vector<int> p1, vector<int> p2){
	return sqrt(pow(p1[0]-p2[0], 2) + pow(p1[1]-p2[1], 2));
}


// turns a csv of lines into a 2D vector
// takes a file name
// returns all the data as a 2D vector
vector<vector<int>> processFile(string fileString){

	vector<string> lines;
	vector<vector<int>> parentVector;

	int breakIndex = fileString.find("\n");
	while(breakIndex != -1) {
		string childString;
		childString = fileString.substr(0, breakIndex);
		fileString = fileString.substr(breakIndex + 1, fileString.size() - breakIndex);
		breakIndex = fileString.find("\n");

		lines.push_back(childString);
	}

	lines.push_back(fileString);

	for(string superString : lines){
		vector<int> tempVector;

		int breakIndex = superString.find(",");
		while(breakIndex != -1) {
			string childString;
			childString = superString.substr(0, breakIndex);
			superString = superString.substr(breakIndex + 1, superString.size() - breakIndex);
			breakIndex = superString.find(",");

			tempVector.push_back(stof(childString));
		}

		tempVector.push_back(stof(superString));
		parentVector.push_back(tempVector);
	}

	return parentVector;
}


// reorganizes the edges into a path
// reads the edges from a file
// writes the path into a different file
void computePath(){

	cout << "Processing The Pathing Of The Edges..." << endl;

	ifstream edgesFile("./ImageData/Edges.csv");
	string edgesFileRead;
	getline(edgesFile, edgesFileRead, '\0');
	edgesFileRead = edgesFileRead.substr(0, edgesFileRead.length() - 1);

	// reads in and processes the edges from the file
	vector<vector<int>> edgesList = processFile(edgesFileRead);

	ofstream pathFile("./ImageData/Path.csv");

	// maximum distance to instantly flag a point as the next edges in the path
	float threshold = 3;

	int n = edgesList.size();

	vector<int> point = edgesList[0];
	edgesList.erase(edgesList.begin());

	for(int i = 0; i < n; i++){
		float minDist = numeric_limits<float>::infinity();
		float currentDist;
		int minIndex;
		

		for(int j = 0; j < edgesList.size(); j++){
			currentDist = dist(point, edgesList[j]);

			if(currentDist < minDist){
				minDist = currentDist;
				minIndex = j;

				if(minDist < threshold){
					break;
				}
			}
		}

		point = edgesList[minIndex];
		pathFile << point[0] << "," << point[1] << endl;
		edgesList.erase(edgesList.begin() + minIndex);
	}

	pathFile.close();

	cout << "Finished Computing Path." << endl << endl;
}


// just a different way to do computerPath()
void computePath2(){

	cout << "Processing The Pathing Of The Edges..." << endl;

	ifstream edgesFile("./ImageData/Edges.csv");
	string edgesFileRead;
	getline(edgesFile, edgesFileRead, '\0');
	edgesFileRead = edgesFileRead.substr(0, edgesFileRead.length() - 1);

	vector<vector<int>> edgesList = processFile(edgesFileRead);

	ofstream pathFile("./ImageData/Path.csv");

	float threshold = 3;

	vector<vector<int>> path;

	vector<int> point = edgesList[0];
	edgesList.erase(edgesList.begin());

	pathFile << point[0] << "," << point[1] << endl;

	while(edgesList.size()){
		int nextEdgeIndex;
		float minDist = numeric_limits<float>::infinity();
		float currentDist;

		for(int i = 0; i < edgesList.size(); i++){
			currentDist = dist(point, edgesList[i]);

			if(currentDist < minDist){
				minDist = currentDist;
				nextEdgeIndex = i;

				if(minDist < threshold){
					break;
				}
			}
		}

		point = edgesList[nextEdgeIndex];
		pathFile <<  point[0] << "," << point[1] << endl;
		edgesList.erase(edgesList.begin() + nextEdgeIndex);
	}

	pathFile.close();

	cout << "Finished Computing Path." << endl << endl;
}


// takes a sequence of integers representing the x and y coordinates
// calculates the DFT and writes it in a file for the x and y coordinates separately
void writeTransform(vector<int> xSequence, vector<int> ySequence){

	ofstream xTransformFile("./ImageData/DFT_X.csv"), yTransformFile("./ImageData/DFT_Y.csv");

	int N = xSequence.size();
	float thetaNull = 2*M_PI/N;

	for(int k = 0; k < N; k++) {
		float xRe = 0;
		float xIm = 0;
		float yRe = 0;
		float yIm = 0;

		float thetaBase = thetaNull*k;

		for(int n = 0; n < N; n++) {
			float theta = thetaBase*n;
			float cosine = cos(theta);
			float sine = sin(theta);

			xRe += xSequence[n]*cosine;
			xIm -= xSequence[n]*sine;

			yRe += ySequence[n]*cosine;
			yIm -= ySequence[n]*sine;
		}

		xRe /= N;
		xIm /= N;

		yRe /= N;
		yIm /= N;

		float xAmp = sqrt(pow(xRe, 2) + pow(xIm, 2));
		float xPhase = atan2(xIm, xRe);

		float yAmp = sqrt(pow(yRe, 2) + pow(yIm, 2));
		float yPhase = atan2(yIm, yRe);

		xTransformFile << xAmp << "," << k << "," << xPhase << endl;
		yTransformFile << yAmp << "," << k << "," << yPhase << endl;
	}

	xTransformFile.close();
	yTransformFile.close();
}


// reads the path from a file
// transforms the x and y coordinates into a separate sequence
// calls the writeTransform function which calculates and writes it into a file
void computeTransform(){

	cout << "Processing The Fourier Transform Of The Sequenced Path..." << endl;

	ifstream pathFile("./ImageData/Path.csv");
	string pathFileRead;
	getline(pathFile, pathFileRead, '\0');
	pathFileRead = pathFileRead.substr(0, pathFileRead.length() - 1);

	pathFile.close();

	vector<vector<int>> pathData = processFile(pathFileRead);

	vector<int> xVector, yVector;

	for(int i = 0; i < pathData.size(); i++) {
		xVector.push_back(pathData[i][0]);
		yVector.push_back(pathData[i][1]);
	}

	writeTransform(xVector, yVector);

	cout << "Finished Computing Transform." << endl << endl;
}


int main(){

	// makes a note of the time for computing the runtime
	auto start = high_resolution_clock::now();

	computePath();
	computeTransform();

	// uses the end time to calculate the duration
	auto end = high_resolution_clock::now();
	auto duration_s = duration_cast<seconds>(end - start);

	cout << "The Runtime Duration Of The Program Was: ";

	// changes the unit of the duration if necessary
	// prints the duration of the program
	if(duration_s.count() > 60) {
		auto duration_m = duration_cast<minutes>(end - start);

		if(duration_m.count() > 60) {
			auto duration_h = duration_cast<hours>(end - start);

			cout << duration_h.count() << " hours.";

		} else {
			cout << duration_m.count() << " minutes.";

		}

	} else {
		cout << duration_s.count() << " seconds.";

	}

	cout << endl;

	cout << "It Is Safe To Close The Program" << endl;

	cin.get();

	return 0;
}