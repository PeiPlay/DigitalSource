#include "HRpwm.h"

void HRpwm_Init(HRpwm_t* hrpwm)
{
   uint32_t OutputsToStart;
   switch (hrpwm->timerIdentifier)
   {
#ifdef HRTIM_TIMERID_TIMER_A
   case HRTIM_TIMERID_TIMER_A:
      hrpwm->_timerIndex = HRTIM_TIMERINDEX_TIMER_A;
      hrpwm->period = hrpwm->hhrtim->Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].PERxR;
      OutputsToStart = HRTIM_OUTPUT_TA1 | HRTIM_OUTPUT_TA2;
      break;
#endif
#ifdef HRTIM_TIMERID_TIMER_B
   case HRTIM_TIMERID_TIMER_B:
      hrpwm->_timerIndex = HRTIM_TIMERINDEX_TIMER_B;
      hrpwm->period = hrpwm->hhrtim->Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].PERxR;
      OutputsToStart = HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2;
      break;
#endif
#ifdef HRTIM_TIMERID_TIMER_C
   case HRTIM_TIMERID_TIMER_C:
      hrpwm->_timerIndex = HRTIM_TIMERINDEX_TIMER_C;
      hrpwm->period = hrpwm->hhrtim->Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].PERxR;
      OutputsToStart = HRTIM_OUTPUT_TC1 | HRTIM_OUTPUT_TC2;
      break;
#endif
#ifdef HRTIM_TIMERID_TIMER_D
   case HRTIM_TIMERID_TIMER_D:
      hrpwm->_timerIndex = HRTIM_TIMERINDEX_TIMER_D;
      hrpwm->period = hrpwm->hhrtim->Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].PERxR;
      OutputsToStart = HRTIM_OUTPUT_TD1 | HRTIM_OUTPUT_TD2;
      break;
#endif
#ifdef HRTIM_TIMERID_TIMER_E
   case HRTIM_TIMERID_TIMER_E:
      hrpwm->_timerIndex = HRTIM_TIMERINDEX_TIMER_E;
      hrpwm->period = hrpwm->hhrtim->Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_E].PERxR;
      OutputsToStart = HRTIM_OUTPUT_TE1 | HRTIM_OUTPUT_TE2;
      break;
#endif
#ifdef HRTIM_TIMERID_TIMER_F
   case HRTIM_TIMERID_TIMER_F:
      hrpwm->_timerIndex = HRTIM_TIMERINDEX_TIMER_F;
      hrpwm->period = hrpwm->hhrtim->Instance->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_F].PERxR;
      OutputsToStart = HRTIM_OUTPUT_TF1 | HRTIM_OUTPUT_TF2;
      break;
#endif
   default:
       Error_Handler();
       break;
   }
   
   if(hrpwm->dutyLimit > 1.0f) hrpwm->dutyLimit = 1.0f;
   if(hrpwm->dutyLimit < 0.0f) hrpwm->dutyLimit = 0.0f;
   if(hrpwm->dutyLimit < 0.5f) hrpwm->dutyLimit = 0.5f - hrpwm->dutyLimit;
   
   if(hrpwm->defaultDuty > hrpwm->dutyLimit) hrpwm->defaultDuty = hrpwm->dutyLimit;
   if(hrpwm->defaultDuty < (1.0f - hrpwm->dutyLimit)) hrpwm->defaultDuty = 1.0f - hrpwm->dutyLimit;

   __HAL_HRTIM_SETCOMPARE(hrpwm->hhrtim, hrpwm->_timerIndex, HRTIM_COMPAREUNIT_1, (uint32_t)(hrpwm->defaultDuty * hrpwm->period));
   
   HAL_HRTIM_WaveformOutputStart(&hhrtim, OutputsToStart);
   HAL_HRTIM_WaveformCounterStart(&hhrtim, (hrpwm->timerIdentifier) | HRTIM_TIMERID_MASTER);

   
}
inline void HRpwm_SetDuty(HRpwm_t* hrpwm, float duty)
{
   if(duty > hrpwm->dutyLimit) duty = hrpwm->dutyLimit;
   if(duty < (1.0f - hrpwm->dutyLimit)) duty = 1.0f - hrpwm->dutyLimit;
   __HAL_HRTIM_SETCOMPARE(hrpwm->hhrtim, hrpwm->_timerIndex, HRTIM_COMPAREUNIT_1, (uint32_t)(duty * hrpwm->period));
   hrpwm->duty = duty;
}
inline float HRpwm_GetDuty(HRpwm_t* hrpwm)
{
   return hrpwm->duty;
}







