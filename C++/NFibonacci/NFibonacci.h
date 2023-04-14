#ifndef NFIBONACCI_H
#define NFIBONACCI_H

#include <iostream>
#include <queue>

using namespace std;

class NFibonacci
{
	public:
		NFibonacci(int size){
			maxSizeQueue = size;
		}
		
		void InsertOneInitialValue(float value);
		float FindNFibonacciMPosition(int m);
		
	private:
		int maxSizeQueue;
		float maxInitialValues = 0;
		queue<float> initialValues;		
};

#endif
