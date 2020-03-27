#pragma once
#include <string>

namespace rls{

struct TimeStamp{
	int year;
	int month;
	int day;
	int hour;
	int minute;
};

enum class ItemState{
	created,
	assigned,
	inprogress,
	finished,
	paused,
	aborted
};

std::string toString(const ItemState &state);
ItemState parseItemState(const std::string &str);

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
	TimeStamp createTime;
	TimeStamp assignedTime;
	TimeStamp finishedTime;
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