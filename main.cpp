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
void backwardElimination(const vector<vector<float>>& data );
float leaveOneOutCrossValidation(const vector<vector<float>>& data, const set<int>& currentSet, int featureToAdd);
float getDefaultRate(int classNumber, const vector<vector<float>>& data);


int main(int argc, char** argv) {
    //Ver_2_CS170_Fall_2021_Small_data__86.txt
    //Ver_2_CS170_Fall_2021_LARGE_data__27.txt

    //Ver_2_CS170_Fall_2021_Small_data__61.txt
    //Ver_2_CS170_Fall_2021_LARGE_data__22.txt

    //Ver_2_CS170_Fall_2021_Small_data__18.txt
    //Ver_2_CS170_Fall_2021_LARGE_data__25.txt
    

    // string filename = argv[1];

    // vector<vector<float>> data = parseData(filename);
    //featureSearch(data);
    // backwardSelection(data);

    string filename;
    int algorithm;
    vector<vector<float>> data;

    cout << "=======================================" << endl;
    cout << "========|| Feature Selection ||========" << endl;
    cout << "=======================================" << endl;
    cout << endl;

    cout << "Type in the name of the file to test: ";
    cin >> filename;
    cout << endl;

    data = parseData(filename);

    cout << "Type the number of the algorithm you want to run:" << endl << endl;
    cout << "1) Forward Selection" << endl;
    cout << "2) Backward Elimination" << endl << endl;
    cout << "Your selection: ";
    cin >> algorithm;

    cout << endl;
    if (algorithm == 1) {
        cout << "Beginning Search." << endl << endl;
        featureSearch(data);
    }
    else if (algorithm == 2) {
        cout << "Beginning Search." << endl << endl;
        backwardElimination(data);
    }



}

float getDefaultRate(int classNumber, const vector<vector<float>>& data) {

    int actual = 0;
    int total = data.size();

    for (const vector<float>& row : data) {
        actual = classNumber == static_cast<int>(row.at(0)) ? actual+1 : actual;
    }

    return actual / static_cast<float>(total);
}

void featureSearch(const vector<vector<float>>& data) {

    set<int> currentSetOfFeatures;
    set<int> bestSet;
    float defaultRate = max(getDefaultRate(1, data), getDefaultRate(2, data));
    float bestAccuracy = defaultRate;
    vector< set<int> > curSet; curSet.push_back(currentSetOfFeatures);
    vector<float> acc;   acc.push_back(defaultRate);


    for (int i = 1; i < data.at(0).size(); ++i) {

        cout << "On the " << i << "th level of the search tree" << endl; 

        int featureToAddAtThisLevel = -1;
        float bestAccuracySoFar = 0;
        

        for (int k = 1; k < data.at(0).size(); ++k) {

            if (currentSetOfFeatures.count(k) > 0) continue;
            // cout << "--Considering adding the " << k << " feature" << endl;

            float accuracy = leaveOneOutCrossValidation(data, currentSetOfFeatures, k);

            if (accuracy > bestAccuracySoFar) {
                bestAccuracySoFar = accuracy;
                featureToAddAtThisLevel = k;
    
            }

            set<int> temp = currentSetOfFeatures;
            temp.insert(k);
            cout << "\tUsing feature(s) {";

            for (auto it = temp.begin(); it != temp.end(); it++) {
                auto it_copy = it;
                cout << *it;
                if ( ++it_copy != temp.end()) {
                    cout << ", ";
                }
            }

            cout << "} accuracy is " << accuracy * 100 << "%" << endl;

        }

        cout << endl;

        currentSetOfFeatures.insert(featureToAddAtThisLevel);
        curSet.push_back(currentSetOfFeatures);
        acc.push_back(bestAccuracySoFar);

        if (bestAccuracySoFar > bestAccuracy) {
            bestAccuracy = bestAccuracySoFar;
            bestSet = currentSetOfFeatures;

            cout <<  "Feature set {";
            for (auto it = currentSetOfFeatures.begin(); it != currentSetOfFeatures.end(); it++) {
                auto it_copy = it;
                cout << *it;
                if ( ++it_copy != currentSetOfFeatures.end()) {
                    cout << ", ";
                }
            }
            cout << "} was best, accuracy is " << bestAccuracySoFar * 100 << "%" << endl << endl;
        }
        else {
            cout << "Warning: Accuracy has decreased! Continuing search in case of local maxima." << endl;

            cout <<  "Feature set {";
            for (auto it = currentSetOfFeatures.begin(); it != currentSetOfFeatures.end(); it++) {
                auto it_copy = it;
                cout << *it;
                if ( ++it_copy != currentSetOfFeatures.end()) {
                    cout << ", ";
                }
            }
            cout << "} was best, accuracy is " << bestAccuracySoFar * 100 << "%" << endl << endl;
        }

    }

    cout << "===========================================================================================================" << endl;
    cout << "Finished search!! The best feature subset is {";

    for (auto it = bestSet.begin(); it != bestSet.end(); it++) {
        auto it_copy = it;
        cout << *it;
        if ( ++it_copy != bestSet.end() ) {
            cout << ", ";
        }
    }
    cout << "} with an accuracy of " << bestAccuracy * 100 << "%" << endl;
    cout << "===========================================================================================================" << endl;
    cout << endl;


    // cout << "=====================================" << endl;
    // cout << "BEST SET: ";
    // for (auto it = bestSet.begin(); it != bestSet.end(); it++) {
    //     cout << *it << " ";
    // }
    // cout << endl;
    // cout << "accuracy: " << bestAccuracy << endl;
    // cout << endl;

    // cout << "=====================================" << endl;

    // for (int i = 0; i < curSet.size(); ++i) {
    //     const set<int>& s = curSet.at(i);

    //     for (const auto& item : s) {
    //         cout << item << " ";
    //     }
    //     cout << "=====> " << acc.at(i) << endl;
    // }


}

