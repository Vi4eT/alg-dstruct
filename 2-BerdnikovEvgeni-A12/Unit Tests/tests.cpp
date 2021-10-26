#include "gtest/gtest.h"
#include "../XOR linked list/XOR.h"

TEST(AddTest, SecondElement_ReturnFalse)
{
	char *data = "2";
	list l, *first = &l;
	first->data = "1";
	first->addr = NULL;
	Add(&first, data);
	EXPECT_STREQ(first->data, data);
	EXPECT_EQ(l.addr, first);
	free(first);
}

TEST(AddTest, NElement_ReturnFalse)
{
	char *data = "3";
	list l1, l2, *first = &l1, *second = &l2;
	first->data = "1";
	first->addr = &l2;
	second->data = "2";
	second->addr = &l1;
	Add(&second, data);
	EXPECT_STREQ(second->data, data);
	EXPECT_EQ(l2.addr, (list*)((uintptr_t)first ^ (uintptr_t)second));
	free(second);
}

TEST(IterTest, TwoElementsList)
{
	list l1, l2, *first = &l1, *second = &l2;
	first->addr = &l2;
	second->addr = &l1;
	Iter(&first, &second);
	EXPECT_EQ(first, &l2);
	EXPECT_FALSE(second);
}

TEST(IterTest, NElementsList)
{
	list l1, l2, l3, *first = &l1, *second = &l2;
	first->addr = &l2;
	second->addr = (list*)((uintptr_t)&l1 ^ (uintptr_t)&l3);
	Iter(&first, &second);
	EXPECT_EQ(first, &l2);
	EXPECT_EQ(second, &l3);
}

TEST(FindTest, ExistingElement_ReturnValidVal)
{
	list l1, l2, l3, *first = &l1, *second = &l2, *third = &l3;
	first->addr = &l2;
	second->addr = (list*)((uintptr_t)&l1 ^ (uintptr_t)&l3);
	third->addr = &l2;
	first->data = "1";
	second->data = "2";
	third->data = "3";
	list *r = Find(&second, &third, "1");
	EXPECT_EQ(r, &l1);
}

TEST(FindTest, NotExistingElement_ReturnNull)
{
	list l1, l2, l3, *first = &l1, *second = &l2, *third = &l3;
	first->addr = &l2;
	second->addr = (list*)((uintptr_t)&l1 ^ (uintptr_t)&l3);
	third->addr = &l2;
	first->data = "1";
	second->data = "2";
	third->data = "3";
	list *r = Find(&second, &third, "0");
	EXPECT_FALSE(r);
}

TEST(FindTest, ListWithNull_ReturnValidVal)
{
	list l1, l2, l3, *first = &l1, *second = &l2, *third = &l3;
	first->addr = &l2;
	second->addr = (list *)((uintptr_t)&l1 ^ (uintptr_t)&l3);
	third->addr = &l2;
	first->data = "1";
	second->data = "2";
	third->data = NULL;
	list *r = Find(&second, &third, "2");
	EXPECT_EQ(r, &l2);
}

TEST(FindTest, NotExistingWithNull_ReturnNull)
{
	list l1, l2, l3, *first = &l1, *second = &l2, *third = &l3;
	first->addr = &l2;
	second->addr = (list *)((uintptr_t)&l1 ^ (uintptr_t)&l3);
	third->addr = &l2;
	first->data = NULL;
	second->data = "2";
	third->data = "3";
	list *r = Find(&second, &third, "0");
	EXPECT_FALSE(r);
}

TEST(DelTest, TwoElementsList)
{
	list l1, *first = &l1, *second = (list*)malloc(sizeof(list));
	assert(second);
	first->addr = second;
	second->addr = first;
	Del(first, second);
	EXPECT_FALSE(first->addr);
}

