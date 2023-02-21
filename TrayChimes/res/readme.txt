TrayChimes v1.22

written by Chris Conti

Program requirements:
Windows 95 or Windows NT 4.0
wave file (sound) output device

This program installs a tray icon which plays chimes on every quarter hour.
You can also set an alarm for any time of the day which will cause a sound
to be played at that time.  The interface should be self explanatory-
just right click or double click the TrayChimes icon in the system tray
to display the configuration dialog.  You can terminate the program by
right clicking the icon and selecting Exit.

If you have any problems, questions or comments, feel free to write
to me at cmconti@aol.com

Release Info:
1.0 	24 October, 96
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
            TODO: Find a way to let all of the time fields be edited from the keyboard.
        3.  Setup image: replaced the default alarm wave file with a generic alarm sound.
        4.  Stopped multiple alarm dialogs from appearing which could happen if the computer were
            left running unattended for several days.

1.22   23 February, 00
        1.  Fixed problem of tray icon disappearing when explorer restarts (n.b. requires IE4
            w/shell update or Win98/Win200 (or later) to work ).
        2.  Recompiled with VC6 to reduce exe size.