#ifndef EXCEPTIONMESSAGE_H
#define EXCEPTIONMESSAGE_H

#include <string>
class ExceptionMessage
{
	public:
		ExceptionMessage(std::string m);
		~ExceptionMessage();
		std::string getMessage();
	protected:
		std::string message;
};

#endif // EXCEPTIONMESSAGE_H
