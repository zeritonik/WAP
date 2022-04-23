#pragma once

#include <Windows.h>
#include <string>


class EasyLoger
{
private:
	static WORD get_default_attribute();
	virtual void ColorOutput(const std::string& text, const WORD attribute) const;
	virtual void ColorOutput(const long long num, const WORD attribute) const;

protected:
	const HANDLE output_handle;
	const WORD default_attribute;

public:
	EasyLoger();

	inline void info(const std::string& text) const { ColorOutput(text, default_attribute); };
	inline void info(const long long num) const { ColorOutput(num, default_attribute); };

	inline void special_info(const std::string& text) const { ColorOutput(text, FOREGROUND_GREEN | FOREGROUND_INTENSITY); };
	inline void special_info(const long long num) const { ColorOutput(num, FOREGROUND_GREEN | FOREGROUND_INTENSITY); };

	inline void warn(const std::string& text) const { ColorOutput(text, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); };
	inline void warn(const long long num) const { ColorOutput(num, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); };

	inline void error(const std::string& text) const { ColorOutput(text, FOREGROUND_RED | FOREGROUND_INTENSITY); };
	inline void error(const long long num) const { ColorOutput(num, FOREGROUND_RED | FOREGROUND_INTENSITY); };
};


class NamedEasyLoger : public EasyLoger
{
private:
	const std::string name;

	void ColorOutput(const std::string& text, const WORD attribute) const;
	virtual void ColorOutput(const long long num, const WORD attribute) const;
public:
	NamedEasyLoger(const std::string& loger_name);
};


class TimedEasyLoger : public EasyLoger
{
private:
	void ColorOutput(const std::string& text, const WORD attribute) const;
	virtual void ColorOutput(const long long num, const WORD attribute) const;
public:
	TimedEasyLoger();
};


class NamedTimedEasyLoger : public EasyLoger
{
private:
	const std::string name;

	void ColorOutput(const std::string& text, const WORD attribute) const;
	virtual void ColorOutput(const long long num, const WORD attribute) const;
public:
	NamedTimedEasyLoger(const std::string& loger_name);
};