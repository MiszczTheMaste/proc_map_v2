// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include "include/effolkronium/random.hpp"
#include <string>
#include <math.h>
using Random = effolkronium::random_static;

using namespace std;
string tet;

class Tile {
	public:
		bool walkable;
		bool active;
		bool notSet;
		bool startPoint;
		bool endPoint;
};
class Dbg {
	public:
		int mapPoint[2];
		int startPoint[2];
		int endPoint[2];
};

Dbg pointsCord;
Tile mapa[16][16];
vector<vector<int>> activeTiles;
vector<vector<int>> futureTiles; //? xDD
int cycles = 0;
int fail = 0;

vector<double> smallestInVector(vector<vector<double>> vectorxD) {
	double smallest = vectorxD[0][0];
	vector<double> closestPoint = vectorxD[0];
	for (int i = 0; i < vectorxD.size(); i++) {
		if (vectorxD[i][0] < smallest) {
			smallest = vectorxD[i][0];
			closestPoint = vectorxD[i];
		}
	}
	return closestPoint;
}

void connectPoints(vector<int> point, vector<double> closestPoint) {//point[] 0 - y; 1 -x
	if (point[1] == 15 || point[1] == 0) {
		for (int i = 0; i <= abs(point[0] - closestPoint[1]); i++) {
			if (point[0] > closestPoint[1]) {
				mapa[(int)closestPoint[1] + i][(int)closestPoint[2]].walkable = true;
			}
			else
			{
				mapa[(int)closestPoint[1] - i][(int)closestPoint[2]].walkable = true;
			}

		}
		for (int g = 0; g <= abs(point[1] - closestPoint[2]); g++) {
			if (point[1] > closestPoint[2]) {
				mapa[point[0]][point[1] - g].walkable = true;
			}
			else
			{
				mapa[point[0]][point[1] + g].walkable = true;
			}

		}
	}
	else {
		for (int i = 0; i <= abs(point[0] - closestPoint[1]); i++) {
			if (point[0] > closestPoint[1]) {
				mapa[point[0] - i][point[1]].walkable = true;
			}
			else
			{
				mapa[point[0] + i][point[1]].walkable = true;
			}
		}
		for (int g = 0; g <= abs(point[1] - closestPoint[2]); g++) {
			if (point[1] > closestPoint[2]) {
				mapa[(int)closestPoint[1]][(int)closestPoint[2] + g].walkable = true;
			}
			else
			{
				mapa[(int)closestPoint[1]][(int)closestPoint[2] - g].walkable = true;
			}
		}
	}
	
}
void generateMap() {
	for (int i = 0; i < 16; i++) {
		for (int g = 0; g < 16; g++) {
			mapa[i][g].walkable = false;
			mapa[i][g].active = false;
			mapa[i][g].notSet = true;
			mapa[i][g].startPoint = false;
			mapa[i][g].endPoint = false;
		}
	}
	vector<int> cords;
	vector<int> cordsStart;
	vector<int> cordsEnd;
	switch (Random::get<int>(0, 3))
	{
	case 0:
		cordsStart = { Random::get<int>(2, 14), 0 }; 
		switch (Random::get<int>(0, 2))
		{
		case 0:
			cordsEnd = { Random::get<int>(2, 14), 15 }; 
			break;
		case 1:
			cordsEnd = { 15, Random::get<int>(2, 14) }; 
			break;
		case 2:
			cordsEnd = { 0, Random::get<int>(2, 14) }; 
			break;
		}
		break;
	case 1:
		cordsStart = { 15, Random::get<int>(2, 14) }; 
		switch (Random::get<int>(0, 2))
		{
		case 0:
			cordsEnd = { 0, Random::get<int>(2, 14) }; 
			break;
		case 1:
			cordsEnd = { Random::get<int>(2, 14), 0 }; 
			break;
		case 2:
			cordsEnd = { Random::get<int>(2, 14), 15 }; 
			break;
		}
		break;
	case 2:
		cordsStart = { Random::get<int>(2, 14), 15 }; 
		switch (Random::get<int>(0, 2))
		{
		case 0:
			cordsEnd = { 0, Random::get<int>(2, 14) }; 
			break;
		case 1:
			cordsEnd = { 15, Random::get<int>(2, 14) }; 
			break;
		case 2:
			cordsEnd = { Random::get<int>(2, 14), 0 }; 
			break;
		}
		break;
	case 3:
		cordsStart = { 0, Random::get<int>(2, 14) };
		switch (Random::get<int>(0, 2))
		{
		case 0:
			cordsEnd = { Random::get<int>(2, 14), 15 }; 
			break;
		case 1:
			cordsEnd = { Random::get<int>(2, 14), 0 }; 
			break;
		case 2:
			cordsEnd = { 15, Random::get<int>(2, 14) }; 
			break;
		}
		break;

	}
	mapa[cordsStart[0]][cordsStart[1]].walkable = true;
	mapa[cordsStart[0]][cordsStart[1]].startPoint = true;
	mapa[cordsEnd[0]][cordsEnd[1]].walkable = true;
	mapa[cordsEnd[0]][cordsEnd[1]].endPoint = true;
	cords = { Random::get<int>(2, 13), Random::get<int>(2, 13) }; //0 y-axis 1 x-axis
	mapa[cords[0]][cords[1]].walkable = true;
	mapa[cords[0]][cords[1]].active = true;
	activeTiles.push_back(cords);

	pointsCord.endPoint[0] = cordsEnd[0];
	pointsCord.endPoint[1] = cordsEnd[1];
	pointsCord.startPoint[0] = cordsStart[0];
	pointsCord.startPoint[1] = cordsStart[1];
	pointsCord.mapPoint[0] = cords[0];
	pointsCord.mapPoint[1] = cords[1];


	while (true) {
		for (int i = 0; i < activeTiles.size(); i++) {
			if (activeTiles[i][0] < 14 && activeTiles[i][1] < 14 && activeTiles[i][0] > 1 && activeTiles[i][1] > 1) { //chyba nie działa but not sure
				if (mapa[activeTiles[i][0] - 1][activeTiles[i][1]].notSet == true) {
					bool kkk = Random::get<bool>();
					mapa[activeTiles[i][0] - 1][activeTiles[i][1]].notSet = false;
					if (kkk == true) {
						futureTiles.push_back({ activeTiles[i][0] - 1 , activeTiles[i][1] });
						mapa[activeTiles[i][0] - 1][activeTiles[i][1]].walkable = kkk;
						mapa[activeTiles[i][0] - 1][activeTiles[i][1]].active = kkk;
					}
				}
				if (mapa[activeTiles[i][0]][activeTiles[i][1] + 1].notSet == true) {
					bool kkk = Random::get<bool>();	
					mapa[activeTiles[i][0]][activeTiles[i][1] + 1].notSet = false;
					if (kkk == true) {
						futureTiles.push_back({ activeTiles[i][0] , activeTiles[i][1] + 1 });
						mapa[activeTiles[i][0]][activeTiles[i][1] + 1].walkable = kkk;
						mapa[activeTiles[i][0]][activeTiles[i][1] + 1].active = kkk;
					}
				}
				if (mapa[activeTiles[i][0] + 1][activeTiles[i][1]].notSet == true) {
					bool kkk = Random::get<bool>();
					
					mapa[activeTiles[i][0] + 1][activeTiles[i][1]].notSet = false;
					if (kkk == true) {
						futureTiles.push_back({ activeTiles[i][0] + 1 , activeTiles[i][1] });
						mapa[activeTiles[i][0] + 1][activeTiles[i][1]].walkable = kkk;
						mapa[activeTiles[i][0] + 1][activeTiles[i][1]].active = kkk;
					}
				}
				if (mapa[activeTiles[i][0]][activeTiles[i][1] - 1].notSet == true) {
					bool kkk = Random::get<bool>();
					mapa[activeTiles[i][0]][activeTiles[i][1] - 1].notSet = false;
					if (kkk == true) {
						futureTiles.push_back({ activeTiles[i][0] , activeTiles[i][1] - 1 });
						mapa[activeTiles[i][0]][activeTiles[i][1] - 1].walkable = kkk;
						mapa[activeTiles[i][0]][activeTiles[i][1] - 1].active = kkk;
					}
				}
			}
		}
		activeTiles.clear();
		activeTiles = futureTiles;
		futureTiles.clear();
		cycles++;
		vector<vector<double>> distStart;
		vector<vector<double>> distEnd;
		distStart.clear();
		if (cycles > 30) {
			cycles = 0;
			for (int i = 1; i < 15; i++) {
				for (int g = 1; g < 15; g++) {// i = y ; g = x
					if (mapa[i][g].walkable == true && mapa[i][g].startPoint == false && mapa[i][g].endPoint == false) {
						vector<double> kkk;
						kkk.push_back(sqrt(pow((cordsStart[1] - g), 2) + pow((-cordsStart[0] -(-i)), 2)));
						kkk.push_back(i);
						kkk.push_back(g);
						distStart.push_back(kkk);
						vector<double> dong;
						dong.push_back(sqrt(pow((cordsEnd[1] - g), 2) + pow((-cordsEnd[0] - (-g)), 2)));
						dong.push_back(i);
						dong.push_back(g);
						distEnd.push_back(dong);
					}
				}
			}
			connectPoints(cordsStart, smallestInVector(distStart));
			connectPoints(cordsEnd, smallestInVector(distEnd));
			break;
		}
	}
}





