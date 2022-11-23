#ifndef __WD_MYLOGGER_H__
#define __WD_MYLOGGER_H__

#include <log4cpp/Category.hh>

#include <string>
using std::string;

namespace search_engine
{

class Mylogger
{
public:
	enum Priority
    {
		ERROR = 300,
		WARN,
		INFO,
		DEBUG
	};

	static Mylogger *getInstance();

	static void destroy();

	template <class... Args>
	void warn(const char * msg, Args... args)
	{
		_mycat.warn(msg, args...);
	}

    //模板的声明与实现要写到一起，不能分开
	template <class... Args>
	void error(const char * msg, Args... args)
	{
		_mycat.error(msg, args...);
	}

	template <class... Args>
	void info(const char * msg, Args... args)
	{
		_mycat.info(msg, args...);
	}

	template <class... Args>
	void debug(const char * msg, Args... args)
	{
		_mycat.debug(msg, args...);
	}

	void warn(const char * msg);
	void error(const char * msg);
	void info(const char * msg);
	void debug(const char * msg);

	void setPriority(Priority p);
private:
	Mylogger();
	~Mylogger();
private:
	log4cpp::Category  &_mycat;
	static Mylogger * _pInstance;
};

#define prefix(msg) string("[")\
	.append(__FILE__).append(":")\
	.append(__FUNCTION__).append(":")\
	.append(std::to_string(__LINE__)).append("] ")\
	.append(msg).c_str()

//##__VA_ARGS__ 宏前面加上##的作用在于，当可变参数的个数为0时，
//这里的##起到把前面多余的","去掉的作用,否则会编译出错
#define LogError(msg,...) Mylogger::getInstance()->error(prefix(msg), ##__VA_ARGS__)
#define LogWarn(msg, ...) Mylogger::getInstance()->warn(prefix(msg), ##__VA_ARGS__)
#define LogInfo(msg, ...) Mylogger::getInstance()->info(prefix(msg), ##__VA_ARGS__)
#define LogDebug(msg, ...) Mylogger::getInstance()->debug(prefix(msg), ##__VA_ARGS__)

}//end of namespace wd

#endif
