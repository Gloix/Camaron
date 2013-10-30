#include "qtworkerbase.h"
#include <QThread>
#include <exception>
#include <iostream>
QtWorkerBase::QtWorkerBase()
{
	badAlloc = false;
	finishedRight = false;
}
QtWorkerBase::~QtWorkerBase()
{
}
void QtWorkerBase::process(){
	try{
		doWork();
	}catch(std::bad_alloc& e){
		badAlloc = true;
	}
	while(!finishedRight){
		finishedRight = true;
		try{
			//emit finished();
			QThread::currentThread()->quit();
		}catch(std::bad_alloc& e){
			finishedRight = false;
		}
	}
}

bool QtWorkerBase::hasBadAlloc(){
	return badAlloc;
}

