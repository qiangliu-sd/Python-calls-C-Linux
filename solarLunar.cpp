
//#include <ostream>
#include "date/LunarDate.h"
#include <string_view>
//using namespace std;

#if defined(__WSL_C__)	//  -D__WSL_C__
    #define EXPORT //  __attribute__((visibility("default")))
    //#define IMPORT
#else   //  default: Microsoft MSVC
    #define EXPORT __declspec(dllexport)
    //#define IMPORT __declspec(dllimport)
#endif

// Python: create_string_buffer(100) for fetching string from C via [date_out]
extern "C" EXPORT int lunarFetch(const char* sun_date, char* date_out);
extern "C" EXPORT int solarFetch(const char* lun_date, char* date_out);


static int string2CharStar(const string_view & str, char* str_copy) {
	auto _s_len = str.size();
	str.copy(str_copy, _s_len);
	//str_copy[_s_len] = '\0';		// C str terminated by null
	return _s_len; // _s_len + 1;
}


// client has to 
//		make sure input [sun_date] is a valid date as "yyyymmdd"
//		check whether: "LEAP" is in [date_out]
int lunarFetch(const char* sun_date, char * date_out) {
	LunarDate _l_date(sun_date);
	string _dt_str=(_l_date.showStr());
	if (_l_date.isLeapMon()) _dt_str += "LEAP";
	return string2CharStar(_dt_str, date_out);
}

// client has to 
//		make sure input [lun_date] is in "yyyymmdd" format
//		check whether returned date is "19000101" (i.e., input [lun_date] is BAD)
int solarFetch(const char* lun_date, char* date_out) {
	SolarDate _s_date(lun_date);
	return string2CharStar(_s_date.datesPowerS(), date_out);
}

