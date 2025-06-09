
//#include <ostream>
  
#include "date/LunarDate.h"

//using namespace std;

#if defined(__WSL_C__)	//  -D__WSL_C__
    #define EXPORT //  __attribute__((visibility("default")))
    //#define IMPORT
#else   //  default: Microsoft MSVC
    #define EXPORT __declspec(dllexport)
    //#define IMPORT __declspec(dllimport)
#endif

extern "C" EXPORT int lunarFetch(const char* sun_date, char* date_out);
extern "C" EXPORT int solarFetch(const char* lun_date, char* date_out);

// Python: create_string_buffer(100) for fetching strings from C
int string2CharStar(const string & str, char* str_copy) {
	int _s_len = str.size();
	str.copy(str_copy, _s_len);
	//str_copy[_s_len] = '\0';		// C str terminated by null
	//cout << str_copy << endl;
	return _s_len; // _s_len + 1;
}

int lunarFetch(const char* sun_date, char * date_out) {
	LunarDate _l_date(sun_date);
	string _dt_str=(_l_date.showStr());
	if (_l_date.isLeapMon()) _dt_str += "LEAP";
	//cout << "_dt_str="<< _dt_str << endl;
	return string2CharStar(_dt_str, date_out);
}

int solarFetch(const char* lun_date, char* date_out) {
	SolarDate _s_date(lun_date);
	//cout << _s_date.datesPowerS() << endl;
	return string2CharStar(_s_date.datesPowerS(), date_out);
}
