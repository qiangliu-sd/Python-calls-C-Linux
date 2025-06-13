# Call Linux shared-object C (foreign) functions directly via ctypes inside Python

With Python **ctypes**, Python can directly call (foreign) C functions defined in shared object (.so) or dynamic library on Linux. This note documents the compiling and linking of C functions in Ubuntu 24.04.2 LTS (GNU/Linux 6.6.87.1-microsoft-standard-WSL2 x86_64).

### Build dynamic lib (*.so) on Linux
Complex makefile (see a [mini version below](#my-actual-makemini-file)) are utilized to create a *.so file in two steps. To make it easy to undertsand, let's assume there are three .cpp (and three corresponding .h) files in the current dir:
- LunarDate.cpp   &nbsp;&nbsp;&nbsp;&nbsp;  // C++ classes implement convertions between lunar and solar dates
- DateHelper.cpp  &nbsp;&nbsp;&nbsp;&nbsp;  // C++ help functions used in LunarDate.cpp 
- solarLunar.cpp  &nbsp;&nbsp;&nbsp;&nbsp;  // C functions (using the LunarDate and SolarDate classes) to be exported to Python

1.	Compile object files one-by-one:
    - clang++ **-fPIC -fdeclspec** -I. -D__WSL_C__ -c LunarDate.cpp -o LunarDate.o
    - clang++ ...
    - clang++ ...
2.	Link the three object files into a shared object file:
    - clang++  **-shared**    DateHelper.o LunarDate.o  solarLunar.o   -o libSolarLunar.so

Notes:
- Compiler option [-fPIC] is necessary for shared lib
- Compiler option [-fdeclspec] is needed for calling C functions directly in Python
- The pre-processor [\_\_WSL\_C\_\_] takes care of Linux or Windows (see [solarLunar.cpp](solarLunar.cpp))
- Linker option [-shared] works with [-fPIC] together
- solarLunar.cpp is actually used in [convertions between Gregorian and Chinese lunar dates](../../../solar-lunar-date-tool)

### Python calls C functions
To call C functions in Python, you must pass the correct argument types (i.e., byte string, c_double, ctypes Structure, or ctypes array) and specify the return type. Check out examples directly in [c4Python.py](../../../Python-calls-C-dll/tree/main/c4PyTest) for what most of you need to know.

### My actual make.mini file
To create the libSolarLunar.so for Python, type:
> make -f make.mini dlib
```
DYNA_LIB = lib/libSolarLunar.so
OBJS = _obj_DEBUG/vcprjs/qlcdll/lunarPrj/solarLunar.o \
    _obj_DEBUG/utility/date/DateHelper.o \
    _obj_DEBUG/utility/date/LunarDate.o    

COMPILE.cxx = clang++ -I../utility  -c

# rule for making shared object
dlib:$(DYNA_LIB)

$(DYNA_LIB):$(OBJS)		
	-mkdir -p lib
	clang++ -shared  $(OBJS)     -o $@

# rule for making object from C++ sources
objs:$(OBJS)
	
_obj_DEBUG/%.o:../%.cpp
ifeq ($(dir $@), $(wildcard $(dir $@)))
	-mkdir -p $(dir $@)
endif
	$(COMPILE.cxx) -fPIC -fdeclspec -D__WSL_C__ $< -o $@
```
