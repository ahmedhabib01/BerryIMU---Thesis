/*
    This program  reads the angles from the accelerometer and gyroscope
    on a BerryIMU connected to a Raspberry Pi.


    The BerryIMUv1, BerryIMUv2 and BerryIMUv3 are supported.

    Feel free to do whatever you like with this code
    Distributed as-is; no warranty is given.

    https://ozzmaker.com/berryimu/
*/

#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include "IMU.h"

#define SAMPLES 100
#define DT 0.02         // [s/loop] loop period. 20ms
#define AA 0.97         // complementary filter constant

#define A_GAIN 0.0573    // [deg/LSB]
#define G_GAIN 0.070     // [deg/s/LSB]
#define RAD_TO_DEG 57.29578
#define M_PI 3.14159265358979323846
 




void  INThandler(int sig)// Used to do a nice clean exit when Ctrl-C is pressed
{
	signal(sig, SIG_IGN);
	exit(0);
}

int mymillis()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec) * 1000 + (tv.tv_usec)/1000;
}

int timeval_subtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    result->tv_sec = diff / 1000000;
    result->tv_usec = diff % 1000000;
    return (diff<0);
}

int main(int argc, char *argv[])
{
	int  accRaw[3];
	int  magRaw[3];
	int  gyrRaw[3];

	int startInt  = mymillis();

	signal(SIGINT, INThandler);

	detectIMU();
	enableIMU();

    printf ("DT = %f", DT);

	for (int i=0; i<SAMPLES; ++i)
	{
		startInt = mymillis();

		readACC(accRaw);
		readGYR(gyrRaw);
		readMAG(magRaw);
        
		printf ("%d %d %d    %d %d %d    %d %d %d",
                accRaw[0],accRaw[1],accRaw[2],
                gyrRaw[0],gyrRaw[1],gyrRaw[2],
                magRaw[0],magRaw[1],magRaw[2]);
        
		// Each loop should be at least 20ms.
		while (mymillis()-startInt<DT*1000)
            usleep(100);

		printf("%d\n", mymillis()- startInt);
    }
}

