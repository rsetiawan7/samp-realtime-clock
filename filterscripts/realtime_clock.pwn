#define FILTERSCRIPT

#include <a_samp>
#include <realtime-clock>

new
  Text:g_TDClock
;

void:SetClock(hour, minute)
{
  new
    timestr[8]
  ;

  format(timestr, sizeof(timestr), "%02d:%02d", hour, minute);
  TextDrawSetString(g_TDClock, timestr);
}

ShowClock(playerid)
{
  if (!IsPlayerConnected(playerid))
  {
    return 0;
  }

  return TextDrawShowForPlayer(playerid, g_TDClock);
}

ShowClockToAllPlayers()
{
  return TextDrawShowForAll(g_TDClock);
}

HideClockToAllPlayers()
{
  return TextDrawHideForAll(g_TDClock);
}

public OnFilterScriptInit()
{
  // This design of textdraw picked from filterscript 'gl_realtime'.
  g_TDClock = TextDrawCreate(605.0, 25.0, "00:00");

  TextDrawUseBox(g_TDClock, 0);
  TextDrawFont(g_TDClock, 3);
  TextDrawSetShadow(g_TDClock, 0); // no shadow
  TextDrawSetOutline(g_TDClock, 2); // thickness 1
  TextDrawBackgroundColor(g_TDClock, 0x000000FF);
  TextDrawColor(g_TDClock, 0xFFFFFFFF);
  TextDrawAlignment(g_TDClock, 3);
  TextDrawLetterSize(g_TDClock, 0.5, 1.5);

  // Set interval and don't start the timer first.
  RealTime_SetInterval(10000, false);

  // Sync realtime with server time.
  RealTime_Sync();

  ShowClockToAllPlayers();

  SetClock(RealTime_GetHour(), RealTime_GetMinute());
  return 1;
}

public OnFilterScriptExit()
{
  RealTime_StopTime();
  HideClockToAllPlayers();
  return 1;
}

public OnPlayerConnect(playerid)
{
  ShowClock(playerid);
  SetPlayerTime(playerid, RealTime_GetHour(), RealTime_GetMinute());
  return 1;
}

public OnWorldTimeUpdate(hour, minute)
{
  SetClock(hour, minute);

  // Update the worldtime's server info.
  new
    str[16]
  ;

  format(str, sizeof(str), "worldtime %02d:%02d", hour, minute);
  SendRconCommand(str);
  return 1;
}
