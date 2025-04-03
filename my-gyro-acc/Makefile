csources := $(shell find . -name "*.c")
cbinaries := $(csources:.c=)
cppsources := $(shell find . -name "*.cc")
cppbinaries := $(cppsources:.cc=)

# Konto 3
gam : IMU.o gyro_accelerometer.o
	gcc -o gam -Wall IMU.o gyro_accelerometer.o

# Default rules
%: %.c
	gcc -Wall $< -o $@

%: %.cc
	g++ -Wall $< -o $@

clean :
	rm -f $(cbinaries) $(cppbinaries) *.o
