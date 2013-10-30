#ifndef QTUTILS_H
#define QTUTILS_H

#include <QString>
#include <QThread>
#include <vector>
#define QtUtilsReadQtLineEditsToGLMVEC3(glmvec3,lineedit)\
	glmvec3.x = QtUtils::readFloatFromQText(lineedit##_r->text(),1.0f);\
	glmvec3.y = QtUtils::readFloatFromQText(lineedit##_g->text(),1.0f);\
	glmvec3.z = QtUtils::readFloatFromQText(lineedit##_b->text(),1.0f)
#define QtUtilsReadQtLineEditsToGLMVEC4(glmvec4,lineedit)\
	glmvec4.x = QtUtils::readFloatFromQText(lineedit##_r->text(),1.0f);\
	glmvec4.y = QtUtils::readFloatFromQText(lineedit##_g->text(),1.0f);\
	glmvec4.z = QtUtils::readFloatFromQText(lineedit##_b->text(),1.0f);\
	glmvec4.w = QtUtils::readFloatFromQText(lineedit##_a->text(),1.0f)
class QDockWidget;
class QWidget;
class QtWorkerBase;
class QtUtils
{
	public:
		static float readFloatFromQText(QString text, float defaultValue);
		static int readIntFromQText(QString text, int defaultValue);
		static int readUIntFromQText(QString text, uint defaultValue);
		static void fixDockingWidgetSize(QDockWidget* toFix, QWidget*);
		static void fixDockingWidgetArea();
		static QThread *runWorkerInThread(QtWorkerBase* worker,
										  bool autodelete = false);
		static void runWorkersInThread(std::vector<QtWorkerBase *> &workers,
									   bool wait = true);
	private:
		QtUtils();
};

#endif // QTUTILS_H
