#include <gtest/gtest.h>
#include <log_helper.hpp>

#include <string>
#include <vector>


log_helper::Stringnizer s;

class TestObject{
	public:
	TestObject(int _i):i(_i){}
	std::string ToString() const { return std::string("test") + std::to_string(i);}
	private:
		int i;
};

class TestObject2{
	public:
	//no const
	std::string ToString(){ return "test";}
};

TEST(log_helper_test, single_int){
	EXPECT_STREQ(s.ToString(1).c_str(), "1");
	EXPECT_STREQ(s.ToString(2).c_str(), "2");
	EXPECT_STREQ(s.ToString(-1).c_str(), "-1");
}

TEST(log_helper_test, single_float){
	EXPECT_STREQ(s.ToString(1.0f).c_str(), "1.000000");
	EXPECT_STREQ(s.ToString(-10.f).c_str(), "-10.000000");
}

TEST(log_helper_test, single_object){
	TestObject o1(1);
	TestObject o2(2);
	TestObject2 o3;
	EXPECT_STREQ(s.ToString(o1).c_str(), "test1");
	EXPECT_STREQ(s.ToString(o2).c_str(), "test2");
	EXPECT_STREQ(s.ToString(o3).c_str(), "test");
}

TEST(log_helper_test, vector_float){
	std::vector<float> v{-1,0, 1};
	EXPECT_STREQ(s.ToString(v).c_str(), "[-1.000000,0.000000,1.000000]");
}

TEST(log_helper_test, pair){
	std::pair<int, float> p{1, -1.f};
	EXPECT_STREQ(s.ToString(p).c_str(), "{1,-1.000000}");
}

TEST(log_helper_test, tuple){
	std::tuple<int, float, float> p{1, -1.f, -10.f};
	EXPECT_STREQ(s.ToString(p).c_str(), "{1,-1.000000,-10.000000}");
}


TEST(log_helper_test, complext){
	std::map<int, std::pair<int, int>> m{{1,{1,1}},{2,{2,2}}};
	EXPECT_STREQ(s.ToString(m).c_str(), "[{1,{1,1}},{2,{2,2}}]");
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}




