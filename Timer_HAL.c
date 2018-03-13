//------------------------------------------
// Timer API (Application Programming Interface)
// Also known as BUTTON HAL (Hardware Abstraction Layer)
// HAL is a specific form of API that designs the interface with a certain hardware

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <TIMER_HAL.h>
#define TIMER0_PRESCALER 256
#define TIMER1_PRESCALER 1
#define SYS_CLOCK_F 3 // We measure the time in us and therefore, we store frequency in MHz


void InitOneShotSWTimer(
        OneShotSWTimer_t* OST,
        uint32_t  hwtimer,
        uint32_t  waitCycles)
{
    OST->hwtimer = hwtimer;
    OST->waitCycles  = waitCycles;
}

void StartOneShotSWTimer(OneShotSWTimer_t* OST)
{
    OST->startCounter = Timer32_getValue(OST->hwtimer);
}

bool OneShotSWTimerExpired(OneShotSWTimer_t* OST)
{
    bool expired = false;

    // HW timer period
    int64_t HWTimerPeriod = UINT32_MAX+ 1;

    //This is C2 from notes, while OST->startCounter is C1
    uint32_t currentCounter = Timer32_getValue(OST->hwtimer);

    int64_t ElapsedCycles =  OST->startCounter - currentCounter;
    if (ElapsedCycles < 0)
        ElapsedCycles += HWTimerPeriod ;

    if (ElapsedCycles >= OST->waitCycles)
        expired = true;
    else
        expired = false;

    return expired;
}


void InitHWTimers() {
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_256, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_0_BASE, UINT32_MAX);
    Timer32_startTimer(TIMER32_0_BASE, false);

    Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_1_BASE, UINT32_MAX);
    Timer32_startTimer(TIMER32_1_BASE, false);
}

 uint64_t WaitCycles(uint32_t hwtimer, uint32_t TimeInUs)
  {
      uint64_t waitCycles;
      if (hwtimer == TIMER32_0_BASE)
          waitCycles = TimeInUs * SYS_CLOCK_F / TIMER0_PRESCALER;

      else  if (hwtimer == TIMER32_1_BASE)
          waitCycles = TimeInUs * SYS_CLOCK_F / TIMER1_PRESCALER;

      return waitCycles;
  }



