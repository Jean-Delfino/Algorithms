#include "NFibonacci.h"

void NFibonacci::InsertOneInitialValue(float value){
	initialValues.push(value);
	maxInitialValues += value;
}

float ReturnOneOfTheInitialNumbers(queue<float> values, int m){
	int i;
	
	for(i = 0; i < m ; i++){
		values.pop();
	}
	
	return values.front();	
}

//M will never be less than size
float NFibonacci::FindNFibonacciMPosition(int m){
	queue<float> values(initialValues);
	
	if(m < 0)return -1;
	if(m < maxSizeQueue)return ReturnOneOfTheInitialNumbers(values, m);

	float res = maxInitialValues;
	float auxMinValue;
	int i;
	
	for(i = maxSizeQueue; i < m; i++){
		auxMinValue = values.front();
		
		values.pop();
		values.push(res);
		res = res + res - auxMinValue;
	}
	
	return res;
}


