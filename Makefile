.PHONY: all test clean distclean
.DEFAULT_GOAL:= all

GTEST_PATH=thirdparty/googletest/
GTEST_BUILD_PATH=$(GTEST_PATH)/build

all: test

test: $(GTEST_BUILD_PATH)
	#build test
	g++ -o test ./sample/test.cpp -Isrc -I$(GTEST_PATH)/googletest/include -L$(GTEST_PATH)/build/googlemock/gtest -lgtest -pthread -std=c++11
	#launch test
	./test

$(GTEST_BUILD_PATH):
	mkdir -p $(GTEST_BUILD_PATH)
	cd $(GTEST_BUILD_PATH) && cmake ../ && make

clean:
	rm test

distclean: clean
	rm -rf $(GTEST_BUILD_PATH)