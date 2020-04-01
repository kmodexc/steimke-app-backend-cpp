#include "Place.h"

namespace rls{


std::string toString(const PlaceType& p){
	switch(p){
		case PlaceType::_public:
			return "public";
		case PlaceType::_private:
			return "private";
	}
	return "public";
}
PlaceType parsePlaceType(const std::string& str){
	if(str == "public")
		return PlaceType::_public;
	else if(str == "private")
		return PlaceType::_private;
	
	return PlaceType::_public;
}



}