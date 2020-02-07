#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "07";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2020";
	static const char UBUNTU_VERSION_STYLE[] =  "20.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 1;
	static const long BUILD  = 131;
	static const long REVISION  = 682;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 275;
	#define RC_FILEVERSION 0,1,131,682
	#define RC_FILEVERSION_STRING "0, 1, 131, 682\0"
	static const char FULLVERSION_STRING [] = "0.1.131.682";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 29;
	

}
#endif //VERSION_H
