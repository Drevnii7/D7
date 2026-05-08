#pragma once

#include "IPreprocessor.h"
#include <map>

class CStandartPreprocessor : public IPreprocessor
{
public: // CBaseService

	virtual void RunProcessing() override;

	virtual void Reset() override;

protected: // IPreprocessor

	// std::list<FToken> m_tokens;

public:

	int FindNextPreprocessorDirective();

	void DirectiveCollectUsing();
	void DirectiveApplyUsing();
protected:

	std::map<std::string, FToken> m_usings;
};

#if 0 // Standart DIRECTIVE and MACRO

	// preprocessor func
	// 600 - 699
	GROUP_PREPROCESSOR_FUNC = 600,
	M_USING   = 601,
	M_DEFINE  = 602,
	M_IFDEF   = 603,
	M_IFNDEF  = 604,
	M_IF      = 605,
	M_ELSE    = 606,
	M_ELIF    = 607,
	M_ENDIF   = 608,
	M_INCLUDE = 609,

	// preprocessor property
	// 700 - 799
	GROUP_PREPROCESSOR_PROPERTY = 700,

	MP_POS = 701, // (str) 120:0
	MP_LINE = 702, // (int) 120
	MP_COLUMN = 703, // (int) 0

	MP_FILE = 710, // (str) C:/Users/Drevnii7/main.cpp
	MP_FILE_PATH = 711, // (str) C:/Users/Drevnii7/
	MP_FILE_NAME = 712, // (str) main.cpp

	MP_DATE = 720, // (str) 08.05.2026
	MP_DATE_YEAR = 721, // (int) 2026
	MP_DATE_MONTH = 722, // (int) 5
	MP_DATE_DAY = 723, // (int) 8

	MP_TIME = 730, // (str) 16:08:02
	MP_TIME_HOUR = 731, // (int) 16
	MP_TIME_MINUTE = 732, // (int) 8
	MP_TIME_SECOND = 733, // (int) 26

	MP_FUNC = 740, // (tokens) void main(int argc, char *argv[])
	MP_FUNC_RETURN = 741, // (tokens) void
	MP_FUNC_NAME = 742, // (tokens) main
	MP_FUNC_ARGS = 743, // (tokens) int argc, char *argv[]
	MP_FUNC_ARGS_TYPES = 744, // (tokens) int, char *
	MP_FUNC_ARGS_NAMES = 745, // (tokens) argc, argv
	MP_FUNC_ARGS_COUNT = 746, // (int) 2
	MP_FUNC_DEFINE_LINE = 747, // (int) function declaration position
	MP_FUNC_LINE = 748, // (int) position relative to the function declaration

	MP_TOKEN_TYPE = 760, // (token type) get IDENTIFIER type (static reflection)
	MP_TOKEN_NAME = 761, // (str) get IDENTIFIER name (static reflection)
#endif