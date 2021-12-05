#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <windows.h>
#include <iomanip>
using namespace std;
using std::this_thread::sleep_for;

void loadFromFile(const char* fileName, double& a, double& b, int& swap);
void saveToFile(const char* name, int low, int high, int quantity);
double determinePrice(double sell, double buy, double price);
void winOrLoss(double asset);
char getChoice();
void intro(double asset, double usd, double stock, double volArr[3]);
void tutorial();
void buyInfo(double price, double usd);
void sellInfo(double price, double stock);
void buyStock(double volArr[3], double& usd, double& stock);
void sellStock(double volArr[3], double& usd, double& stock);
void newPrice(double& buyVolume, double& sellVolume, int& swap, double& price);
void exitPage();
void noUsd();
void noStock();
void failReaction();
int swapResult(double& a, double& b, int& swap);
void printChoice();
void intro2(double volArr[3]);
void printTutor();
void updateValue(char choice, double& usd, double& stock, double volArr[3]);
void updateValue2(char choice, double& usd, double& stock, double volArr[3]);
void printCancel();


int main() {
	char choice = 'a';
	double price = 50;
	double asset = 0;
	double usd = 1000;
	double stock = 0;
	double buyVolume = 0;
	double sellVolume = 0;
	double buy = 0;
	double sell = 0;
	double money = 0;
	int swap = 0;
	double volArr[3] = {};
	//Initialize price and asset
	price = price + 0.01 * (buyVolume - sellVolume) /price;
	asset = usd + stock * price;
	//let user choose whether they wnat a tutorial
	tutorial();

	//update the trading volume
	saveToFile("input.csv", 4000, 8000, 2);
	system("start random.exe");  //call microservice
	loadFromFile("output.csv", buyVolume, sellVolume, swap);

	//now we have a random buying volume and a random selling volume stored in variables
	price = determinePrice(sellVolume, buyVolume, price); //calculate the market price
	asset = usd + stock * price;

	//generate the UI
	cout << "Welcome to the stock simulator!!!" << endl;
	volArr[0] = buyVolume;
	volArr[1] = sellVolume;
	volArr[2] = price;
	intro(asset, usd, stock, volArr);
	winOrLoss(asset);
	choice = getChoice();
	
	while (choice != 'q') {
		if (choice == 'b') {
			buyInfo(price, usd);
			cin >> buy;

			//if user enter an invalid input, bring them to the home page.
			if (cin.fail()) {
				failReaction();
			}
			else if (buy*price <= usd) {
				volArr[0] = money;
				volArr[1] = buy;
				volArr[2] = price;
				buyStock(volArr, usd, stock);
				newPrice(buyVolume, sellVolume, swap, price);
				asset = usd + stock * price;
				if(price == 0){
					break;
				}
			}
			else {
				noUsd();
			}
		}
		else if (choice == 's') {
			sellInfo(price, stock);
			cin >> sell;

			//if user enter an invalid input, bring them to the home page.
			if (cin.fail()) {
				failReaction();
			}
			else if (sell <= stock) {
				volArr[0] = money;
				volArr[1] = sell;
				volArr[2] = price;
				sellStock(volArr, usd, stock);
				newPrice(buyVolume, sellVolume, swap, price);
				asset = usd + stock * price;
				if(price == 0){
					break;
				}
			}
			else {
				noStock();
			}
		}
		//manually connect to microservice to refreash data
		else if (choice == 'r') {
			newPrice(buyVolume, sellVolume, swap, price);
			asset = usd + stock * price;
			if(price == 0){
					break;
				}
			cout << "Market information has been refreshed!!!" << endl;
		}
		else {
			cout << "Invalid input. Please make your choice again. " << endl;
		}
		cout << endl << endl;
		volArr[0] = buyVolume;
		volArr[1] = sellVolume;
		volArr[2] = price;
		intro(asset, usd, stock, volArr);
		winOrLoss(asset);
		choice = getChoice();
	}
	exitPage();
	return 0;
}

