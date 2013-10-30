#ifndef MODELGENERALSTATICSCOLLECTOR_H
#define MODELGENERALSTATICSCOLLECTOR_H
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <sstream>

struct ModelStaticsItem{
	public:
		ModelStaticsItem(std::string s):itemTitle(s){}
		std::string itemTitle;
		std::map<std::string, std::string> stats;
		void addStat(std::string s,std::string f){
			stats.insert( std::make_pair( s, f ) );
		}
		void addStat(std::string s,float f){
			std::stringstream ss;
			ss << f;
			stats.insert( std::make_pair( s, ss.str() ) );
		}
		void addStat(std::string s,int f){
			std::stringstream ss;
			ss << f;
			stats.insert( std::make_pair( s, ss.str() ) );
		}
};
class VertexCloud;
class PolygonMesh;
class PolyhedronMesh;
class ModelGeneralStaticsCollector
{
	public:
		ModelGeneralStaticsCollector();
		void fillModelStatics(VertexCloud*);
		void fillModelStatics(PolygonMesh*);
		void fillModelStatics(PolyhedronMesh*);
		std::vector<ModelStaticsItem>& getModelStatics();
		ModelStaticsItem* findItem(std::string title);
		void clean();
	private:
		std::vector<ModelStaticsItem> modelStatics;
};

#endif // MODELGENERALSTATICSCOLLECTOR_H
