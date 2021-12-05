#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <random>

using namespace std;

int minimum;
int maximum;
int cardinality;
std::vector<int> numbers;
int decision;
int valueCounter;
string curVal;
int requestCounter = 0;
bool running = true;

std::random_device rd;
std::mt19937 gen(rd());

int main()
{
    ifstream initialInput("input.csv");
    string initialCsvData;
    getline(initialInput, initialCsvData);

    cout << "Welcome to the McCafferty Rudd random number generation microservice!" << endl;

    string currentCsvData;

    ++requestCounter;
    cout << "Request #" << requestCounter << ":" << endl;

    stringstream parameters(initialCsvData);

    valueCounter = 0;

    while(getline(parameters, curVal, ',')){
        if (valueCounter == 0) {
            minimum = stoi(curVal);
        } else if (valueCounter == 1) {
            maximum = stoi(curVal);
            cout << "Range: (" << minimum << ", " << maximum << ")" << endl;
        } else if (valueCounter == 2) {
            cardinality = stoi(curVal);
            cout << "Quantity of numbers to be generated in range: " << cardinality << endl;
        }
        ++valueCounter;
    }

    std::uniform_real_distribution<> dis(minimum, maximum);

    ofstream output("output.csv");

    cout << "Generated numbers: ";

    for (int i = 0; i < cardinality; i++) {
        int num = dis(gen);
        cout << num;
        if ((i+1) != cardinality) {
            cout << ", ";
        }
        numbers.push_back(num);
    }

    cout << endl;

    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        output << (*it);
        if ((it + 1) != numbers.end()) {
            output << ",";
        }
    }

    numbers.clear();

    output.close();

    currentCsvData = initialCsvData;

    while (currentCsvData == initialCsvData) {

        string csvData;

        ifstream input("input.csv");

        getline(input, csvData);

        if (csvData != initialCsvData) {

            ++requestCounter;
            cout << "Request #" << requestCounter << ":" << endl;

            stringstream parameters(csvData);

            valueCounter = 0;

            while(getline(parameters, curVal, ',')){
                if (valueCounter == 0) {
                    minimum = stoi(curVal);
                } else if (valueCounter == 1) {
                    maximum = stoi(curVal);
                    cout << "Range: (" << minimum << ", " << maximum << ")" << endl;
                } else if (valueCounter == 2) {
                    cardinality = stoi(curVal);
                    cout << "Quantity of numbers to be generated in range: " << cardinality << endl;
                }
                ++valueCounter;
            }

            std::uniform_real_distribution<> dis(minimum, maximum);

            ofstream output("output.csv");

            cout << "Generated numbers: ";

            for (int i = 0; i < cardinality; i++) {
                int num = dis(gen);
                cout << num;
                if ((i+1) != cardinality) {
                    cout << ", ";
                }
                numbers.push_back(num);
            }

            cout << endl;

            for (auto it = numbers.begin(); it != numbers.end(); ++it) {
                output << (*it);
                if ((it + 1) != numbers.end()) {
                    output << ",";
                }
            }

            numbers.clear();

            output.close();

            currentCsvData = csvData;
            initialCsvData = csvData;
        }
    }

    return 0;
}