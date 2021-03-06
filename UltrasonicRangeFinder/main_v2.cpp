#include <pigpio.h>
#include <iostream>
#include <time.h>

#include <stdlib.h>
#include <sys/time.h>


using namespace std;


struct timeval tv;

double getTime()
{
	gettimeofday(&tv, NULL);

    return (double)tv.tv_sec + (double)tv.tv_usec * 0.000001;
}


int coefficient = 17150;//12550.0; // nalezy dobrac go doswiadczalnie

int TRIG = 23;
int ECHO = 24;

void initializePins()
{
    gpioSetMode(TRIG, PI_OUTPUT);
    gpioSetMode(ECHO, PI_INPUT);
}

bool waitValue(int value, int limit = 10000)
{
    for(int i = 0; gpioRead(ECHO) == value; ++i)
    {
        if(i >= limit)
            return false;
    }

    return true;
}

double detectDistance()
{
    gpioWrite(TRIG, 0);
    usleep(500000);
    gpioWrite(TRIG, 1);
    usleep(10);
    gpioWrite(TRIG, 0);

    if(waitValue(0))
    {
        double pulseStart = getTime();

        if(waitValue(1))
        {
            double pulseEnd = getTime();

            double duration = pulseEnd - pulseStart;
            double distance = duration * coefficient;

            return distance;
        }
    }
}

int main () 
{
	if(gpioInitialise() < 0){
		cout << "pigpio initialisation failed" << endl; 
	}
	else
	{
		cout << "pigpio initialisation ok" << endl;

        initializePins();

        double sum = 0.0;

        // aby poprawic wynik
        for(int i = 0; i < 10; ++i)
            sum += detectDistance();

        double distance = sum / 10;

        cout << "Odleglosc: " << distance << "cm" << endl;
	}

	gpioTerminate();

	return 0; 
}
