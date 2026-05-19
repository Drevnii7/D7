#include "CService.hpp"

#include "../utils/notify.hpp"

#include "../lexer/CLexer.hpp"
#include "../preprocessor/CPreprocessor.hpp"
#include "../parser/CParser.hpp"

#include "../utils/performance_timer.hpp"

#ifndef DISABLE_TEST
#include "../tests/Test_AllInOne.hpp"
#endif

using expected = d7::expected;

void d7::CService::SetConfig(const CServiceConfig& NewConfig)
{
	m_config = NewConfig;
}

void d7::CService::SetConfig(CServiceConfig&& NewConfig)
{
	m_config = std::move(NewConfig);
}

#define PARSE_ARG(arg_long, arg_short) \
	((arg == arg_long || arg == arg_short) && i + 1 < argc)

#define PARSE_ARG_1(arg_long, arg_short) \
	(arg == arg_long || arg == arg_short)

expected d7::CService::SetConfig(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		std::string arg = argv[i];

		if PARSE_ARG("--lexer-in", "-li")
		{
			m_config.FilePath_Lexer_In = argv[++i];
			m_config.MaskWork[0] = true;
		}
		else if PARSE_ARG("--lexer-out", "-lo")
		{
			m_config.FilePath_Lexer_Out = argv[++i];
			m_config.MaskWork[0] = true;
		}
		else if PARSE_ARG("--preprocessor-in", "-ppi")
		{
			m_config.FilePath_Preprocessor_In = argv[++i];
			m_config.MaskWork[1] = true;
		}
		else if PARSE_ARG("--preprocessor-out", "-ppo")
		{
			m_config.FilePath_Preprocessor_Out = argv[++i];
			m_config.MaskWork[1] = true;
		}
		else if PARSE_ARG("--parser-in", "-pi")
		{
			m_config.FilePath_Parser_In = argv[++i];
			m_config.MaskWork[2] = true;
		}
		else if PARSE_ARG("--parser-out", "-po")
		{
			m_config.FilePath_Parser_Out = argv[++i];
			m_config.MaskWork[2] = true;
		}
		else if PARSE_ARG("--notify-mask", "-nm")
		{
			std::string Mask = argv[++i];
			for (int li = 0; li < Mask.size() && li < m_config.MaskNotify.size(); li++)
			{
				m_config.MaskNotify[li] = (Mask[li] == '1');
			}
		}
		else if PARSE_ARG("--work-mask", "-wm")
		{
			std::string Mask = argv[++i];
			for (int li = 0; li < Mask.size() && li < m_config.MaskWork.size(); li++)
			{
				m_config.MaskWork[li] = (Mask[li] == '1');
			}
		}
		else if PARSE_ARG_1("--help", "-h")
		{
			notify_warning("\"--help\" or \"-h\" - show this text");
			notify_warning("\"--lexer-in\" or \"-li\" - set lexer input file");
			notify_warning("\"--lexer-out\" or \"-lo\" - set lexer output file");
			notify_warning("\"--preprocessor-in\" or \"-ppi\" - set preprocessor input file");
			notify_warning("\"--preprocessor-out\" or \"-ppo\" - set preprocessor output file");
			notify_warning("\"--parser-in\" or \"-pi\" - set parser input file");
			notify_warning("\"--parser-out\" or \"-po\" - set parser output file");
			notify_warning("\"--notify-mask\" or \"-nm\" - set mask: 0) Trace, 1) Callback, 2) Info, 3) Warning. Example: 0001 - only warning");
			notify_warning("\"--work-mask\" or \"-wm\" - set mask: 0) Lexer, 1) Preprocessor 3) Parser. Example: 010 - only preprocessor");
#ifndef DISABLE_TEST
			notify_warning("\"--run-test\" or \"-rt\" - run all tests");
#endif
			return expected::Fail();
		}
		else if PARSE_ARG_1("--run-test", "-rt")
		{
#ifndef DISABLE_TEST
			tests::Test_AllInOne();
#endif
			return expected::Fail();
		}

		
	}

	return expected::Success();
}

