#ifndef PROPERTYFIELDLOADINGSTRATEGY_H
#define PROPERTYFIELDLOADINGSTRATEGY_H

#include "Exceptions/unknownextensionexception.h"
#include "Exceptions/modelloadingexception.h"
#include <string>
#include <vector>
#include <memory>
#include <QThread>
#include "UI/modelloadingprogressdialog.h"
#include "PropertyFieldLoading/propertyfielddef.h"
#define N_DEFAULT_THREADS_MODEL_LOADING 8
#define PROPERTY_FIELD_LOAD_FUNC_DEC \
	virtual void load( std::string filename, Model* model, std::vector<std::shared_ptr<PropertyFieldDef>> propertyFieldDef)
#define PROPERTY_FIELD_LOAD_DEFS_FUNC_DEC \
	virtual std::vector<std::shared_ptr<PropertyFieldDef>> loadDefs( std::string filename)
#define PROPERTY_FIELD_VALIDATE_FUNC_DEC \
	virtual bool validate( std::string filename )
#define PROPERTY_FIELD_LOADING_EXTENDING_CLASS_MINIMAL(x)\
	x();\
	virtual ~x();\
	PROPERTY_FIELD_LOAD_FUNC_DEC;\
	PROPERTY_FIELD_LOAD_DEFS_FUNC_DEC;\
	PROPERTY_FIELD_VALIDATE_FUNC_DEC;

class Model;
struct AcceptedFileFormat;

class PropertyFieldLoadingStrategy:public QThread
{
		Q_OBJECT
	public:
		PropertyFieldLoadingStrategy();
		PropertyFieldLoadingStrategy(std::string fileFormatName, std::string fileFormatExt);
		virtual ~PropertyFieldLoadingStrategy();
		PROPERTY_FIELD_LOAD_FUNC_DEC = 0;
		PROPERTY_FIELD_LOAD_DEFS_FUNC_DEC = 0;
		PROPERTY_FIELD_VALIDATE_FUNC_DEC = 0;
		std::vector<AcceptedFileFormat>& getFileFormats();

		//QThread
		void run();
		void loadPropertyFieldQThread(std::string filename, Model* model, std::vector<std::shared_ptr<PropertyFieldDef>> propertyFieldDefs);
	signals:
		void propertyFieldsLoadedSuccesfully();
		void setupDialog(std::string, std::vector<std::shared_ptr<PropertyFieldDef>>);
		void setLoadedProgress(unsigned int);
		void addMessage(QString);
		void errorLoadingPropertyField(QString);
		void warningLoadingPropertyField(QString);
	protected:
		std::vector<AcceptedFileFormat> acceptedFileFormats;
		//tools
	private:
		Model* model;
		std::string propertyFieldPath;
		std::vector<std::shared_ptr<PropertyFieldDef>> selected;

};

#endif // PROPERTYFIELDLOADINGSTRATEGY_H
