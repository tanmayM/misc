#include<stdio.h>
#include <time.h>
#include "T.h"

T tick = {0,0};

T nticks[20];

/* res = t1-t2*/
void t_sub(T t1, T t2, T *res)
{
    res->sec = t1.sec - t2.sec;

    if(t1.nsec < t2.nsec)
    {
        res->nsec = t1.nsec + 1000000000 - t2.nsec;
        res->sec--;
    }
    else
    {
        res->nsec = t1.nsec - t2.nsec;
    }
}

void t_tick(T *t)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    t->sec = (unsigned long)ts.tv_sec;
    t->nsec = (unsigned long)ts.tv_nsec;
}


void T_Tick()
{
    t_tick(&tick);
}

T T_GetTick()
{
    T_Tick();
    return tick;
}

void T_PrintTick()
{
    T_Tick();
    printf("CurTime:%f sec\n", tick.sec + ((double)tick.nsec/1000000000));
}

void T_PrintElapsed(char *s)
{
    T t, res={0,0};

    t = tick;

    if(!tick.sec && !tick.nsec)
    {
        T_Tick();
        return;
    }
 
    T_Tick();

    t_sub(tick, t, &res);
    printf("%s: time elapsed: %f msec\n", s, res.sec*1000 + ((double)res.nsec)/1000000);
}

void T_TimeForNCalls(int N)
{
    static T ntick={0,0}, ntick2={0,0};
    static int n=0,c=0;

    if(n!=N)
    {
        n = N;
        c=n;
        t_tick(&ntick);
    }

    c--;

    if(!c)
    {
        T res={0,0};
        int cps=0;
        double tmp;

        t_tick(&ntick2);
        t_sub(ntick2, ntick, &res);   
        tmp = (double)res.sec + ((double)res.nsec)/1000000000;

        cps = n/tmp;
        printf("Time for %d calls=%f sec\t -->\t Calls per second=%d\n", n, tmp, cps);

        c=n;
        ntick = ntick2;
    }
    
}

void t_tickonce()
{
    static T t1 = {0,0}, t2, res;

    if(!t1.sec && !t1.nsec)
    {
        t_tick(&t1);
        return;
    }

    t_tick(&t2);

    t_sub(t2, t1, &res);

    printf("func time: %f msec\n", res.sec*1000 + ((double)res.nsec/1000000));

    t1.sec = t1.nsec = 0;
    t2.sec = t2.nsec = 0;
    res.sec = res.nsec = 0;
}



#if 0
main()
{
    T_Tick();
    T_PrintTick();
    T_PrintElapsed(NULL);
    usleep(1000000);
    T_PrintElapsed(NULL);

    while(1)
    {
        usleep(10000);
        T_TimeForNCalls(100);
    }

    //T_TIMETAKEN(printf("Hello World\n");)
    return 0;   
}
#endif
