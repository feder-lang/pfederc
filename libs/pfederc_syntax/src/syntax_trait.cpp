#include "pfederc/syntax.hpp"
using namespace pfederc;

std::unique_ptr<Expr> Parser::parseTrait(std::unique_ptr<Capabilities> &&caps) noexcept {
  sanityExpect(TokenType::TOK_KW_TRAIT);

  bool err = false; // hard errors

  TemplateDecls templ;
  if (*lexer.getCurrentToken() == TokenType::TOK_OP_TEMPL_BRACKET_OPEN) {
    templ = parseTemplateDecl();
    // maybe soft error
  }

  const Token *const tokId = lexer.getCurrentToken();
  if (!expect(TokenType::TOK_ID)) {
    generateError(std::make_unique<SyntaxError>(LVL_ERROR,
      SyntaxErrorCode::STX_ERR_EXPECTED_ID, tokId->getPosition()));
    err = true;
  }

  std::vector<std::unique_ptr<Expr>> impltraits;
  if (*lexer.getCurrentToken() == TokenType::TOK_OP_DCL) {
    lexer.next(); // eat :
    parseTraitImpl(err, impltraits);
  }

  if (!expect(TokenType::TOK_EOL)) {
    generateError(std::make_unique<SyntaxError>(LVL_ERROR,
      SyntaxErrorCode::STX_ERR_EXPECTED_EOL, lexer.getCurrentToken()->getPosition()));
  }
  // trait body: funtions
  std::list<std::unique_ptr<FuncExpr>> functions;
  parseTraitBody(err, functions);

  const Position &pos = lexer.getCurrentToken()->getPosition();
  if (!expect(TokenType::TOK_STMT)) {
    generateError(std::make_unique<SyntaxError>(LVL_ERROR,
      SyntaxErrorCode::STX_ERR_EXPECTED_STMT, lexer.getCurrentToken()->getPosition(),
      std::vector<Position> { tokId->getPosition() }));
  }

  if (err)
    return nullptr;

  return std::make_unique<TraitExpr>(lexer, pos, std::move(caps),
      tokId, std::move(templ),
      std::move(impltraits), std::move(functions));
}

void Parser::parseTraitImpl(bool &err,
    std::vector<std::unique_ptr<Expr>> &impltraits) noexcept {
  // inherit traits
  std::unique_ptr<Expr> expr(parseExpression());
  // comma separated list of expressions
  while (expr && isBiOpExpr(*expr, TokenType::TOK_OP_COMMA)) {
    BiOpExpr &biopexpr = dynamic_cast<BiOpExpr&>(*expr);
    auto rhs = biopexpr.getRightPtr();
    if (rhs)
      impltraits.insert(impltraits.begin(), std::move(rhs));
    // next iterator step
    expr = biopexpr.getLeftPtr();
  }

  if (expr)
    impltraits.insert(impltraits.begin(), std::move(expr));
}

void Parser::parseTraitBody(bool &err,
    std::list<std::unique_ptr<FuncExpr>> &functions) noexcept {
  while (*lexer.getCurrentToken() != TokenType::TOK_STMT
      && *lexer.getCurrentToken() != TokenType::TOK_EOF) {
    if (*lexer.getCurrentToken() == TokenType::TOK_EOL) {
      lexer.next();
      continue;
    }

    std::unique_ptr<Expr> expr(parseExpression());
    if (expr && expr->getType() == ExprType::EXPR_FUNC) {
      std::unique_ptr<FuncExpr> funcExpr(
            dynamic_cast<FuncExpr*>(expr.release()));

      if (!funcExpr->getTemplates().empty()) {
        generateError(std::make_unique<SyntaxError>(LVL_ERROR,
              SyntaxErrorCode::STX_ERR_TRAIT_SCOPE_FUNC_TEMPL,
              funcExpr->getTemplates().front()->id->getPosition()
              + funcExpr->getTemplates().back()->expr->getPosition()));
      }

      if (funcExpr->getBody()) {
        generateError(std::make_unique<SyntaxError>(LVL_ERROR,
              SyntaxErrorCode::STX_ERR_TRAIT_SCOPE_FUNC_BODY,
              funcExpr->getBody()->getPosition()));
      }

      functions.push_back(std::move(funcExpr));
    } else if (expr) {
      generateError(std::make_unique<SyntaxError>(LVL_ERROR, 
            SyntaxErrorCode::STX_ERR_TRAIT_SCOPE, expr->getPosition()));
      // soft error
    }

    if (!expect(TokenType::TOK_EOL)) {
      generateError(std::make_unique<SyntaxError>(LVL_ERROR,
        SyntaxErrorCode::STX_ERR_EXPECTED_EOL, lexer.getCurrentToken()->getPosition()));
      // soft error
      skipToStmtEol();
    }
  }
}
