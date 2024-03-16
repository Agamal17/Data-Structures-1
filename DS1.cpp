#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

class SortedUniL{
    public:
        class CarNode {
        public:
            string make;
            string model;
            int carsNum;
            CarNode *next;

            CarNode(string &mk, string &mod, int num = 1,CarNode *in = 0)   // Create a CarNode with argument "num" representing quantity to be stored in the inventory
            : make(mk), model(mod), next(in), carsNum(num) {}
        };

        CarNode *head, *tail;

        SortedUniL():head(0),tail(0){}

        void insert(string& mk, string& mod, int num = 1);                  // insert Car's data with argument "num" representing quantity to be stored in the inventory
        int search(string& mk, string& mod) const;
        void remove(string& mk, string& mod);
        void display() const;
};

string upScale(string& x){                                                  // Function for making all inputs Upper-case to avoid Case-sensitive errors
    int xl = x.length();
    for (int i=0;i<xl;i++) x[i]= toupper(x[i]);
    return x;
}
void SortedUniL::insert(string &mk, string &mod, int num) {
    upScale(mk);    // up scaling mk and mod to be uppercase
    upScale(mod);
    if (num<1) cout<<"Enter a valid quantity.";
    else if (head == 0) head = tail = new CarNode(mk, mod, num);    // to handle if list was empty
    else {
        CarNode* prev = 0;
        CarNode *p = head;
        for (; p != 0 && mk >= p->make; p = p->next) {          // iterates to find the place to put the car's data
            prev = p;
            if (mk == p->make) {                                // if car's make was found:
                if (mod == p->model) {                          // if car's make and model is found, increase its number by num
                    (p->carsNum)+=num;
                    return;
                }
                else if (mod > p->model)                        // if make was equal but model was greater, we iterate to find the place to put the model's node in.
                    while (p!=0 && mk == p->make && mod > p->model) {       // loop is broken if make was not found again, so it puts the node after the last node of the car with the make,
                        prev = p;                                           // or if model was found to be less than a model with the same make of the car, or if we iterated to the end of list and found nothing.
                        p = p->next;
                    }
                    break;
            }
        }
        if (prev == 0){                                         // to handle if the desired car's data is to be added before head of list
            CarNode* tmp = head;
            head = new CarNode(mk, mod,num);
            head ->next = tmp;
        }
        else if (prev == tail){                                 // to handle if the desired car's data is to be inserted after the tail of list
            prev -> next = new CarNode(mk, mod,num);
            tail = prev->next;
        }
        else {
            prev->next = new CarNode(mk, mod,num);              // to handle normally inserting a car node between two nodes
            prev->next->next = p;
        }
    }
}

int SortedUniL::search(string &mk, string &mod) const {
    upScale(mk);
    upScale(mod);
    for (CarNode* p = head;p!=0;p=p->next)
        if (p->make == mk && p->model == mod) return p->carsNum;            // if found, return car's quantity
    return 0;           // if not found return 0, which means no cars of this model is found.
}

void SortedUniL::remove(string &mk, string &mod) {
    upScale(mk);
    upScale(mod);
    if (head == 0){                     // if list is already Empty
        cout<<"No Cars to remove.\n\n";
        return;
    }
    for (CarNode* p = head, *prev = 0 ;p!=0; prev = p, p = p=p->next)
        if (p->make == mk && p->model == mod) {                             // if car is found
            if (p->carsNum > 1){                                            // if carsNum is more than one, decrement it.
                (p->carsNum)--;
                return;
            }
            else{                                                           // if carsNum was equal to one
                if (head == tail && p == head) {                            // if data was found at the only node in the list
                    head = tail = 0;
                }
                else if (p == tail) {                                       // if data was found at the end of list
                    prev -> next = 0;
                    tail = prev;
                }
                else if (p==head) head = p->next;                           // if data was found at head of list
                else prev -> next = p->next;                                // if none of these special cases occured, this is the normal procedure
                delete p;
                return;
            }
        }
    cout<<"Not Found.\n\n";             // if loop finished without interference from inside, print that the car is not found
}

void SortedUniL::display() const {
    if (head == 0) cout<<"No Cars Available.\n\n";
    for (CarNode* p = head;p!=0;p=p->next)
        cout<<"Car's Make: "<<p->make<<endl<<"Car's Model: "<<p->model<<endl<<"Available: "<<p->carsNum<<endl<<endl;
}


int main() {
    int x;
    string input;
    convert:
    cout<<"Do you want to read cars' datum from a file? Enter 0 for no, 1 for yes."<<endl;
    cin>>input;
    try {
        x = stoi(input);
    }
    catch (exception &err) {
        cout<<"Enter a Valid Number\n";
        goto convert;
    }

    SortedUniL Cars;
    string mk, mod;

    switch (x) {
        case 0:
            break;
        case 1: {
            cout<<"Enter Name of file: "<<endl;
            cin>>input;
            cin.ignore();
            ifstream file(input);

            if (!file.is_open()) {
                cout << "file couldn't open" << endl;
                return 1;
            }

            getline(file, input);
            x = stoi(input);

            int length;              // declaring variable outside loops for calculating length of the string from getline(), instead of declaring it multiple times inside a loop.
            int num = 0;            //variable for storing number of cars in the file
            while (x--) {
                getline(file, input);
                length = input.length();
                for (int i=0;i<length;i++){
                    if (input[i] == '|') {
                        for (int j = 0; j < i; j++) mk += input[j];
                        for (int j = i+1;input[j]!=':';j++) mod += input[j];
                    }
                    else if (input[i] == ':') {
                        for (int j = length-1, c = 0;j>i;j--,c++)
                            if (input[j]>47 && input[j]<58)                 //for ensuring that the characters after ":" in the file are numbers.
                                num = num + ((input[j] - 48) * pow(10,c));  //reading numbers from right to left with the "c" variable acting as power for the radix 10
                        break;
                    }
                }
                Cars.insert(mk, mod, num);
                mk = mod = "";
                num = 0;
            }
        }
            break;
        default:
            cout<<"Enter a valid option."<<endl;
            goto convert;
    }

    while(true) {
        cout << "Welcome to the Cars' Management System!" << endl;
        cout << "Please choose an option by number:" << endl;
        cout << "1- Add a car." << endl;
        cout << "2- Remove a car." << endl;
        cout << "3- Search for a car by make and model." << endl;
        cout << "4- Display all cars." << endl;
        cout << "5- Exit" << endl;

        cin>>input;
        try {
            x = stoi(input);
        }
        catch (exception &err) {
            cout<<"Enter a Valid Number\n";
            continue;
        }

        switch (x) {
            case 1:
            case 2:
            case 3:
                cout<<"Enter Car's Make: "<<endl;
                cin.ignore();
                getline(cin,mk);
                cout<<"Enter Car's Model: "<<endl;
                getline(cin,mod);
                switch (x) {
                    case 1:
                        Cars.insert(mk, mod);
                        break;
                    case 2:
                        Cars.remove(mk, mod);
                        break;
                    case 3:
                        cout<<"Available: "<<Cars.search(mk, mod)<<endl;
                        break;
                    default:
                        ;
                }
                cout<<endl;
                break;

            case 4:
                Cars.display();
                break;
            case 5:
                exit(0);
            default:
                cout<<"Enter a valid option. "<<endl;
        }
    }
}
