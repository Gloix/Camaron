#ifndef DYNAMICORDEREDCONTAINERWITHMAXSIZE_H
#define DYNAMICORDEREDCONTAINERWITHMAXSIZE_H
#include <map>
#include <vector>
#include <glm/glm.hpp>
class DynamicOrderedContainerWithMaxSize
{
	public:
		DynamicOrderedContainerWithMaxSize();
		void addElement(glm::vec3, int);
		std::vector<std::pair<glm::vec2,int> >& getOrderedElements();
		unsigned int getSize();
		void setCapacity(unsigned int);
		unsigned int getCapacity();
		void clear();
	private:
		void deleteSmallestElement();
		unsigned int nItems;
		unsigned int maxCapacity;
		float maxValue;
		std::map<float,std::vector<std::pair<glm::vec2,int> > > storage;
		std::vector<std::pair<glm::vec2,int> > orderedElements;
};

#endif // DYNAMICORDEREDCONTAINERWITHMAXSIZE_H
