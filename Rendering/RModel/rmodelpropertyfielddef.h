#ifndef RMODELPROPERTYFIELDDEF_H
#define RMODELPROPERTYFIELDDEF_H
#include <GL/glew.h>
#include <memory>

class PropertyFieldDef;

//template<typename T, typename std::enable_if<std::is_base_of<PropertyFieldDef, T>::value>::type* = nullptr>
//	typename std::enable_if<std::is_base_of<PropertyFieldDef, T>::value>::type
template<typename T>
class RModelPropertyFieldDef
{
	public:
		RModelPropertyFieldDef(std::shared_ptr<T>, GLuint, GLsizei, GLvoid*);
		virtual ~RModelPropertyFieldDef();
		std::string& getName() const;
		RModelPropertyFieldDef();
		GLuint getBuffer() const;
		GLsizei getStride() const;
		GLvoid* getOffset() const;
		std::shared_ptr<T> getPropertyFieldDef() const;

	protected:
		std::shared_ptr<T> propertyFieldDef;
	private:
		GLuint buffer;
		GLsizei stride;
		GLvoid* offset;
};

template <typename T>
RModelPropertyFieldDef<T>::RModelPropertyFieldDef(std::shared_ptr<T> propertyFieldDef,
											   GLuint buffer, GLsizei stride, GLvoid* offset) :
	propertyFieldDef(propertyFieldDef),
	buffer(buffer),
	stride(stride),
	offset(offset){}

template<typename T>
RModelPropertyFieldDef<T>::~RModelPropertyFieldDef(){}

template<typename T>
std::string& RModelPropertyFieldDef<T>::getName() const
{
	return this->propertyFieldDef->getNName();
}

template<typename T>
GLuint RModelPropertyFieldDef<T>::getBuffer() const
{
	return buffer;
}

template<typename T>
std::shared_ptr<T> RModelPropertyFieldDef<T>::getPropertyFieldDef() const
{
	return this->propertyFieldDef;
}

template<typename T>
GLsizei RModelPropertyFieldDef<T>::getStride() const
{
	return stride;
}

template<typename T>
GLvoid* RModelPropertyFieldDef<T>::getOffset() const
{
	return (GLvoid*)offset;
}

#endif // RMODELPROPERTYFIELDDEF_H
