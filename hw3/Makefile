# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
SRIPATH ?= /home/student/06/b06902048/DSP/dsp_hw3/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
LM ?= bigram.lm

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -w --std=c++11
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
.PHONY: all clean map run

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
run:
	mkdir -p result2
	for i in $(shell seq 1 10); do \
		echo "running $$i.txt";	\
		./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result2/$$i.txt; \
	done;
map:
	echo "Mapping!"
	python3 mapping.py $(FROM) $(TO)
clean:
	$(RM) $(OBJ) $(TARGET)

