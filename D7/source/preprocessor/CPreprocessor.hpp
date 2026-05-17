#pragma once

#include "IPreprocessor.hpp"
#include <functional>
#include <map>
#include <list>

namespace d7
{
    // See CPreprocessor.md
    class CPreprocessor : public IPreprocessor
    {
    public:
        CPreprocessor();

        d7::expected Run() override;

    private:

        using MacroHandler = std::function<std::list<FToken>(const FToken&, const FToken&, const FToken&)>;

        std::map<std::string, MacroHandler> m_MacroSubstitutions;
        
        bool IsContainsMacro();

        void Register_MacroSubstitutions();

        // MS - Macro Substitutions
        static std::list<FToken> MS_Pos(const FToken& Before, const FToken& Now, const FToken& After);
        static std::list<FToken> MS_Line(const FToken& Before, const FToken& Now, const FToken& After);
        static std::list<FToken> MS_Column(const FToken& Before, const FToken& Now, const FToken& After);
    
    
    
        std::map<std::string, std::list<FToken>> m_UserMacros;
        bool TryParseDefineDirective(std::list<FToken>::iterator& it);
        void RegisterDefineMacro(const std::string& name, std::list<FToken>&& tokens);

    };
}