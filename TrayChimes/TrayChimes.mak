# Microsoft Developer Studio Generated NMAKE File, Based on TrayChimes.dsp
!IF "$(CFG)" == ""
CFG=TrayChimes - Win32 Release
!MESSAGE No configuration specified. Defaulting to TrayChimes - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "TrayChimes - Win32 Release" && "$(CFG)" !=\
 "TrayChimes - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TrayChimes.mak" CFG="TrayChimes - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TrayChimes - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TrayChimes - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TrayChimes - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\TrayChimes.exe"

!ELSE 

ALL : "$(OUTDIR)\TrayChimes.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AlarmDlg.obj"
	-@erase "$(INTDIR)\MessageDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TimeSelection.obj"
	-@erase "$(INTDIR)\TrayChimes.obj"
	-@erase "$(INTDIR)\TrayChimes.pch"
	-@erase "$(INTDIR)\TrayChimes.res"
	-@erase "$(INTDIR)\TrayChimesDlg.obj"
	-@erase "$(OUTDIR)\TrayChimes.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\TrayChimes.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TrayChimes.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TrayChimes.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\TrayChimes.pdb" /machine:I386 /out:"$(OUTDIR)\TrayChimes.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AlarmDlg.obj" \
	"$(INTDIR)\MessageDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TimeSelection.obj" \
	"$(INTDIR)\TrayChimes.obj" \
	"$(INTDIR)\TrayChimes.res" \
	"$(INTDIR)\TrayChimesDlg.obj"

"$(OUTDIR)\TrayChimes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TrayChimes - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\TrayChimes.exe"

!ELSE 

ALL : "$(OUTDIR)\TrayChimes.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AlarmDlg.obj"
	-@erase "$(INTDIR)\MessageDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TimeSelection.obj"
	-@erase "$(INTDIR)\TrayChimes.obj"
	-@erase "$(INTDIR)\TrayChimes.pch"
	-@erase "$(INTDIR)\TrayChimes.res"
	-@erase "$(INTDIR)\TrayChimesDlg.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\TrayChimes.exe"
	-@erase "$(OUTDIR)\TrayChimes.ilk"
	-@erase "$(OUTDIR)\TrayChimes.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_MBCS" /Fp"$(INTDIR)\TrayChimes.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TrayChimes.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TrayChimes.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=winmm.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\TrayChimes.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\TrayChimes.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AlarmDlg.obj" \
	"$(INTDIR)\MessageDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TimeSelection.obj" \
	"$(INTDIR)\TrayChimes.obj" \
	"$(INTDIR)\TrayChimes.res" \
	"$(INTDIR)\TrayChimesDlg.obj"

"$(OUTDIR)\TrayChimes.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "TrayChimes - Win32 Release" || "$(CFG)" ==\
 "TrayChimes - Win32 Debug"
SOURCE=.\AlarmDlg.cpp
DEP_CPP_ALARM=\
	".\AlarmDlg.h"\
	".\StdAfx.h"\
	".\TimeSelection.h"\
	".\TrayChimes.h"\
	".\TrayChimesDlg.h"\
	

"$(INTDIR)\AlarmDlg.obj" : $(SOURCE) $(DEP_CPP_ALARM) "$(INTDIR)"\
 "$(INTDIR)\TrayChimes.pch"


SOURCE=.\MessageDlg.cpp
DEP_CPP_MESSA=\
	".\MessageDlg.h"\
	".\StdAfx.h"\
	".\TrayChimes.h"\
	

"$(INTDIR)\MessageDlg.obj" : $(SOURCE) $(DEP_CPP_MESSA) "$(INTDIR)"\
 "$(INTDIR)\TrayChimes.pch"


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "TrayChimes - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_MBCS" /Fp"$(INTDIR)\TrayChimes.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TrayChimes.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TrayChimes - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_MBCS" /Fp"$(INTDIR)\TrayChimes.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TrayChimes.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TimeSelection.cpp
DEP_CPP_TIMES=\
	".\StdAfx.h"\
	".\TimeSelection.h"\
	

"$(INTDIR)\TimeSelection.obj" : $(SOURCE) $(DEP_CPP_TIMES) "$(INTDIR)"\
 "$(INTDIR)\TrayChimes.pch"


SOURCE=.\TrayChimes.cpp
DEP_CPP_TRAYC=\
	".\AlarmDlg.h"\
	".\StdAfx.h"\
	".\TimeSelection.h"\
	".\TrayChimes.h"\
	".\TrayChimesDlg.h"\
	

"$(INTDIR)\TrayChimes.obj" : $(SOURCE) $(DEP_CPP_TRAYC) "$(INTDIR)"\
 "$(INTDIR)\TrayChimes.pch"


SOURCE=.\TrayChimes.rc
DEP_RSC_TRAYCH=\
	".\res\regulator.bmp"\
	".\res\speaker.bmp"\
	".\res\speaker.ico"\
	".\res\tray_clock.ico"\
	".\res\TrayChimes.ico"\
	".\res\TrayChimes.rc2"\
	

"$(INTDIR)\TrayChimes.res" : $(SOURCE) $(DEP_RSC_TRAYCH) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\TrayChimesDlg.cpp
DEP_CPP_TRAYCHI=\
	".\AlarmDlg.h"\
	".\MessageDlg.h"\
	".\StdAfx.h"\
	".\TimeSelection.h"\
	".\TrayChimes.h"\
	".\TrayChimesDlg.h"\
	

"$(INTDIR)\TrayChimesDlg.obj" : $(SOURCE) $(DEP_CPP_TRAYCHI) "$(INTDIR)"\
 "$(INTDIR)\TrayChimes.pch"



!ENDIF 