//This function read results from my classmate's service
void loadFromFile(const char* fileName, double& a, double& b, int& swap)
{
	ifstream	in;
	in.open(fileName);
	if (!in)
	{
		cerr << "Failed to open " << fileName << " for reading!" << endl;
		exit(1);
	}
	in >> a;
	in.get();
	in >> b;
	in.get();
	//swap a and b to increase randombility.
	swap = swapResult(a, b, swap) + 1;
	
	in.close();
}

int swapResult(double& a, double& b, int& swap){
	double temp = 0;
	if(swap % 2 != 1){
		temp = a;
		a = b;
		b = temp;
	}
	return swap;
}

//This function input value to my classmate's service
void saveToFile(const char* name, int low, int high, int quantity)
{
	ofstream 	out;
	out.open(name);
	if (!out)
	{
		cerr << "Failed to open " << name << " for writing!" << endl;
		exit(1);
	}
	out << low << "," << high << ","<<quantity << endl;

	out.close();
}

double determinePrice(double sell, double buy, double price){
	double diff = 0;
	diff = buy - sell;
	price = price + 0.1 * diff /price; //This function calculate the market price based on volume
	if(price <= 0){
		cout<<"The company went backrupt, game over :("<<endl;
		return 0;
	}
	return price;
}

//This function prints out different result based on value of asset
void winOrLoss(double asset){
	if(asset < 1000){
		cout<<"You've lost $"<<1000-asset<<endl;
	}
	else{
		cout<<"You've earned $"<<asset-1000<<endl;
	}
}

char getChoice(){
	printChoice();
	char choice = 'x';
	cin >> choice;
	choice = tolower(choice);
	return choice;
}

void printChoice(){
	cout << endl << endl;
	cout << "Operations: " << endl;
	cout << "Press <B> on your keybord and press <enter> to buy stocks" << endl;
	cout << "Press <S> on your keybord and press <enter> to sell stocks" << endl;
	cout << "Press <R> on your keybord and press <enter> to refreash market information ";
	cout << "(That you can get a new market price without trading.)" << endl;
	cout << "Press <Q> on your keybord and press <enter> to exit the program" << endl;
	cout << endl;
	cout << "Your choice: " << endl;
}

void intro(double asset, double usd, double stock, double volArr[3]){
	cout << setw(30) << setfill('*') << "" << endl;  //Set a barrier to make UI looks nice
	cout << "Here's a summary of your account:" << endl;
	cout << "Your total assets: $" << asset << endl;
	cout << "USD avaliable: $" << usd << endl;
	cout << "You own " << stock << " stock" << endl;
	cout << setw(30) << setfill('*') << "" << endl;
	intro2(volArr);
}

void intro2(double volArr[3]){
	cout << endl << endl;
	cout << setw(30) << setfill('*') << "" << endl;
	cout << "Today's market: " << endl;
	cout << "Market price: $" << volArr[2] << endl;
	cout << "Buying volume: " << volArr[0] << " thousand" << endl;
	cout << "Selling volume: " << volArr[1] << " thousand" << endl;
	cout << setw(30) << setfill('*') << "" << endl;
}

void tutorial(){
	char choice = 'x';
	cout << setw(30) << setfill('*') << "" << endl;
	cout << "We have a tutorial avaliable!!!" <<endl;
	cout << "Type <Y> on your keyboard and press <enter> to see the tutorial" <<endl;
	cout << "Type <N> on your keyboard and press <enter> to skip the tutorial" <<endl;
	cin >> choice;
	choice = tolower(choice);

	if(choice == 'y'){
		printTutor();
	}

}

//This is the content of the tutorial
void printTutor(){
	cout << setw(30) << setfill('*') << "" << endl;
	cout << "Welcome to the tutorial. " <<endl;
	cout << "You can make commands to buy or sell stocks by typing <B> or <S> on your keyborad." << endl;
	cout << "You will have $1000 initial dollars on your account, ";
	cout << "and your assets are the sum of your avaliable dollars and value of your stocks." << endl << endl;
	cout << "Have fun!!!" << endl;
	cout << setw(30) << setfill('*') << "" << endl;
	cout << endl << endl << endl << endl << endl;
}

