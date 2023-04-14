#include <iostream>

#include "NFibonacci.h"

/*
The idea is a fibonacci or tribonacci or whatever N might be, then a give you the N number, cool, right

4
A B C D 
(4) = A + B + C + D
(5) = 4 + B + C + D
(6) = 5 + 4 + C + D 
*/

using namespace std;

int main() {
    int fibonacciSize;
    int m;
    float value;
    cout << "Fibonacci size: ";
    cin >> fibonacciSize;
    
    NFibonacci fibonacci(fibonacciSize);
	
	for (int i = 0; i < fibonacciSize; i++) {
        cin >> value;
        fibonacci.InsertOneInitialValue(value);
    }
    
    cout << "Insert M position to search: ";
    cin >> m;
    cout << fibonacci.FindNFibonacciMPosition(m);
    return 0;
}
