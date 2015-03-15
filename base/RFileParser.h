#ifndef __RFileParser_H__
#define __RFileParser_H__
#include <stdio.h>
#include <string>
#include "RFileloader.h"
typedef std::string RWord;
#define RWORDS_COUNT 1
const RWord res_word[RWORDS_COUNT] = { "$include" };
class RFileParser
{
public:
	static uint stringLength(const char *a);
	static std::string genParsed(std::string fullname);
	static uint toSpace(const char *r);
	static uint getPreproc(RWord const c);
	static bool compare(RWord const a, RWord const b);
	static uint getFileNamePosition(std::string &fullname);
};
#endif
