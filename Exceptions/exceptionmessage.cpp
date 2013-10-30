#include "exceptionmessage.h"

ExceptionMessage::ExceptionMessage(std::string m):
	message(m)
{
}
ExceptionMessage::~ExceptionMessage(){

}
std::string ExceptionMessage::getMessage(){
	return message;
}
