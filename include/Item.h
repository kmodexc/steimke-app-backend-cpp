#pragma once
#include <string>

namespace rls{

enum class ItemState{
	created,
	assigned,
	inprogress,
	finished,
	paused,
	aborted
};

class Item{
	int id;
	ItemState state;
	std::string name;
	std::string shortdesc;
	std::string desc;
	int assid;
	int creaid;
	int prio;
	int wl;
public:
	Item();
	Item(int id,ItemState state,std::string name,std::string shortdesc,std::string desc,int assid,int creaid,int prio,int wl);
	int getID() const;
	ItemState getState() const;
	std::string getName() const;
	std::string getShortDiscription() const;
	std::string getDescription() const;
	int getAssignedID() const;
	int getCreatorID() const;
	int getPriority() const;
	int getWorkload() const;
};

}