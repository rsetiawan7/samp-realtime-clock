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

#if !defined _INC_a_samp
  #error Include `<a_samp> first.`
#endif

// Variables

static g_RT_Hour = 0;
static g_RT_Minute = 0;
static g_RT_TimerInterval = 1000;
static g_RT_Timer;
static bool:g_RT_PlayerTime_Freeze[MAX_PLAYERS];

// Forwards

forward OnWorldTimeUpdate(hour, minute);

// Non-public functions

stock RealTime_IsPlayerFreezed(playerid)
{
  if (playerid < 0 || playerid > MAX_PLAYERS)
  {
    return 0;
  }

  if (!IsPlayerConnected(playerid))
  {
    return 0;
  }

  return (g_RT_PlayerTime_Freeze[playerid] == true ? 1 : 0);
}

stock RealTime_FreezeForPlayer(playerid)
{
  if (playerid < 0 || playerid > MAX_PLAYERS)
  {
    return 0;
  }

  if (!IsPlayerConnected(playerid))
  {
    return 0;
  }

  g_RT_PlayerTime_Freeze[playerid] = true;
  return 1;
}

stock RealTime_UnfreezeForPlayer(playerid)
{
  if (playerid < 0 || playerid > MAX_PLAYERS)
  {
    return 0;
  }

  if (!IsPlayerConnected(playerid))
  {
    return 0;
  }

  g_RT_PlayerTime_Freeze[playerid] = true;
  return 1;
}

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
    if (!RealTime_IsPlayerFreezed(p))
    {
      RealTime_SyncPlayerWorldTime(p);
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

stock void:RealTime_SetWorldTime(hour, minute, bool:restart_timer = true, bool:broadcast = true)
{
  RealTime_SetHour(hour);
  RealTime_SetMinute(minute);

  if (restart_timer)
  {
    RealTime_StartTime();
  }

  RealTime_UpdatePlayerWorldTime();

  if (broadcast)
  {
    CallRemoteFunction("OnWorldTimeUpdate", "dd", hour, minute);
  }
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

// Built-in callbacks

#if defined FILTERSCRIPT
  public OnFilterScriptInit()
  {
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
      if (IsPlayerConnected(i))
      {
        RealTime_UnfreezeForPlayer(i);
      }
    }

    #if defined RT_OnFilterScriptInit
      return RT_OnFilterScriptInit();
    #else
      return 1;
    #endif
  }
  #if defined _ALS_OnFilterScriptInit
    #undef OnPlayerConnect
  #else
    #define _ALS_OnFilterScriptInit
  #endif
  #define OnFilterScriptInit RT_OnFilterScriptInit
  #if defined RT_OnFilterScriptInit
    forward RT_OnFilterScriptInit();
  #endif
#else
  public OnGameModeInit()
  {
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
      if (IsPlayerConnected(i))
      {
        RealTime_UnfreezeForPlayer(i);
      }
    }

    #if defined RT_OnGameModeInit
      return RT_OnGameModeInit();
    #else
      return 1;
    #endif
  }
  #if defined _ALS_OnGameModeInit
    #undef OnPlayerConnect
  #else
    #define _ALS_OnGameModeInit
  #endif
  #define OnGameModeInit RT_OnGameModeInit
  #if defined RT_OnGameModeInit
    forward RT_OnGameModeInit();
  #endif
#endif

public OnPlayerConnect(playerid)
{
  g_RT_PlayerTime_Freeze[playerid] = false;

  #if defined RT_OnPlayerConnect
    return RT_OnPlayerConnect(playerid);
  #else
    return 1;
  #endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect RT_OnPlayerConnect
#if defined RT_OnPlayerConnect
	forward RT_OnPlayerConnect(playerid);
#endif
