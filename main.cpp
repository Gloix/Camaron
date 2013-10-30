#include <QApplication>
#include "visualizador.h"
#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include "Utils/openglutils.h"
#include "Factories/EvaluationStrategyRegistry.h"
#include "Factories/modelexportstrategyregistry.h"
#include "Factories/ModelLoadingFactory.h"
#include "Factories/selectionstrategyregistry.h"
#include "Factories/RendererRegistry.h"
void test(int nCells){
	//double cellWidth = 1.0/nCells;
	float cellWidth = 2.0f/nCells;
	int *values = new int[nCells];
	std::cout<<"N CELL: " << nCells << std::endl;
	std::cout<<"CELL WIDTH: " << cellWidth << std::endl;

	for(int i = 0;i<nCells;i++){
		//values[i] = ((float)(i*cellWidth))*nCells;
		values[i] = (i*cellWidth)*nCells;
	}
	for(int i = 0;i<nCells;i++)
		if(values[i]!=i)
			std::cout << "Pifia!: Values["<<i<<"] = "<<values[i]<<std::endl;
	delete values;
}

void printVersions();
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Visualizador w;
	w.show();
	if(!w.initGlewAndRendererFactory()){
		w.hide();
		RendererRegistry::deleteSingletonInstance();
		EvaluationStrategyRegistry::deleteSingletonInstance();
		ModelExportStrategyRegistry::deleteSingletonInstance();
		ModelLoadingFactory::deleteSingletonInstances();
		SelectionStrategyRegistry::deleteSingletonInstance();
		return -1;//fail
	}
	int end = a.exec();
	RendererRegistry::deleteSingletonInstance();
	EvaluationStrategyRegistry::deleteSingletonInstance();
	ModelExportStrategyRegistry::deleteSingletonInstance();
	ModelLoadingFactory::deleteSingletonInstances();
	SelectionStrategyRegistry::deleteSingletonInstance();
	OpenGLUtils::deleteSingletonInstance();
	return end;
}

