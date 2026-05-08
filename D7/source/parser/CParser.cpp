#include "CParser.h"
#include "../Notify/Notify.h"
#include <cctype>

const FToken& CParser::consume(UTokenType type, const std::string& message)
{
    if (check(type))
    {
        return *advance();
    }
    throw std::runtime_error(message + " at " + GetErrorPos());
}

std::string CParser::GetErrorPos() const
{
    if (isAtEnd()) return "[EOF]";
    const auto* tok = peek();
    return std::to_string(tok->Line) + ":" + std::to_string(tok->Row);
}

bool CParser::isTypeKeyword(const FToken& tok) const
{
    if (tok.Type != UTokenType::IDENTIFIER) return false;
    const auto& lex = tok.Lexeme;
    return lex == "int" || lex == "float" || lex == "double" ||
        lex == "char" || lex == "bool" || lex == "void" ||
        lex == "auto" || lex == "const" || lex == "using";
}

bool CParser::isDeclarationStart() const
{
    return !isAtEnd() && check(UTokenType::IDENTIFIER) && isTypeKeyword(*peek());
}

void CParser::RunProcessing()
{
    if (m_tokens.empty())
    {
        Error("No tokens to parse");
        return;
    }

    m_rootNode = FASTNode();
    m_rootNode.Type = UNodeType::Block;
    m_current = 0;

    try
    {
        while (!isAtEnd())
        {
            auto decl = parseDeclaration();
            if (decl && decl->IsValid())
            {
                m_rootNode.AddChild(std::move(decl));
            }
            else if (!isAtEnd())
            {
                Warning("Skipped invalid token at " + GetErrorPos() + ": " + peek()->Lexeme);
                advance();
            }
        }
    }
    catch (const std::runtime_error& e)
    {
        Fatal(std::string("Parse error: ") + e.what());
    }
}

std::unique_ptr<FASTNode> CParser::parseProgram()
{
    auto program = std::make_unique<FASTNode>();
    program->Type = UNodeType::Block;

    while (!isAtEnd())
    {
        auto decl = parseDeclaration();
        if (decl) program->AddChild(std::move(decl));
        else break;
    }
    return program;
}

std::unique_ptr<FASTNode> CParser::parseDeclaration()
{
    if (isAtEnd()) return nullptr;
    if (!isDeclarationStart()) return parseStatement();

    auto typeTok = *advance();
    if (!check(UTokenType::IDENTIFIER)) throw std::runtime_error("Expected name after type at " + GetErrorPos());
    auto nameTok = *advance();

    if (check(UTokenType::LPAR)) return parseFuncDecl(typeTok, nameTok);
    return parseVarDecl(typeTok, nameTok);
}

std::unique_ptr<FASTNode> CParser::parseVarDecl(const FToken& typeTok, const FToken& nameTok)
{
    auto node = std::make_unique<FASTNode>();
    node->Type = UNodeType::VarDecl;
    node->Token = typeTok;

    auto nameNode = std::make_unique<FASTNode>();
    nameNode->Type = UNodeType::Identifier;
    nameNode->Token = nameTok;
    node->AddChild(std::move(nameNode));

    if (match(UTokenType::ASSIGN))
    {
        node->AddChild(parseExpression());
    }

    consume(UTokenType::SEMICOLON, "Expected ';' after variable declaration");
    return node;
}

std::unique_ptr<FASTNode> CParser::parseFuncDecl(const FToken& typeTok, const FToken& nameTok)
{
    auto node = std::make_unique<FASTNode>();
    node->Type = UNodeType::FunctionDecl;
    node->Token = typeTok;

    auto nameNode = std::make_unique<FASTNode>();
    nameNode->Type = UNodeType::Identifier;
    nameNode->Token = nameTok;
    node->AddChild(std::move(nameNode));

    consume(UTokenType::LPAR, "Expected '(' after function name");

    while (!check(UTokenType::RPAR) && !isAtEnd())
    {
        auto param = std::make_unique<FASTNode>();
        param->Type = UNodeType::Identifier;

        if (check(UTokenType::IDENTIFIER) && isTypeKeyword(*peek())) advance();
        if (!check(UTokenType::IDENTIFIER)) throw std::runtime_error("Expected parameter name at " + GetErrorPos());

        param->Token = *advance();
        node->AddChild(std::move(param));
        if (!match(UTokenType::COMMA)) break;
    }
    consume(UTokenType::RPAR, "Expected ')' after parameters");

    if (match(UTokenType::LBRA))
    {
        auto body = parseBlock();
        if (body) node->AddChild(std::move(body));
    }
    else
    {
        consume(UTokenType::SEMICOLON, "Expected '{' or ';' after function parameters");
    }
    return node;
}

