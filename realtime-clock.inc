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

stock RealTime_SyncPlayerWorldTime(playerid)
{
  if (!IsPlayerConnected(playerid))
  {
    return 0;
  }

  SetPlayerTime(playerid, RealTime_GetHour(), RealTime_GetMinute());
  return 1;
}

stock void:RealTime_UpdatePlayerWorldTime()
{
  for (new p = 0; p < MAX_PLAYERS; p++)
  {
    RealTime_SyncPlayerWorldTime(p);
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

stock RealTime_GetInterval()
{
  return g_RT_TimerInterval;
}

stock void:RealTime_SetHour(hour, bool:broadcast = true)
{
  g_RT_Hour = hour % 24;
  RealTime_UpdatePlayerWorldTime();

  if (broadcast)
  {
    // Broadcast the callback.
    CallRemoteFunction("OnWorldTimeUpdate", "dd", RealTime_GetHour(), RealTime_GetMinute());
  }
}

stock void:RealTime_SetMinute(minute, bool:broadcast = true)
{
  g_RT_Minute = minute % 60;
  RealTime_UpdatePlayerWorldTime();

  if (broadcast)
  {
    // Broadcast the callback.
    CallRemoteFunction("OnWorldTimeUpdate", "dd", RealTime_GetHour(), RealTime_GetMinute());
  }
}

stock void:RealTime_StopTime()
{
  KillTimer(g_RT_Timer);
}

stock void:RealTime_StartTime()
{
  // To make sure it's not running twice!
  RealTime_StopTime();
  g_RT_Timer = SetTimer("RealTime_UpdateWorldTime", RealTime_GetInterval(), true);
}

stock void:RealTime_Sync()
{
  // Restart the timer first.
  RealTime_StartTime();

  // Get time of server.
  gettime(g_RT_Hour, g_RT_Minute);

  // Then, update the player world time.
  RealTime_UpdatePlayerWorldTime();

  // Broadcast the callback.
  CallRemoteFunction("OnWorldTimeUpdate", "dd", RealTime_GetHour(), RealTime_GetMinute());
}

stock void:RealTime_SetInterval(interval, bool:restart_timer = true)
{
  g_RT_TimerInterval = interval;

  if (restart_timer)
  {
    RealTime_StartTime();
  }
}

stock void:RealTime_SetWorldTime(hour, minute, bool:restart_timer = true)
{
  RealTime_SetHour(hour);
  RealTime_SetMinute(minute);

  if (restart_timer)
  {
    RealTime_StartTime();
  }

  RealTime_UpdatePlayerWorldTime();
}

// Public functions

forward void:RealTime_UpdateWorldTime();
public void:RealTime_UpdateWorldTime()
{
  RealTime_SetMinute(RealTime_GetMinute() + 1, false);

  if (RealTime_GetMinute() == 0)
  {
    RealTime_SetHour(RealTime_GetHour() + 1, false);
  }

  RealTime_UpdatePlayerWorldTime();
  CallRemoteFunction("OnWorldTimeUpdate", "dd", RealTime_GetHour(), RealTime_GetMinute());
}