void drawMap() {
	for (auto &mapTile : mapa) {
		for (int i = 0; i<16; i++) {
			if (mapTile[i].walkable == true) {
				if (mapTile[i].startPoint == true) {
					cout << "S";
				}
				else if(mapTile[i].endPoint == true) {
					cout << "E";
				}
				else {
					cout << "+";
				}
			}
			else {
				cout << "/";
			}
		}
		cout << endl;
	}
	cout << "1 to exit, anything else to continue";
	cout << endl;
	cin >> tet;
}
bool checkMap() {
	int tileCount = 0;
	bool valid = true;
	for (int i = 0; i<16; i++) {
		for (int g = 0; g<16; g++) {
			if (mapa[i][g].walkable == true) {
				if (mapa[i - 1][g].walkable != true) {
					valid = false;
				}
				if (mapa[i + 1][g].walkable != true) {
					valid = false;
				}
				if (mapa[i][g - 1].walkable != true) {
					valid = false;
				}
				if (mapa[i][g + 1].walkable != true) {
					valid = false;
				}
				tileCount++;
			}
		}
	}
	if (tileCount < 40 || tileCount > 200 && valid == true) {
		return false;
	}
	else {
		return true;
	}
}
int main()
{

	while (true) {
		if (tet == "1") {
			return 0;
		}
		else {
			generateMap();
			if (checkMap() == true) {
				fail = 0;
				cout << endl;
				cout << "---*---*---*---";
				cout << endl;
				cout << "Start point: ";
				cout << pointsCord.startPoint[0];
				cout << " , ";
				cout << pointsCord.startPoint[1];
				cout << endl;
				cout << "End point: ";
				cout << pointsCord.endPoint[0];
				cout << " , ";
				cout << pointsCord.endPoint[1];
				cout << endl;
				cout << "Map Point: ";
				cout << pointsCord.mapPoint[0];
				cout << " , ";
				cout << pointsCord.mapPoint[1];
				cout << endl;
				drawMap();
			}
			else {
				fail++;
				cout << endl;
				cout << "Failed attempts to generate map: ";
				cout << fail;
				cout << endl;
			}
		}
	}
	/*
	generateMap();
	drawMap();
	while (true) {
		if (tet == "1") {
			return 0;
		}
		else {
			generateMap();
				drawMap();
			
		}
	}*/
}

