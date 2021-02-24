// built-in include guard removal
// just in case the user has a local dependency with the same file name
#if defined _inc_realtime
	#undef _inc_realtime
#endif
// custom include-guard to ensure we don't duplicate
#if defined _realtime_included
	#endinput
#endif
#define _realtime_included

// Includes

#include <a_samp>

// Variables

new
  g_RT_Hour = 0,
  g_RT_Minute = 0,
  g_RT_TimerInterval = 1000,
  g_RT_Timer
;

// Forwards

forward OnWorldTimeUpdate(hour, minute);

// Non-public functions

stock void:RealTime_UpdatePlayerWorldTime()
{
  for (new p = 0; p < MAX_PLAYERS; p++)
  {
    if (IsPlayerConnected(p))
    {
      SetPlayerTime(p, RealTime_GetHour(), RealTime_GetMinute());
    }
  }
}

stock RealTime_GetHour()
{
  return g_RT_Hour;
}

stock RealTime_GetMinute()
{
  return g_RT_Minute;
}

stock void:RealTime_SetHour(hour)
{
  g_RT_Hour = hour % 24;
  RealTime_UpdatePlayerWorldTime();
}

stock void:RealTime_SetMinute(minute)
{
  g_RT_Minute = minute % 60;
  RealTime_UpdatePlayerWorldTime();
}

stock void:RealTime_StartTime()
{
  g_RT_Timer = SetTimer("RealTime_UpdateWorldTime", g_RT_TimerInterval, true);
}

stock void:RealTime_StopTime()
{
  KillTimer(g_RT_Timer);
}

stock void:RealTime_Sync()
{
  // Stop the timer first.
  RealTime_StopTime();

  // Get time of server.
  gettime(g_RT_Hour, g_RT_Minute);

  // Then, update the player world time.
  RealTime_UpdatePlayerWorldTime();

  // Don't forget to start the timer again.
  RealTime_StartTime();
}

stock void:RealTime_RestartTimer()
{
  RealTime_StopTime();
  RealTime_StartTime();
}

stock void:RealTime_SetInterval(interval)
{
  g_RT_TimerInterval = interval;
  RealTime_RestartTimer();
}

stock void:RealTime_SetWorldTime(hour, minute, bool:reset_timer = true)
{
  g_RT_Hour = hour % 24;
  g_RT_Minute = minute % 60;

  if (reset_timer)
  {
    RealTime_RestartTimer();
  }
}

// Public functions

forward void:RealTime_UpdateWorldTime();
public void:RealTime_UpdateWorldTime()
{
  g_RT_Minute = (RealTime_GetMinute() + 1) % 60;

  if (g_RT_Minute == 0)
  {
    g_RT_Hour = (RealTime_GetHour() + 1) % 24;
  }

  printf("Hour: %02d | Minute: %02d", RealTime_GetHour(), RealTime_GetMinute());
  RealTime_UpdatePlayerWorldTime();
  CallRemoteFunction("OnWorldTimeUpdate", "dd", g_RT_Hour, g_RT_Minute);
}
