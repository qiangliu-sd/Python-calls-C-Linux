
//#include <ostream>
#include "date/LunarDate.h"
#include <string_view>		// C++17
//using namespace std;

#if defined(__WSL_C__)	//  -D__WSL_C__
    #define EXPORT //  __attribute__((visibility("default")))
    //#define IMPORT
#else   //  default: Microsoft MSVC
    #define EXPORT __declspec(dllexport)
    //#define IMPORT __declspec(dllimport)
#endif

// Python: create_string_buffer(100) for fetching string from C via
//	 [yyyymmdd_o]
extern "C" EXPORT int lunarFetch(const char* sol_yyyymmdd, char* yyyymmdd_o);
extern "C" EXPORT int solarFetch(const char* lun_yyyymmdd, char* yyyymmdd_o);


static int string2CharStar(const string_view & str, char* str_copy) {
	auto _s_len = str.size();
	str.copy(str_copy, _s_len);
	//str_copy[_s_len] = '\0';		// C str terminated by null
	return _s_len; // _s_len + 1;
}


// client has to:
//		make sure input [sol_yyyymmdd] is a valid date as "yyyymmdd"
//		check whether "LEAP" is in [yyyymmdd_o]
int lunarFetch(const char* sol_yyyymmdd, char * yyyymmdd_o) {
	LunarDate _l_date(sol_yyyymmdd);
	string _ld_str = _l_date.showStr();
	if (_l_date.isLeapMon()) _ld_str += "LEAP";
	return string2CharStar(_ld_str, yyyymmdd_o);
}

// client has to:
//		make sure input [lun_yyyymmdd] is in "yyyymmdd" format
//		check whether returned date is "BAD_INPUT" (i.e., [lun_yyyymmdd] is BAD)
int solarFetch(const char* lun_yyyymmdd, char* yyyymmdd_o) {
	SolarDate _s_date(lun_yyyymmdd);
	return string2CharStar(_s_date.datesPowerS(), yyyymmdd_o);
}

