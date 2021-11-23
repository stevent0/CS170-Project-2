#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


vector<vector<float>> parseData(const string& filename);


int main() {
    vector<vector<float>> data = parseData("CS170_Fall_2021_SMALL_data__1.txt");

    for (vector<float>& row : data) {
        for (float& x : row) {
            cout << x << " ";
        }
        cout << endl;
    }
}

vector<vector<float>> parseData(const string& filename) {

    ifstream ifs("./data/" + filename);
    vector<vector<float>> data;
    string line;

    if (!ifs.is_open()) {
        cout << "Couldn't open " << filename << endl;
    }


    while( getline(ifs, line) ) {
        istringstream iss(line);
        vector<float> row;
        string s;

        while (iss >> s) {
            row.push_back(stof(s));
        }

        data.push_back(row);
    }

    return data;

}