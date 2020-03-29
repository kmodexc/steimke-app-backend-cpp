#pragma once
#include <string>

namespace rls{

struct TimeStamp{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	TimeStamp(){year=0;month=0;day=0;hour=0;minute=0;}
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
	int placeid;

	TimeStamp createTime;
	TimeStamp assignedTime;
	TimeStamp finishedTime;
public:
	Item();
	Item(int id,ItemState state,std::string name,
		std::string shortdesc,std::string desc,
		int assid,int creaid,int prio,int wl,int placeid,
		TimeStamp created,TimeStamp assigned,TimeStamp finished);
	int getID() const;
	ItemState getState() const;
	std::string getName() const;
	std::string getShortDiscription() const;
	std::string getDescription() const;
	int getAssignedID() const;
	int getCreatorID() const;
	int getPriority() const;
	int getWorkload() const;
	int getPlaceID() const;

	TimeStamp getCreateTime() const;
	TimeStamp getAssignedTime() const;
	TimeStamp getFinishedTime() const;
};

}