TEST(DelTest, TwoElementsListFirstElement)
{
	list l2, *second = &l2, *first = (list*)malloc(sizeof(list));
	assert(first);
	first->addr = second;
	second->addr = first;
	Del(second, first);
	EXPECT_FALSE(second->addr);
}

TEST(DelTest, LastElement)
{
	list l1, l2, *first = &l1, *second = &l2, *third = (list*)malloc(sizeof(list));
	assert(third);
	first->addr = second;
	second->addr = (list*)((uintptr_t)first ^ (uintptr_t)third);
	third->addr = second;
	Del(second, third);
	EXPECT_EQ(second->addr, first);
}

TEST(DelTest, MiddleElement)
{
	list l1, l2, l4, l5, *first = &l1, *second = &l2, 
			 *third = (list*)malloc(sizeof(list)), *fourth = &l4, *fifth = &l5;
	assert(third);
	first->addr = second;
	second->addr = (list*)((uintptr_t)first ^ (uintptr_t)third);
	third->addr = (list*)((uintptr_t)second ^ (uintptr_t)fourth);
	fourth->addr = (list*)((uintptr_t)fifth ^ (uintptr_t)third);
	fifth->addr = fourth;
	Del(second, third);
	EXPECT_EQ(second->addr, (list*)((uintptr_t)first ^ (uintptr_t)fourth));
	EXPECT_EQ(fourth->addr, (list*)((uintptr_t)second ^ (uintptr_t)fifth));
}

TEST(KeyDelTest, ExistingElement)
{
	char *data = "2";
	list l1, *first = &l1, *second = (list*)malloc(sizeof(list));
	assert(second);
	first->addr = second;
	second->addr = first;
	second->data = data;
	KeyDel(second, data);
	EXPECT_FALSE(first->addr);
}

TEST(KeyDelTest, ExistingFirstElement)
{
	char *data = "1";
	list l2, *second = &l2, *first = (list*)malloc(sizeof(list));
	assert(first);
	first->addr = second;
	second->addr = first;
	first->data = data;
	second->data = "2";
	KeyDel(second, data);
	EXPECT_FALSE(second->addr);
}

TEST(KeyDelTest, NotExistingElement)
{
	list l1, l2, l3, *first = &l1, *second = &l2, *third = &l3;
	first->addr = second;
	second->addr = (list*)((uintptr_t)first ^ (uintptr_t)third);
	third->addr = second;
	first->data = "1";
	second->data = "2";
	third->data = "3";
	KeyDel(third, "0");
	EXPECT_STREQ(l1.data, first->data);
	EXPECT_STREQ(l2.data, second->data);
	EXPECT_STREQ(l3.data, third->data);
}

TEST(AddrDelTest, ExistingElement)
{
	list l1, *first = &l1, *second = (list*)malloc(sizeof(list));
	assert(second);
	first->addr = second;
	second->addr = first;
	AddrDel(second, second);
	EXPECT_FALSE(first->addr);
}

TEST(AddrDelTest, ExistingFirstElement)
{
	list l2, l3, *first = (list*)malloc(sizeof(list)), *second = &l2, *third = &l3;
	assert(first);
	first->addr = second;
	second->addr = (list*)((uintptr_t)first ^ (uintptr_t)third);
	third->addr = second;
	first->data = "1";
	second->data = "2";
	third->data = "3";
	AddrDel(third, first);
	EXPECT_EQ(second->addr, third);
	EXPECT_EQ(third->addr, second);
}

TEST(AddrDelTest, NotExistingElement)
{
	list l1, l2, l3, *first = &l1, *second = &l2, *third = &l3;
	first->addr = second;
	second->addr = (list*)((uintptr_t)first ^ (uintptr_t)third);
	third->addr = second;
	first->data = "1";
	second->data = "2";
	third->data = "3";
	AddrDel(third, second->addr);
	EXPECT_EQ(&l1, first);
	EXPECT_EQ(&l2, second);
	EXPECT_EQ(&l3, third);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}