#include "sortsearchutils.h"

SortSearchUtils::SortSearchUtils(){}


int SortSearchUtils::binarySearchPosition(std::vector<float>& values,float val){
	int a = 0;
	int b = values.size()-1;
	if(b<0)
		return 0;
	while(b!=a){
		int mid = (b-a)/2+a;
		if(values[mid] > val){
			b = mid;
		}
		else{
			a = mid+1;
		}
	}
	return a;
}
