.PHONY: all clean distclean build run
.DEFAULT_GOAL:= all

GTEST_PATH=thirdparty/googletest/
GTEST_BUILD_PATH=$(GTEST_PATH)/build

all: run

build: testexe

run : build
	#launch test
	./testexe
	
testexe: $(GTEST_BUILD_PATH)
	#build test
	g++ -o testexe ./test/test.cpp -Isrc -I$(GTEST_PATH)/googletest/include -L$(GTEST_PATH)/build/googlemock/gtest -lgtest -pthread -std=c++11
	
$(GTEST_BUILD_PATH):
	mkdir -p $(GTEST_BUILD_PATH)
	cd $(GTEST_BUILD_PATH) && cmake ../ && make

clean:
	rm testexe

distclean: clean
	rm -rf $(GTEST_BUILD_PATH)
