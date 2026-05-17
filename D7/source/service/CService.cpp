#include "CService.hpp"

#include "../utils/notify.hpp"

#include "../lexer/CLexer.hpp"
#include "../preprocessor/CPreprocessor.hpp"

#include "../tests/Test_AllInOne.hpp"

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
		else if PARSE_ARG("--preprocessor-in", "-pi")
		{
			m_config.FilePath_Preprocessor_In = argv[++i];
			m_config.MaskWork[1] = true;
		}
		else if PARSE_ARG("--preprocessor-out", "-po")
		{
			m_config.FilePath_Preprocessor_Out = argv[++i];
			m_config.MaskWork[1] = true;
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
		/*else if PARSE_ARG("--keep-temp", "-kt")
		{
			m_config.KeepTempFiles = (argv[++i] == "true");
		}*/
		else if PARSE_ARG_1("--help", "-h")
		{
			notify_warning("\"--help\" or \"-h\" - show this text");
			notify_warning("\"--lexer-in\" or \"-li\" - set lexer input file");
			notify_warning("\"--lexer-out\" or \"-lo\" - set lexer output file");
			notify_warning("\"--preprocessor-in\" or \"-pi\" - set preprocessor input file");
			notify_warning("\"--preprocessor-out\" or \"-po\" - set preprocessor output file");
			notify_warning("\"--notify-mask\" or \"-nm\" - set mask: 0) Trace, 1) Callback, 2) Info, 3) Warning. Example: 0001 - only warning");
			notify_warning("\"--work-mask\" or \"-wm\" - set mask: 0) Lexer, 1) Preprocessor. Example: 01 - only preprocessor");
			notify_warning("\"--help-lexer\" or \"-hl\" - show help for lexer");
			notify_warning("\"--help-preprocessor\" or \"-hp\" - show help for preprocessor");
			notify_warning("\"--run-test\" or \"-rt\" - run all tests");
			
			//notify_warning("\"--keep-temp\" or \"-kt\" - keep temp files after compleated");

			return expected::Fail();
		}
		else if PARSE_ARG_1("--help-lexer", "-hl")
		{
			notify_warning("Input source code (txt) and return tokens (bin)");
			notify_warning("D7 - A lightweight programming language experiment featuring a custom compiler toolchain.");

			notify_warning("Define: struct, enum, car, func, constructor, destructor, cast");
			notify_warning("Var agreements: in, constructor, optional, out");
			notify_warning("Func agreements: check_return, must_check_return, const, unsafe_use_pointer");
			
			notify_warning("Code example");

			notify_warning("func void Main()");
			notify_warning("{");
			notify_warning("var Pointer<FVector2d> Vector = nullptr;");
			notify_warning("Vector = new FVector2d(123, 789);");
			notify_warning("PrintFVector2d(Vector.UnsafeGet());");
			notify_warning("var ENumbers Number = ENumbers::Second; ");
			notify_warning("if(ENumbers::IsValid(Number))");
			notify_warning("{");
			notify_warning("std::io::printf(\"Enum value{ int }, name{ string }\", Number, ENumbers::ToString(Number));");
			notify_warning("}");
			notify_warning("else");
			notify_warning("{");
			notify_warning("std::io::printf(\"Not valid enum value { int }\", Number);");
			notify_warning("}");
			notify_warning("}");

			return expected::Fail();
		}
		else if PARSE_ARG_1("--help-preprocessor", "-hp")
		{
			notify_warning("Input tokens (bin) and return tokens (bin)");

			notify_warning("Syntax");
			notify_warning("$macro$ - open macro with his standart type");
			notify_warning("@macro@ - open macro as string");
			notify_warning("Support macro");
			notify_warning("POS, LINE, COLUMN");
			notify_warning("Define custom macro: #define macro_name macro_value");

			return expected::Fail();
		}
		else if PARSE_ARG_1("--run-test", "-rt")
		{
			tests::Test_AllInOne();

			return expected::Fail();
		}
	}

	return expected::Success();
}

expected d7::CService::Run()
{
	d7::notify::MaskNotifyLevel = m_config.MaskNotify;
	notify_warning("https://github.com/Drevnii7/D7");

	// Lexer
	d7::CLexer Lexer;
	if (m_config.MaskWork[0] == true)
	{
		if (expected Exp = Lexer.LoadCode(m_config.FilePath_Lexer_In); !Exp)
		{
			return expected::Fatal(("Lexer.LoadCode(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		}

		if (expected Exp = Lexer.Run(); !Exp)
		{
			return expected::Fatal(("Lexer.Run(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		}

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
		
		if (expected Exp = Preprocessor.Run(); !Exp)
		{
			return expected::Fatal(("Preprocessor.Run(): " + Exp.ExtractFatalMessageOrFail()).c_str());
		}

		if (!m_config.FilePath_Preprocessor_Out.empty())
		{
			if (expected Exp = Preprocessor.SaveTokens(m_config.FilePath_Preprocessor_Out); !Exp)
			{
				return expected::Fatal(("Preprocessor.SaveTokens(): " + Exp.ExtractFatalMessageOrFail()).c_str());
			}
		}
	}

	return expected::Success();
}
