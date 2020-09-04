#include "gtest/gtest.h"
#include "DependencyService.h"

using namespace rls;

TEST(TestItemDB, ItemAddGet) {
	DependencyService depend;
	auto db = depend.getDataBaseItem();
	TimeStamp created,assigned,finished;
	Item it(121,ItemState::created,"test","shortdesc","desc",1,2,3,4,5,created,assigned,finished);
	db->add(it);
	auto it2 = db->get(it.getID());
	EXPECT_EQ(it.getID(), it2.getID());
	EXPECT_EQ(it.getName(), it2.getName());
	EXPECT_EQ(it.getShortDiscription(), it2.getShortDiscription());
	EXPECT_EQ(it.getDescription(), it2.getDescription());
	EXPECT_EQ(it.getAssignedID(), it2.getAssignedID());
	EXPECT_EQ(it.getCreatorID(), it2.getCreatorID());
	EXPECT_EQ(it.getPriority(), it2.getPriority());
	EXPECT_EQ(it.getWorkload(), it2.getWorkload());
	EXPECT_EQ(it.getPlaceID(), it2.getPlaceID());
	// do something
}