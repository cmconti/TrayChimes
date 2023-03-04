TrayChimes v1.30

written by Chris Conti

Program requirements:
Windows 10

This program installs a tray icon which plays chimes on every quarter hour.
You can also set an alarm for any time of the day which will cause a sound
to be played at that time.  The interface should be self explanatory-
just right click or double click the TrayChimes icon in the system tray
to display the configuration dialog.  You can terminate the program by
right clicking the icon and selecting Exit.

Chris Conti

Release Info:
1.0     24 October, 96
        1.  Initial Version

1.1     20 March, 97
        1.  Added options to only play alarm once and to display a message box when the
            alarm goes off.
        2.  Fixed problem with bringing window to the top of the screen when the 
            configuration dialog is already visible, but covered by other windows

1.2     23 June, 97
        1.  Stretched size of main window to better show the wave file paths.
        2.  Made the alarm notification a topmost window.
        3.  Added snooze button to alarm notification.
        4.  Added next alarm to tray icon tool tip.
        5.  Made sure only one instance could run at a time. Also, if explorer has restarted,
            when a second instance tries to start, the first instance makes sure its icon
            appears in the tray.

1.21    30 April, 98
        1.  Changed speaker buttons to use an icon instead of a bitmap so they display correctly
            when the user has altered the 3-d object color.
        2.  Fixed problem where tabbing around the controls would get stuck in the time edit box.
        3.  Setup image: replaced the default alarm wave file with a generic alarm sound.
        4.  Stopped multiple alarm dialogs from appearing which could happen if the computer were
            left running unattended for several days.

1.22   23 February, 00
        1.  Fixed problem of tray icon disappearing when explorer restarts (n.b. requires IE4 w/shell
            update or Win98/Win200 (or later) to work ).
        2.  Recompiled with VC6 to reduce exe size.

1.30   27 February, 23
        1.  Rebuilt program using VisualStudio 2022
        2.  Rebuilt installer using Inno Setup
        3.  Updated icons and graphics
        4.  Added option to run on startup
        5.  Use common time picker control
        6.  Targeted Windows 10 SDK
        7.  Clean up loading and saving data
        8.  Updated wav files with ones under a known CC license

TODO:
        1.  Add silent hours for chimes
        2.  Add mute (also cancel any currently playing chimes/alarms)
        3.  Modernize UI
        4.  Stop program on uninstall
        5.  Better alarm controls/granularity (e.g. weekday only, multiple alarms, etc.)
        6.  Better icons and graphics
        7.  Fix snooze with 'play alarm once'
        8.  Github actions

Attributions:
- Clock Image by Icon-Icons
  - https://icon-icons.com/icon/clock-time-furniture/209725
  - https://icon-icons.com/license
- Speaker Image by Microsoft from the Visual Studio Image library.
  - https://learn.microsoft.com/en-us/visualstudio/designers/the-visual-studio-image-library?view=vs-2022
- Hour chime
  - adapted from https://freesound.org/people/QuartzMMN/sounds/448693
  - CC BY 4.0 https://creativecommons.org/licenses/by/4.0/
- Alarm bell
  - adapted from https://freesound.org/people/soundrecorder7/sounds/141287
  - CC BY 3.0 https://creativecommons.org/licenses/by/3.0/
- Westminster chime
  - adapted from https://freesound.org/people/DWOBoyle/sounds/449275
  - CC BY 3.0 https://creativecommons.org/licenses/by/3.0/