std::unique_ptr<FASTNode> CParser::parseStatement()
{
    if (match(UTokenType::RETURN)) return parseReturnStmt();
    if (match(UTokenType::IF)) return parseIfStmt();
    if (match(UTokenType::WHILE)) return parseWhileStmt();
    if (match(UTokenType::FOR)) return parseForStmt();
    if (match(UTokenType::LBRA)) return parseBlock();
    if (match(UTokenType::SEMICOLON)) return std::make_unique<FASTNode>();

    auto expr = parseExpression();
    if (!expr) throw std::runtime_error("Expected statement at " + GetErrorPos());
    consume(UTokenType::SEMICOLON, "Expected ';' after statement");
    return expr;
}

std::unique_ptr<FASTNode> CParser::parseBlock()
{
    auto node = std::make_unique<FASTNode>();
    node->Type = UNodeType::Block;

    while (!check(UTokenType::RBRA) && !isAtEnd())
    {
        if (isDeclarationStart()) node->AddChild(parseDeclaration());
        else node->AddChild(parseStatement());
    }
    consume(UTokenType::RBRA, "Expected '}' after block");
    return node;
}

std::unique_ptr<FASTNode> CParser::parseReturnStmt()
{
    auto node = std::make_unique<FASTNode>();
    node->Type = UNodeType::ReturnStmt;
    node->Token = *previous();

    if (!check(UTokenType::SEMICOLON)) node->AddChild(parseExpression());
    consume(UTokenType::SEMICOLON, "Expected ';' after return");
    return node;
}

std::unique_ptr<FASTNode> CParser::parseIfStmt()
{
    auto node = std::make_unique<FASTNode>();
    node->Type = UNodeType::Block;

    consume(UTokenType::LPAR, "Expected '(' after 'if'");
    auto cond = parseExpression();
    consume(UTokenType::RPAR, "Expected ')' after condition");
    if (cond) node->AddChild(std::move(cond));

    auto thenBranch = parseStatement();
    if (thenBranch) node->AddChild(std::move(thenBranch));

    if (match(UTokenType::ELSE))
    {
        auto elseBranch = parseStatement();
        if (elseBranch) node->AddChild(std::move(elseBranch));
    }
    return node;
}

std::unique_ptr<FASTNode> CParser::parseWhileStmt()
{
    auto node = std::make_unique<FASTNode>();
    node->Type = UNodeType::Block;

    consume(UTokenType::LPAR, "Expected '(' after 'while'");
    auto cond = parseExpression();
    consume(UTokenType::RPAR, "Expected ')' after condition");
    if (cond) node->AddChild(std::move(cond));

    auto body = parseStatement();
    if (body) node->AddChild(std::move(body));
    return node;
}

std::unique_ptr<FASTNode> CParser::parseForStmt()
{
    auto node = std::make_unique<FASTNode>();
    node->Type = UNodeType::Block;

    consume(UTokenType::LPAR, "Expected '(' after 'for'");

    if (!match(UTokenType::SEMICOLON))
    {
        if (isDeclarationStart()) node->AddChild(parseDeclaration());
        else
        {
            auto init = parseExpression();
            if (init) node->AddChild(std::move(init));
            consume(UTokenType::SEMICOLON, "Expected ';' after for initializer");
        }
    }

    if (!check(UTokenType::SEMICOLON))
    {
        auto cond = parseExpression();
        if (cond) node->AddChild(std::move(cond));
    }
    consume(UTokenType::SEMICOLON, "Expected ';' after for condition");

    if (!check(UTokenType::RPAR))
    {
        auto inc = parseExpression();
        if (inc) node->AddChild(std::move(inc));
    }
    consume(UTokenType::RPAR, "Expected ')' after for clauses");

    auto body = parseStatement();
    if (body) node->AddChild(std::move(body));
    return node;
}

std::unique_ptr<FASTNode> CParser::parseExpression() { return parseAssignment(); }

