#ifndef QTWORKERBASE_H
#define QTWORKERBASE_H
#include <QObject>
#include <QString>
class QtWorkerBase: public QObject {
		Q_OBJECT
	public:
		QtWorkerBase();
		~QtWorkerBase();
		virtual void doWork() = 0;
		bool hasBadAlloc();
	public slots:
		void process();

	signals:
		void finished();
	private:
		bool badAlloc, finishedRight;
};

#endif // QTWORKERBASE_H
