#pragma once

#include "../libraries/EasyLoger/EasyLoger.h"

#if defined(WAP_API) // FOR WAP DEBUG
	#if defined(_DEBUG)
	extern const NamedTimedEasyLoger WAP_LOGER;
	#define WAP_INFO(text) WAP_LOGER.info(text)
	#define WAP_SPECIALINFO(text) WAP_LOGER.special_info(text)
	#define WAP_WARN(text) WAP_LOGER.warn(text)
	#define WAP_ERROR(text) WAP_LOGER.error(text)
	#else	
	#define WAP_INFO(text)
	#define WAP_SPECIALINFO(text)
	#define WAP_WARN(text)
	#define WAP_ERROR(text)
	#endif
#else // FOR USER DEBUG
	#if defined(_DEBUG)
	extern const NamedTimedEasyLoger WAA_LOGER;
	#define WAA_INFO(text) WAA_LOGER.info(text)
	#define WAA_SPECIALINFO(text) WAA_LOGER.special_info(text)
	#define WAA_WARN(text) WAA_LOGER.warn(text)
	#define WAA_ERROR(text) WAA_LOGER.error(text)
	#else
	#define WAA_INFO(text)
	#define WAA_SPECIALINFO(text)
	#define WAA_WARN(text)
	#define WAA_ERROR(text)
	#endif
#endif