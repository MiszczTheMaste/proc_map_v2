// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <random>
#include "include/effolkronium/random.hpp"
#include <string>
using Random = effolkronium::random_static;

using namespace std;
string tet;

class Tile {
	public:
		bool walkable;
		bool active;
		bool notSet;
};

Tile mapa[16][16];
vector<vector<int>> activeTiles;
vector<vector<int>> futureTiles; //? xDD
int cycles = 0;
int fail = 0;

void generateMap() {
	for (int i = 0; i < 16; i++) {
		for (int g = 0; g < 16; g++) {
			mapa[i][g].walkable = false;
			mapa[i][g].active = false;
			mapa[i][g].notSet = true;
		}
	}
	vector<int> cords = { Random::get<int>(2, 14), Random::get<int>(2, 14) }; //0 x-axis 1 y-axis
	mapa[cords[0]][cords[1]].walkable = true; 
	mapa[cords[0]][cords[1]].active = true; 
	activeTiles.push_back(cords);

	while (true) {
		for (int i = 0; i < activeTiles.size(); i++) {
			if (activeTiles[i][0] < 15 && activeTiles[i][1] < 15 && activeTiles[i][0] > 1 && activeTiles[i][1] > 1) { //chyba nie działa but not sure
				if (mapa[activeTiles[i][0] - 1][activeTiles[i][1]].notSet == true) {
					bool kkk = Random::get<bool>();
					mapa[activeTiles[i][0] - 1][activeTiles[i][1]].walkable = kkk;
					mapa[activeTiles[i][0] - 1][activeTiles[i][1]].active = kkk;
					mapa[activeTiles[i][0] - 1][activeTiles[i][1]].notSet = false;
					if (kkk == true) {
						futureTiles.push_back({ activeTiles[i][0] - 1 , activeTiles[i][1] });
					}
				}
				if (mapa[activeTiles[i][0]][activeTiles[i][1] + 1].notSet == true) {
					bool kkk = Random::get<bool>();
					mapa[activeTiles[i][0]][activeTiles[i][1] + 1].walkable = kkk;
					mapa[activeTiles[i][0]][activeTiles[i][1] + 1].active = kkk;
					mapa[activeTiles[i][0]][activeTiles[i][1] + 1].notSet = false;
					if (kkk == true) {
						futureTiles.push_back({ activeTiles[i][0] , activeTiles[i][1] + 1 });
					}
				}
				if (mapa[activeTiles[i][0] + 1][activeTiles[i][1]].notSet == true) {
					bool kkk = Random::get<bool>();
					mapa[activeTiles[i][0] + 1][activeTiles[i][1]].walkable = kkk;
					mapa[activeTiles[i][0] + 1][activeTiles[i][1]].active = kkk;
					mapa[activeTiles[i][0] + 1][activeTiles[i][1]].notSet = false;
					if (kkk == true) {
						futureTiles.push_back({ activeTiles[i][0] + 1 , activeTiles[i][1] });
					}
				}
				if (mapa[activeTiles[i][0]][activeTiles[i][1] - 1].notSet == true) {
					bool kkk = Random::get<bool>();
					mapa[activeTiles[i][0]][activeTiles[i][1] - 1].walkable = kkk;
					mapa[activeTiles[i][0]][activeTiles[i][1] - 1].active = kkk;
					mapa[activeTiles[i][0]][activeTiles[i][1] - 1].notSet = false;
					if (kkk == true) {
						futureTiles.push_back({ activeTiles[i][0] , activeTiles[i][1] - 1 });
					}
				}
			}
		}
		activeTiles.clear();
		activeTiles = futureTiles;
		futureTiles.clear();
		cycles++;
		if (cycles > 20) {
			cycles = 0;
			break;
		}
	}
}

void drawMap() {
	for (auto &mapTile : mapa) {
		for (int i = 0; i<16; i++) {
			if (mapTile[i].walkable == true) {
				cout << "+";
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
				drawMap();
			}
			else {
				fail++;
				cout << "Failed attempts to generate map: ";
				cout << fail;
				cout << endl;
			}
		}
	}
}