std::unique_ptr<FASTNode> CParser::parseAssignment()
{
    auto expr = parseLogicalOr();

    if (match(UTokenType::ASSIGN) || match(UTokenType::ASSIGN_ADD) || match(UTokenType::ASSIGN_SUB) ||
        match(UTokenType::ASSIGN_MUL) || match(UTokenType::ASSIGN_DIV) || match(UTokenType::ASSIGN_MOD))
    {
        auto op = *previous();
        auto right = parseAssignment();
        auto binary = std::make_unique<FASTNode>();
        binary->Type = UNodeType::BinaryExpr;
        binary->Token = op;
        binary->AddChild(std::move(expr));
        binary->AddChild(std::move(right));
        return binary;
    }
    return expr;
}

std::unique_ptr<FASTNode> CParser::parseLogicalOr()
{
    auto expr = parseLogicalAnd();
    while (match(UTokenType::OR))
    {
        auto op = *previous();
        auto right = parseLogicalAnd();
        auto binary = std::make_unique<FASTNode>();
        binary->Type = UNodeType::BinaryExpr;
        binary->Token = op;
        binary->AddChild(std::move(expr));
        binary->AddChild(std::move(right));
        expr = std::move(binary);
    }
    return expr;
}

std::unique_ptr<FASTNode> CParser::parseLogicalAnd()
{
    auto expr = parseEquality();
    while (match(UTokenType::AND))
    {
        auto op = *previous();
        auto right = parseEquality();
        auto binary = std::make_unique<FASTNode>();
        binary->Type = UNodeType::BinaryExpr;
        binary->Token = op;
        binary->AddChild(std::move(expr));
        binary->AddChild(std::move(right));
        expr = std::move(binary);
    }
    return expr;
}

std::unique_ptr<FASTNode> CParser::parseEquality()
{
    auto expr = parseComparison();
    while (match(UTokenType::EQUAL) || match(UTokenType::NOT_EQUAL))
    {
        auto op = *previous();
        auto right = parseComparison();
        auto binary = std::make_unique<FASTNode>();
        binary->Type = UNodeType::BinaryExpr;
        binary->Token = op;
        binary->AddChild(std::move(expr));
        binary->AddChild(std::move(right));
        expr = std::move(binary);
    }
    return expr;
}

std::unique_ptr<FASTNode> CParser::parseComparison()
{
    auto expr = parseTerm();
    while (match(UTokenType::LESS) || match(UTokenType::GREATER) || match(UTokenType::LESS_EQUAL) || match(UTokenType::GREATER_EQUAL))
    {
        auto op = *previous();
        auto right = parseTerm();
        auto binary = std::make_unique<FASTNode>();
        binary->Type = UNodeType::BinaryExpr;
        binary->Token = op;
        binary->AddChild(std::move(expr));
        binary->AddChild(std::move(right));
        expr = std::move(binary);
    }
    return expr;
}

std::unique_ptr<FASTNode> CParser::parseTerm()
{
    auto expr = parseFactor();
    while (match(UTokenType::ADD) || match(UTokenType::SUB))
    {
        auto op = *previous();
        auto right = parseFactor();
        auto binary = std::make_unique<FASTNode>();
        binary->Type = UNodeType::BinaryExpr;
        binary->Token = op;
        binary->AddChild(std::move(expr));
        binary->AddChild(std::move(right));
        expr = std::move(binary);
    }
    return expr;
}

std::unique_ptr<FASTNode> CParser::parseFactor()
{
    auto expr = parseUnary();
    while (match(UTokenType::MUL) || match(UTokenType::DIV) || match(UTokenType::MOD))
    {
        auto op = *previous();
        auto right = parseUnary();
        auto binary = std::make_unique<FASTNode>();
        binary->Type = UNodeType::BinaryExpr;
        binary->Token = op;
        binary->AddChild(std::move(expr));
        binary->AddChild(std::move(right));
        expr = std::move(binary);
    }
    return expr;
}

std::unique_ptr<FASTNode> CParser::parseUnary()
{
    if (match(UTokenType::NOT) || match(UTokenType::SUB) || match(UTokenType::ADD) || match(UTokenType::INC) || match(UTokenType::DEC))
    {
        auto op = *previous();
        auto right = parseUnary();
        auto unary = std::make_unique<FASTNode>();
        unary->Type = UNodeType::UnaryExpr;
        unary->Token = op;
        unary->AddChild(std::move(right));
        return unary;
    }
    auto expr = parsePrimary();
    if (match(UTokenType::INC) || match(UTokenType::DEC))
    {
        auto op = *previous();
        auto unary = std::make_unique<FASTNode>();
        unary->Type = UNodeType::UnaryExpr;
        unary->Token = op;
        unary->AddChild(std::move(expr));
        return unary;
    }
    return expr;
}

