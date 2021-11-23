#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>  

using namespace std;

vector<vector<float>> parseData(const string& filename);
void featureSearch(vector<vector<float>>& data);
float leaveOneOutCrossValidation(const vector<vector<float>>& data, set<float> currentSet, int featureToAdd);


int main() {
    vector<vector<float>> data = parseData("CS170_Fall_2021_SMALL_data__1.txt");

    featureSearch(data);
}

void featureSearch(vector<vector<float>>& data) {

    set<float> currentSetOfFeatures;

    for (int i = 1; i < data.at(0).size(); ++i) {

        cout << "On the " << i << "th level of the search tree" << endl; 

        int featureToAddAtThisLevel;
        float bestAccuracySoFar = 0;

        for (int k = 1; k < data.at(0).size(); ++k) {

            if (currentSetOfFeatures.count(k) > 0) continue;
            cout << "--Considering adding the " << k << " feature" << endl;

            float accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, k+1);

            if (accuracy > bestAccuracySoFar) {
                bestAccuracySoFar = accuracy;
                featureToAddAtThisLevel = k;
            }

        }

        currentSetOfFeatures.insert(featureToAddAtThisLevel);
        cout << "On level " << i << " added feature " << featureToAddAtThisLevel << " to current set" << endl;

    }
}

float leaveOneOutCrossValidation(const vector<vector<float>>& data, set<float> currentSet, int featureToAdd) {
    return rand() % 101;
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