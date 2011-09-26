libpath = /LIBPATH:"C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib\x64" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\lib\amd64"
cl = "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\amd64\cl.exe"
link = "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\bin\amd64\link.exe"
nasm = "C:\temp\nasm\nasm.exe"
clparam = /nologo /EHsc /c /D "_CONSOLE" /D "WIN32" /D "NDEBUG" /D "UNICODE" /D "_UNICODE" /Zc:wchar_t /Zc:forScope /Gd /GS /Gy /GL /W3 /O2 /Oi
linkparam = /LTCG /RELEASE /SUBSYSTEM:CONSOLE /NOLOGO /MACHINE:X64 /DYNAMICBASE /NXCOMPAT /OPT:REF /OPT:ICF /ALLOWISOLATION /INCREMENTAL:NO $(libpath)
objs = dump512.obj
src = start.asm
target = start.bin
dump512 = dump512.exe

all: $(target) $(dump512)

clean:
	del $(objs) $(target) $(dump512)

dump512.obj: dump512.cpp
	$(cl) $(clparam) /Fo"$@" $**

$(dump512): $(objs)
	$(link) $(linkparam) /OUT:$(dump512) kernel32.lib $**

$(target):
	$(nasm) -o $@ bootloader\start.asm

