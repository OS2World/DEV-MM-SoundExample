# makefile
# Created by IBM WorkFrame/2 MakeMake at 18:54:04 on 24 Aug 1997
#
# The actions included in this make file are:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .c .obj .rc .res 

.all: \
    E:\C_Projects\SoundDemo_Source\snddemo.exe

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe /Ss /Ti /G5 /Fo"%|dpfF.obj" /C %s

E:\C_Projects\SoundDemo_Source\snddemo.exe: \
    E:\C_Projects\SoundDemo_Source\snddemo.obj \
    E:\C_Projects\SoundDemo_Source\sndos2.obj \
    E:\C_Projects\SoundDemo_Source\dsp.obj \
    E:\C_Projects\SoundDemo_Source\dialog.res \
    {$(LIB)}mmpm2.lib
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     /B" /de /exepack:2 /pmtype:pm"
     /FeE:\C_Projects\SoundDemo_Source\snddemo.exe 
     mmpm2.lib 
     E:\C_Projects\SoundDemo_Source\snddemo.obj
     E:\C_Projects\SoundDemo_Source\sndos2.obj
     E:\C_Projects\SoundDemo_Source\dsp.obj
<<
    rc.exe E:\C_Projects\SoundDemo_Source\dialog.res E:\C_Projects\SoundDemo_Source\snddemo.exe

E:\C_Projects\SoundDemo_Source\dialog.res: \
    E:\C_Projects\SoundDemo_Source\dialog.rc \
    {$(INCLUDE)}dialog.h

E:\C_Projects\SoundDemo_Source\snddemo.obj: \
    E:\C_Projects\SoundDemo_Source\snddemo.c \
    {E:\C_Projects\SoundDemo_Source;$(INCLUDE);}SndOS2.h \
    {E:\C_Projects\SoundDemo_Source;$(INCLUDE);}dsp.h \
    {E:\C_Projects\SoundDemo_Source;$(INCLUDE);}dialog.h

E:\C_Projects\SoundDemo_Source\dsp.obj: \
    E:\C_Projects\SoundDemo_Source\dsp.c \
    {E:\C_Projects\SoundDemo_Source;$(INCLUDE);}dsp.h

E:\C_Projects\SoundDemo_Source\sndos2.obj: \
    E:\C_Projects\SoundDemo_Source\sndos2.c \
    {E:\C_Projects\SoundDemo_Source;$(INCLUDE);}SndOS2.h \
    {E:\C_Projects\SoundDemo_Source;$(INCLUDE);}dsp.h