void backwardElimination(const vector<vector<float>>& data) {
    set<int> currentSetOfFeatures;
    vector<int> answer;
    set<int> bestSet;

    for (int i = 1; i <= data.at(0).size()-1; ++i) {
        currentSetOfFeatures.insert(i);
    }

    float defaultRate = leaveOneOutCrossValidation(data, currentSetOfFeatures, -1);
    float bestAccuracy = defaultRate;

    vector< set<int> > curSet; curSet.push_back(currentSetOfFeatures);
    vector<float> acc;   acc.push_back(defaultRate);
    



    for (int i = 1; i < data.at(0).size(); ++i) {

        cout << "On the " << i << "th level of the search tree" << endl; 

        int featureToRemoveAtThisLevel = -1;
        float bestAccuracySoFar = 0;
        float accuracy;

        for (int k = 1; k < data.at(0).size(); ++k) {

            if (currentSetOfFeatures.count(k) == 0) continue;
            cout << "--Considering removing the " << k << " feature" << endl;

            set<int> temp = currentSetOfFeatures;
            temp.erase(k);
            

            accuracy = leaveOneOutCrossValidation(data, temp, -1);  //featureToAdd=-1 since no features to add
            if (accuracy > bestAccuracySoFar) {
                bestAccuracySoFar = accuracy;
                featureToRemoveAtThisLevel = k;
    
            }

        }

        // If condition terminates feature search when best accuracy is found for subset of size 1
        if (currentSetOfFeatures.size() > 1) {
            currentSetOfFeatures.erase(featureToRemoveAtThisLevel);
            curSet.push_back(currentSetOfFeatures);
            acc.push_back(bestAccuracySoFar);


            if (bestAccuracySoFar > bestAccuracy) {
                bestAccuracy = bestAccuracySoFar;
                bestSet = currentSetOfFeatures;
                cout << "BEST SET SO FAR: ";
                for (auto it = bestSet.begin(); it != bestSet.end(); it++) {
                    cout << *it << " ";
                }
                cout << " (with accuracy " << bestAccuracySoFar << ")";
                cout << endl;
            }
        }


    
        // cout << "On level " << i << " added feature " << featureToAddAtThisLevel << " to current set" << endl;

    }

    cout << "=====================================" << endl;
    cout << "BEST SET: ";
    for (auto it = bestSet.begin(); it != bestSet.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
    cout << "accuracy: " << bestAccuracy << endl;
    cout << endl;

    cout << "=====================================" << endl;

    for (int i = 0; i < curSet.size(); ++i) {
        const set<int>& s = curSet.at(i);

        for (const auto& item : s) {
            cout << item << " ";
        }
        cout << "=====> " << acc.at(i) << endl;
    }
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

                //Calculate the distance between features in data[i][j] and data[k][j]
                for (int j = 1; j < data.at(0).size(); ++j) {  

                    //only consdering features in current set + featuretoadd
                    if (currentSet.count(j) > 0 || j == featureToAdd) {
                        const vector<float>& row_i = data.at(i); 
                        const vector<float>& row_k = data.at(k);
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
    
    
    return numberCorrectlyClassified / static_cast<float>(data.size());
}

vector<vector<float>> parseData(const string& filename) {

    ifstream ifs("./data/" + filename);
    vector<vector<float>> data;
    string line;

    if (!ifs.is_open()) {
        cout << "Couldn't open " << filename << endl;
        exit(0);
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