expected d7::CService::Run()
{
	d7::notify::MaskNotifyLevel = m_config.MaskNotify;
	notify_warning("https://github.com/Drevnii7/D7");

	d7::performance_timer CServiceRinTimer("CServiceRunTimer", true);
	d7::performance_timer CServiceRinTimerLexer("CServiceRunTimer_Lexer", true);
	d7::performance_timer CServiceRinTimerPreprocessor("CServiceRunTimerPreprocessor", true);
	d7::performance_timer CServiceRinTimerParser("CServiceRunTimer_Parser", true);

	CServiceRinTimer.Start();

	// Lexer
	d7::CLexer Lexer;
	if (m_config.MaskWork[0] == true)
	{
		if (expected Exp = Lexer.LoadCode(m_config.FilePath_Lexer_In); !Exp)
		{
			return expected::Fatal(("Lexer.LoadCode(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		}

		CServiceRinTimerLexer.Start();
		if (expected Exp = Lexer.Run(); !Exp)
		{
			return expected::Fatal(("Lexer.Run(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		}
		CServiceRinTimerLexer.Stop();

		if (!m_config.FilePath_Lexer_Out.empty())
		{
			if (expected Exp = Lexer.SaveTokens(m_config.FilePath_Lexer_Out); !Exp)
			{
				return expected::Fatal(("Keep temp: Lexer.SaveTokens(): " + Exp.ExtractFatalMessageOrFail()).c_str());
			}
		}
	}

	// Preprocessor
	d7::CPreprocessor Preprocessor;
	if (m_config.MaskWork[1] == true)
	{
		if (m_config.FilePath_Preprocessor_In.empty())
		{
			if (m_config.MaskWork[0] == true)
			{
				Preprocessor.SetTokens(Lexer.ExtractTokens());
			}
			else
			{
				return expected::Fatal("FilePath_Preprocessor_In empty and lexer not work");
			}
		}
		else
		{
			if (expected Exp = Preprocessor.LoadTokens(m_config.FilePath_Preprocessor_In); !Exp)
			{
				return expected::Fatal(("Preprocessor.LoadTokens(): " + Exp.ExtractFatalMessageOrFail()).c_str());
			}
		}

		CServiceRinTimerPreprocessor.Start();
		if (expected Exp = Preprocessor.Run(); !Exp)
		{
			return expected::Fatal(("Preprocessor.Run(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		}
		CServiceRinTimerPreprocessor.Stop();

		if (!m_config.FilePath_Preprocessor_Out.empty())
		{
			if (expected Exp = Preprocessor.SaveTokens(m_config.FilePath_Preprocessor_Out); !Exp)
			{
				return expected::Fatal(("Preprocessor.SaveTokens(): " + Exp.ExtractFatalMessageOrFail()).c_str());
			}
		}
	}


	// Parser
	d7::CParser Parser;
	if (m_config.MaskWork[1] == true)
	{
		if (m_config.FilePath_Parser_In.empty())
		{
			if (m_config.MaskWork[1] == true)
			{
				Parser.SetTokens(Preprocessor.ExtractTokens());
			}
			else
			{
				return expected::Fatal("FilePath_Parser_In empty and preprocessor not work");
			}
		}
		else
		{
			if (expected Exp = Parser.LoadTokens(m_config.FilePath_Parser_In); !Exp)
			{
				return expected::Fatal(("Parser.LoadTokens(): " + Exp.ExtractFatalMessageOrFail()).c_str());
			}
		}

		CServiceRinTimerParser.Start();
		if (expected Exp = Parser.Run(); !Exp)
		{
			return expected::Fatal(("Parser.Run(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		}
		CServiceRinTimerParser.Stop();

		if (!m_config.FilePath_Parser_Out.empty())
		{
			if (expected Exp = Parser.SaveAST(m_config.FilePath_Parser_Out); !Exp)
			{
				return expected::Fatal(("Parser.SaveAST(): " + Exp.ExtractFatalMessageOrFail()).c_str());
			}
		}
	}

	CServiceRinTimer.Stop();
	return expected::Success();
}