void buyInfo(double price, double usd){
	cout << endl << endl << endl;
	cout << setw(90) << setfill('*') << "" << endl;
	cout << "Stock price: $" << price <<endl;
	cout << "USD avaliable: $" << usd <<endl;
	cout << "How many stocks would you want to buy?" << endl;
	cout << "(You can enter the desired quantity and press <enter> key on your keybord)" << endl;
	cout << "You can also press any keys on your keybord and press ";
	cout << "<enter> to return to the operation selection page" << endl;
}

void sellInfo(double price, double stock){
	cout << endl << endl << endl;
	cout << setw(90) << setfill('*') << "" << endl;
	cout << "Stock price: $" << price << endl;
	cout << "Quantity of stocks you own: " << stock << endl;
	cout << "How many stocks would you want to sell?" << endl;
	cout << "(You can enter the desired quantity and press <enter> key on your keybord)" << endl;
	cout << "You can also press any keys on your keybord and press ";
	cout << "<enter> to return to the operation selection page" << endl;
}

void buyStock(double volArr[3], double& usd, double& stock){
	char choice = 'x';
	volArr[0] = volArr[1] * volArr[2];
	cout << endl << endl;
	cout << "Are you sure you want to buy " << volArr[1] << " stocks? ";
	cout << "They cost you $"<<volArr[0]<<" dollars" << endl;
	cout << "Enter <Y> to confirm and <N> to cancel purchase: ";
	cin >> choice; //Give user choice to go back
	choice = tolower(choice);
	updateValue(choice, usd, stock, volArr);
}

//Those two functions update avaliable usd and stock
void updateValue(char choice, double& usd, double& stock, double volArr[3]){
	if (choice == 'y') {
		usd = usd - volArr[0];
		stock = volArr[1] + stock;
		cout << "Your order has been placed!!!" << endl;
		sleep_for(2000ms);
	}
	else {
		printCancel();
	}
}

void updateValue2(char choice, double& usd, double& stock, double volArr[3]){
	if (choice == 'y') {
		usd = usd + volArr[0];
		stock = stock - volArr[1];
		cout << "Your order has been placed!!!" << endl;
		sleep_for(2000ms);
	}
	else {
		cout << "Your purchase has been cancelled" << endl;
		sleep_for(2000ms);
	}
}

void printCancel(){
	cout << "Your purchase has been cancelled" << endl;
	sleep_for(2000ms);
}

void sellStock(double volArr[3], double& usd, double& stock){
	char choice = 'x';
	volArr[0] = volArr[1] * volArr[2];
	cout << endl << endl;
	cout << "Are you sure you want to sell " << volArr[1] << " stocks? ";
	cout << "They are worth $" << volArr[0] << " dollars" << endl;

	//Give user option to go back
	cout << "Enter <Y> to confirm and <N> to cancel purchase: ";
	cin >> choice;
	choice = tolower(choice);
	updateValue2(choice, usd, stock, volArr);
}


void newPrice(double& buyVolume, double& sellVolume, int& swap, double& price){
	saveToFile("input.csv", 4000, 8000, 2);
	loadFromFile("output.csv", buyVolume, sellVolume, swap);
	price = determinePrice(sellVolume, buyVolume, price);
}

void exitPage(){
	cout << endl << endl <<endl <<endl<<endl<<endl<< endl << endl << endl;
	cout << endl << endl << endl << endl << endl << endl << "Thank you for using the program!!!";
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
}

void noUsd(){
	cout << endl << endl;
	cout << "Oops!!! You don't have that amount of USD avaliable." << endl;
	cout << "\nI'm taking you back to the opeion selection page now!!!" << endl;
	sleep_for(2000ms);
}

void noStock(){
	cout << endl << endl;
	cout << "Oops!!! You don't have that amount of stocks avaliable." << endl;
	cout << "\nI'm taking you back to the opeion selection page now!!!" << endl;
	sleep_for(2000ms);
}

void failReaction(){
	cin.clear();
	cin.ignore(100, '\n');
	cout << endl << endl;
	cout << "\nI'm taking you back to the option selection page now!!!" << endl << endl << endl;
	sleep_for(2000ms);
}