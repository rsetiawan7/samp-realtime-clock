# samp-realtime-clock

[![sampctl](https://img.shields.io/badge/sampctl-samp--realtime--clock-2f2f2f.svg?style=for-the-badge)](https://github.com/rsetiawan7/samp-realtime-clock)

A library that sync your world time to your hour and minute for all players.

You can control the world time whatever you like with some of short functions.

<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->

## Installation

Simply install to your project:

```bash
sampctl package install rsetiawan7/samp-realtime-clock
```

Include in your code and begin using the library:

```pawn
#include <realtime-clock>
```

## Usage

- Update your server info's worldtime when the time's changed.

```c
public OnWorldTimeUpdate(hour, minute)
{
  // Update the server info's worldtime.
  new
    str[16]
  ;

  format(str, sizeof(str), "worldtime %02d:%02d", hour, minute);
  SendRconCommand(str);
}
```

- Change interval of the time update to 1 minute in GTA SA = 30 seconds.

```c
// Set the interval and restart the timer.
RealTime_SetInterval(30000);
```

- Sync your current time with server time.

```c
Realtime_Sync();

// Called after sync done.
public OnWorldTimeUpdate(hour, minute)
{
  return 1;
}
```

- Stop the time change and set the world time to always night.

```c
// Stop the time first.
RealTime_StopTime();

// Then, set the world time to 00:00 and don't start the timer again.
RealTime_SetWorldTime(0, 0, false);
```

### Functions

```c
// Set each of player's world time with current time.
void:RealTime_UpdatePlayerWorldTime();

// Get current hour.
RealTime_GetHour();

// Get current minute.
RealTime_GetMinute();

// Get current interval of time.
RealTime_GetInterval();

// Set current hour to specific hour.
void:RealTime_SetHour(hour, bool:broadcast = true);

// Set current minute to specific minute.
void:RealTime_SetMinute(minute, bool:broadcast = true);

// Stop updating time. (more likely freeze the current time)
void:RealTime_StopTime();

// Start (again if you already stopped before) to updating time.
void:RealTime_StartTime();

// Sync current time with server time.
void:RealTime_Sync();

// Set interval of time changer (in millisecond).
void:RealTime_SetInterval(interval, bool:restart_timer = true);

// Set current time with hour and minute.
void:RealTime_SetWorldTime(hour, minute, bool:restart_timer = true, bool:broadcast = true);

// Sync current player to current time.
RealTime_SyncPlayerWorldTime(playerid);
```

### Callbacks

```c
// Called every the time updated.
public OnWorldTimeUpdate(hour, minute)
{
  // Do something here ...
}
```

<!--
Write your code documentation or examples here. If your library is documented in
the source code, direct users there. If not, list your API and describe it well
in this section. If your library is passive and has no API, simply omit this
section.
-->

You can view example of usage this library on `filterscripts/realtime_clock.pwn`.

## Testing

<!--
Depending on whether your package is tested via in-game "demo tests" or
y_testing unit-tests, you should indicate to readers what to expect below here.
-->

To test, simply run the package:

```bash
sampctl package run
```
