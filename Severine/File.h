//stl
#include <string>

#pragma once
class RecordingTime
{
public:
	// TODO getters
	unsigned short hour{ 0 };
	unsigned short minute{ 0 };
	unsigned short second{ 0 };
	unsigned short milisecond{ 0 };
	bool init{ false };
public:
	RecordingTime() = default;
	RecordingTime(unsigned short h, 
		unsigned short m,
		unsigned short s,
		unsigned short ms,
		bool in) :
		hour(h),
		minute(m), 
		second(s), 
		milisecond(ms),
		init(in) {};
	
};

class File
{
private:
	RecordingTime mTime;
	std::string mPath;
public:
	File() = delete;
	File(std::string path) : mPath(path) {};
	void setTime(RecordingTime time) { mTime = time; };
	std::string getPath() { return mPath; };
	RecordingTime getTime() { return mTime; }
	~File() = default;
};

