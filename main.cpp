#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <set>  
#include <limits>
#include <math.h>

using namespace std;

vector<vector<float>> parseData(const string& filename);
void featureSearch(const vector<vector<float>>& data );
float leaveOneOutCrossValidation(const vector<vector<float>>& data, const set<int>& currentSet, int featureToAdd);


int main() {
    vector<vector<float>> data = parseData("Ver_2_CS170_Fall_2021_LARGE_data__27.txt");

    featureSearch(data);
}

void featureSearch(const vector<vector<float>>& data) {

    set<int> currentSetOfFeatures;
    vector<int> answer;
    float bestAccuracy = 0;
    set<int> bestSet;

    for (int i = 1; i < data.at(0).size(); ++i) {

        cout << "On the " << i << "th level of the search tree" << endl; 

        int featureToAddAtThisLevel = -1;
        float bestAccuracySoFar = 0;

        for (int k = 1; k < data.at(0).size(); ++k) {

            if (currentSetOfFeatures.count(k) > 0) continue;
            cout << "--Considering adding the " << k << " feature" << endl;

            float accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, k);
            cout << accuracy << endl;

            if (accuracy > bestAccuracySoFar) {
                bestAccuracySoFar = accuracy;
                featureToAddAtThisLevel = k;
    
            }

        }

        currentSetOfFeatures.insert(featureToAddAtThisLevel);
        if (bestAccuracySoFar > bestAccuracy) {
            bestAccuracy = bestAccuracySoFar;
            bestSet = currentSetOfFeatures;
             cout << "BEST SET SO FAR: ";
            for (auto it = bestSet.begin(); it != bestSet.end(); it++) {
                cout << *it << " ";
            }
            cout << endl;
        }
    
        // cout << "On level " << i << " added feature " << featureToAddAtThisLevel << " to current set" << endl;

    }

    for (auto it = bestSet.begin(); it != bestSet.end(); it++) {
        cout << *it << " ";
    }
    cout << "accuracy: " << bestAccuracy << endl;
    cout << endl;
}

float leaveOneOutCrossValidation(const vector<vector<float>>& data, const set<int>& currentSet, int featureToAdd) {

    int numberCorrectlyClassified = 0;

    for (int i = 0; i < data.size(); ++i) { //Loop through each row of data from dataset:  data[i]

        int objectToClassify = static_cast<int>(data.at(i).at(0));
        float nearestNeighborDistance = numeric_limits<float>::max();
        float nearestNeighborLocation = numeric_limits<float>::max();
        int nearestNeighborLabel = -1;

        // cout << "Looping over i, at the " << i << " location" << endl;
        // cout << "The " << i << "th object is in class " << classNumber << endl;

        for (int k = 0; k < data.size(); ++k) {   //Loop through each row of data from dataset:  data[k]

            if (k != i) { //Find a different row of data[i] that's not data[k]

                // cout << "Ask if " << i << " is the nearest neighbor with " << k << endl;

                float triangle_sides = 0; //triangle_sides represents //a^2 + b^2 + c^2 + d^2 + ...
                float distance;
                

                //Calculate the distance between data[i] and data[k]
                for (int j = 1; j < data.at(0).size(); ++j) {  

                    //only consdering features in current set + featuretoadd
                    if (currentSet.count(j) > 0 || j == featureToAdd) {
                        vector<float> row_i = data.at(i); 
                        vector<float> row_k = data.at(k);
                        triangle_sides += pow(row_i.at(j) - row_k.at(j), 2); 
                    }
                }
                distance = sqrt(triangle_sides);  // sqrt(a^2 + b^2 + c^2 + d^2 + ...)

                //If data[k] is closest to data[i] then save it
                if (distance < nearestNeighborDistance) {
                    nearestNeighborDistance = distance;
                    nearestNeighborLocation = k;
                    nearestNeighborLabel = static_cast<int>(data.at(k).at(0));
                }
            }
        }

        //object classification already given in file. Check if it the closest data[k] has the same classification.
        if (objectToClassify == nearestNeighborLabel) numberCorrectlyClassified += 1;
    }

    cout << numberCorrectlyClassified << ", " << static_cast<float>(data.size()) << endl;
    return numberCorrectlyClassified / static_cast<float>(data.size());
}

vector<vector<float>> parseData(const string& filename) {

    ifstream ifs("./data/" + filename);
    vector<vector<float>> data;
    string line;

    if (!ifs.is_open()) {
        cout << "Couldn't open " << filename << ". Make this file is in the data directory."<< endl;
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

    ifs.close();
    return data;

}