std::unique_ptr<FASTNode> CParser::parsePrimary()
{
    if (match(UTokenType::INT) || match(UTokenType::FLOAT) || match(UTokenType::STRING) ||
        match(UTokenType::CHAR) || match(UTokenType::BOOLEAN))
    {
        auto node = std::make_unique<FASTNode>();
        node->Type = UNodeType::Literal;
        node->Token = *previous();
        return node;
    }

    if (check(UTokenType::IDENTIFIER))
    {
        auto& tok = *peek();
        bool isLit = (tok.Lexeme == "true" || tok.Lexeme == "false" ||
            (!tok.Lexeme.empty() && std::isdigit(static_cast<unsigned char>(tok.Lexeme[0]))));
        advance();
        auto node = std::make_unique<FASTNode>();
        node->Type = isLit ? UNodeType::Literal : UNodeType::Identifier;
        node->Token = tok;
        return node;
    }

    if (match(UTokenType::LPAR))
    {
        auto expr = parseExpression();
        consume(UTokenType::RPAR, "Expected ')' after expression");
        return expr;
    }

    throw std::runtime_error("Expected expression at " + GetErrorPos() + ", got: " + (isAtEnd() ? "[EOF]" : peek()->Lexeme));
}

bool CParser::RunFullCycle()
{
    if (!LoadTokens()) return false;
    RunProcessing();
    if (!SaveAST()) return false;
    return true;
}

void CParser::DebugPrint() const
{
    Warning("DebugPrint()");
    std::cout << m_rootNode.Dump() << '\n';
}

void CParser::Reset()
{
    m_inputFilePath.clear();
    m_outputFilePath.clear();
    m_tokens.clear();
    m_rootNode = FASTNode();
    m_current = 0;
}

bool CParser::LoadTokens(const std::string& filePath)
{
    if (!filePath.empty()) m_inputFilePath = filePath;
    try
    {
        m_tokens = FToken::Deserialize<std::vector<FToken>>(m_inputFilePath);
        Success("LoadTokens: " + m_inputFilePath);
    }
    catch (const std::runtime_error& error)
    {
        Error("LoadTokens: " + std::string(error.what()));
        return false;
    }
    return true;
}

bool CParser::SaveTokens(const std::string& filePath)
{
    if (!filePath.empty()) m_outputFilePath = filePath;
    try
    {
        FToken::Serialize<std::vector<FToken>>(m_tokens, m_outputFilePath);
        Success("SaveTokens: " + m_outputFilePath);
    }
    catch (const std::runtime_error& error)
    {
        Error("SaveTokens: " + std::string(error.what()));
        return false;
    }
    return true;
}

bool CParser::SaveAST(const std::string& filePath)
{
    if (!filePath.empty()) m_outputFilePath = filePath;
    try
    {
        FASTNode::Serialize(&m_rootNode, m_outputFilePath);
        Success("SaveAST: " + m_outputFilePath);
    }
    catch (const std::runtime_error& error)
    {
        Error("SaveAST: " + std::string(error.what()));
        return false;
    }
    return true;
}

void CParser::SetTokens(const std::vector<FToken>& tokens)
{
    Reset();
    m_inputFilePath = "CParser::SetTokens()";
    m_tokens = tokens;
}

void CParser::SetTokens(const std::list<FToken>& tokens)
{
    Reset();
    m_inputFilePath = "CParser::SetTokens()";
    m_tokens.assign(tokens.begin(), tokens.end());
}

void CParser::SetTokens(std::vector<FToken>&& tokens)
{
    Reset();
    m_inputFilePath = "CParser::SetTokens()";
    m_tokens = std::move(tokens);
}

void CParser::Fatal(const std::string& message) const { FatalBase(UNotifyFrom::PARSER, message); }
void CParser::Error(const std::string& message) const { ErrorBase(UNotifyFrom::PARSER, message); }
void CParser::Warning(const std::string& message) const { WarningBase(UNotifyFrom::PARSER, message); }
void CParser::Success(const std::string& message) const { SuccessBase(UNotifyFrom::PARSER, message); }