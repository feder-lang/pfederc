#ifndef PFEDERC_SYNTAX_SYNTAX
#define PFEDERC_SYNTAX_SYNTAX

#include "pfederc/core.hpp"
#include "pfederc/errors.hpp"
#include "pfederc/lexer.hpp"
#include "pfederc/expr.hpp"

namespace pfederc {
  class Parser;
  class SyntaxError;

  enum SyntaxErrorCode {
    STX_ERR_EXPECTED_PRIMARY_EXPR,
    STX_ERR_EXPECTED_CLOSING_BRACKET,
    STX_ERR_EXPECTED_ARR_CLOSING_BRACKET,
    STX_ERR_EXPECTED_TEMPL_CLOSING_BRACKET,
    STX_ERR_EXPECTED_FUNCTION_ID,
    STX_ERR_EXPECTED_PARAMETERS,
    STX_ERR_INVALID_VARDECL_ID,
    STX_ERR_EXPECTED_ID,
    STX_ERR_EXPECTED_VARDECL,
    STX_ERR_EXPECTED_GUARD,
    STX_ERR_EXPECTED_FN_DCL_DEF,
    STX_ERR_EXPECTED_STMT,
    STX_ERR_FUNC_VAR_NO_TEMPL,
    STX_ERR_EXPECTED_EOL,
    STX_ERR_EXPECTED_EOF,
    STX_ERR_EXPECTED_EOF_EOL,
    STX_ERR_INVALID_EXPR,
    STX_ERR_EXPECTED_EXPR,
    STX_ERR_PROGNAME,
    STX_ERR_CLASS_SCOPE,
    STX_ERR_EXPECTED_CONSTRUCTION,
  };

  typedef std::tuple<const Token * /*progName*/,
               Exprs /*&&imports*/,
               Exprs /*&&defs*/,
               bool /*error*/> ModBody;

  class Parser final {
    Lexer &lexer;
    std::vector<std::unique_ptr<SyntaxError>> errors;
    std::map<Expr*, std::string> descriptions;

    std::unique_ptr<ErrorExpr> generateError(
      std::unique_ptr<SyntaxError> &&syntaxError) noexcept;

    std::unique_ptr<Expr> parseUnary() noexcept;
    /*!\return Returns nullptr if the current token isn't a valid
     * primary expression (beginning), otherwise an expression is returned.
     */
    std::unique_ptr<Expr> parsePrimary() noexcept;

    /*!\return Returns nullptr if the current token isn't a valid
     * binary expression (beginning), otherwise an expression is returned.
     */
    std::unique_ptr<Expr> parseBinary(std::unique_ptr<Expr> lhs,
        Precedence minPrecedence) noexcept;

    /*!\brief Eats current token. If current token doesn't have
     * the token type 'type' then program panics.
     * \param type
     */
    void sanityExpect(TokenType type) noexcept;

    /*!\brief Eats current token.
     * \param type
     * \return If current token doesn't have the token type 'type' then false
     * is returned, otherwise true.
     */
    bool expect(TokenType type) noexcept;

    std::unique_ptr<Expr> parseArray() noexcept;
    std::unique_ptr<Expr> parseBrackets() noexcept;
    std::unique_ptr<FuncParameter> fromExprDeclToFunctionParam(
      std::unique_ptr<Expr> &&expr, std::unique_ptr<Expr> &&guard,
      std::unique_ptr<Expr> &&guardResult) noexcept;
    std::unique_ptr<FuncParameter> fromExprGuardToFunctionParam(
      std::unique_ptr<Expr> &&expr, std::unique_ptr<Expr> &&guardResult) noexcept;
    std::vector<std::unique_ptr<FuncParameter>> parseFuncParameters() noexcept;
    std::unique_ptr<Expr> parseFuncType() noexcept;
    std::unique_ptr<FuncParameter> fromExprToFunctionParam(
      std::unique_ptr<Expr> &&expr) noexcept;
    std::unique_ptr<Expr> parseFuncReturnType() noexcept;
    std::unique_ptr<Expr> parseFunction() noexcept;
    std::unique_ptr<Expr> parseLambda() noexcept;
    std::unique_ptr<Expr> parseModule() noexcept;
    std::unique_ptr<Expr> parseClass() noexcept;
    std::unique_ptr<Expr> parseEnum() noexcept;
    std::unique_ptr<Expr> parseTrait() noexcept;
    std::unique_ptr<Expr> parseType() noexcept;
    std::unique_ptr<Expr> parseUse() noexcept;
    std::unique_ptr<Expr> parseFor(bool isdo = false) noexcept;
    std::unique_ptr<Expr> parseMatch() noexcept;
    std::unique_ptr<Expr> parseIf(bool isensure = false) noexcept;
    std::unique_ptr<Expr> parseContinue() noexcept;
    std::unique_ptr<Expr> parseBreak() noexcept;
    std::unique_ptr<Expr> parseSafe() noexcept;
    std::unique_ptr<Expr> parseTemplate() noexcept;
    std::unique_ptr<BodyExpr> parseFunctionBody() noexcept;
    ModBody parseModBody(bool isprog = false) noexcept;
    std::unique_ptr<TemplateDecl> fromDeclExprToTemplateDecl(BiOpExpr &expr) noexcept;
    std::unique_ptr<TemplateDecls> parseTemplateDecl() noexcept;
  public:
    inline Parser(Lexer &lexer) noexcept
      : lexer{lexer}, errors(), descriptions() {}
    Parser(const Parser &) = delete;
    ~Parser();

    inline const Lexer &getLexer() const noexcept { return lexer; }
    inline const auto &getErrors() const noexcept { return errors; }
    inline const auto &getDescriptions() const noexcept
    { return descriptions; }

    std::unique_ptr<ProgramExpr> parseProgram() noexcept;

    std::unique_ptr<Expr> parseExpression(Precedence prec = 0) noexcept;
  };

  extern const std::map<TokenType /* opening bracket */,
    SyntaxErrorCode> STX_ERR_BRACKETS;

  class SyntaxError final {
    Level logLevel;
    SyntaxErrorCode err;
    Position pos;
    std::vector<Position> extraPos;
  public:
    inline SyntaxError(Level logLevel, SyntaxErrorCode err, Position pos,
      std::vector<Position> &&extraPos = {})
      : logLevel{logLevel}, err{err}, pos(pos),
        extraPos(std::move(extraPos)) {
    }

    SyntaxError(const SyntaxError &) = delete;
    ~SyntaxError();

    inline Level getLogLevel() const noexcept { return logLevel; }
    inline SyntaxErrorCode getErrorCode() const noexcept { return err; }
    inline const Position &getPosition() const noexcept { return pos; }
  };

  /*!\return Returns true if an error occured while parsing
   * otherwise false.
   */
  bool logParserErrors(Logger &log, const Parser &parser) noexcept;
}
#endif /* PFEDERC_SYNTAX_SYNTAX */
