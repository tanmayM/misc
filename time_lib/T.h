#ifndef __T__
#define __T__

typedef struct T
{
    unsigned long sec;
    unsigned long nsec;
}T;


/* --------- A P I    P A R T 1 -------*/


/* T_tick: Record current time */
void T_Tick();

/* T_GetTick(): Get current time.
   Returns {0,0} in case of error
*/
T T_GetTick();

/* T_PrintTick(): Print current time.
   Prints in Secs.MicroSeconds format.
*/
void T_PrintTick();

/* T_printElapsed(): Prints the time elapsed since last call to
   T_tick() or T_printElapsed(). Prepends the string s to the message.
*/
void T_PrintElapsed(char *s);


/* --------- A P I    P A R T 2 -------*/

/* T_timeForNCalls(): This function can be used to find the frequency of call to this function.
   Useful in real time application to find out if you are meeting a given rate. (e.g. frames per second).

   First call to this function will start the measurement. Caller needs to call with same value of
   'N' for subsequent calls. After N calls, it will print the time elapsed since first call and
   the 'calls/sec' that it implies.

   If the value of N is changed in between, the measurements are reset and restarted.
   N=0 will reset the api.

*/
void T_TimeForNCalls(int N);


/* --------- A P I    P A R T 3 -------*/

/* T_TIMETAKEN: This macro should be used to measure time taken by the function 'x'
   to complete execution.

   function t_tick() and t_printtick() are private and SHOULD NOT BE USED directly
*/
void t_tickonce();

#define T_TIMETAKEN(x) { {t_tickonce();} {x;} {t_tickonce();}}

#endif
