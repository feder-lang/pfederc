cmake_minimum_required(VERSION 3.10)
project(pfederc_test)

include(CTest)
enable_testing()

find_program(VALGRIND "valgrind")
if (VALGRIND)
	set(VALGRIND "${VALGRIND}" --tool=memcheck --leak-check=yes -q)
endif()

macro(build_test name)
	add_executable("${name}"
		"${pfederc_test_SOURCE_DIR}/${name}.cpp")
	target_link_libraries("${name}" ${PFEDERC_LIBRARIES})
	add_lto_support("${name}")
endmacro()

macro(status_test name)
	build_test(${name})
	add_test("${name}" "${name}")
endmacro()

macro(status_test_arg testname name args)
	add_test(NAME "${testname}" COMMAND "${name}" ${args})
endmacro()

macro(match_test testname name args match)
	add_test(NAME "${testname}" COMMAND "${name}" ${args})
	set_property(TEST "${testname}" PROPERTY TIMEOUT 10)
	set_property(TEST "${testname}" PROPERTY PASS_REGULAR_EXPRESSION "${match}")
endmacro()

macro(valgrind_test testname name args)
	if (VALGRIND)
		add_test(NAME "${testname}" COMMAND ${VALGRIND} "${name}" ${args})
	endif()
endmacro()

macro(fail_test testname name args)
	add_test(NAME ${testname} COMMAND ${name} ${args})
	set_property(TEST ${testname} PROPERTY WILL_FAIL TRUE)
endmacro()

status_test(tokenidtostr)

build_test(tokenid)
build_test(tokenidmem)
build_test(tokenstr)
build_test(ast)
build_test(astmem)
build_test(astopt)
build_test(astoptmem)
#build_test(print_mangles)

#status_test(semantic_children)
#status_test(semantic_safe_children)

# Identifiers
# good
match_test(tokenid_id00 tokenid "variable" "^TOK_ID\n$")
match_test(tokenid_id01 tokenid "_Variable" "^TOK_ID\n$")
match_test(tokenid_id02 tokenid "_Var0123456789" "^TOK_ID\n$")
match_test(tokenid_id03 tokenid "snake_cases_used" "^TOK_ID\n$")
match_test(tokenid_id04 tokenid "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "^TOK_ID\n$")
match_test(tokenid_id05 tokenid "abcdefghijklmnopqrtstuvwyz" "^TOK_ID\n$")
match_test(tokenid_id06 tokenid "a0bcdefghijklmnopqrtstuvwyz" "^TOK_ID\n$")
## bad
match_test(tokenid_id50 tokenid "__" "^TOK_ERR\n")
match_test(tokenid_id51 tokenid "_0" "^TOK_ERR\n")
# Keywords
match_test(tokenid_kw00 tokenid "func" "^TOK_KW_FN\n$")
match_test(tokenid_kw01 tokenid "module" "^TOK_KW_MOD\n$")
match_test(tokenid_kw02 tokenid "class" "^TOK_KW_CLASS\n$")
match_test(tokenid_kw03 tokenid "enum" "^TOK_KW_ENUM\n$")
match_test(tokenid_kw04 tokenid "trait" "^TOK_KW_TRAIT\n$")
match_test(tokenid_kw05 tokenid "type" "^TOK_KW_TYPE\n$")
match_test(tokenid_kw06 tokenid "return" "^TOK_KW_RET\n$")
match_test(tokenid_kw07 tokenid "use" "^TOK_KW_USE\n$")
match_test(tokenid_kw08 tokenid "if" "^TOK_KW_IF\n$")
match_test(tokenid_kw09 tokenid "else" "^TOK_KW_ELSE\n$")
match_test(tokenid_kw10 tokenid "match" "^TOK_KW_MATCH\n$")
match_test(tokenid_kw11 tokenid "switch" "^TOK_KW_SWITCH\n$")
match_test(tokenid_kw12 tokenid "for" "^TOK_KW_FOR\n$")
match_test(tokenid_kw13 tokenid "do" "^TOK_KW_DO\n$")
match_test(tokenid_kw14 tokenid "continue" "^TOK_KW_CTN\n$")
match_test(tokenid_kw15 tokenid "break" "^TOK_KW_BRK\n$")
match_test(tokenid_kw16 tokenid "include" "^TOK_KW_INC\n$")
match_test(tokenid_kw17 tokenid "import" "^TOK_KW_IMPORT\n$")
match_test(tokenid_kw18 tokenid "safe" "^TOK_KW_SAFE\n$")
match_test(tokenid_kw19 tokenid "True" "^TOK_KW_TRUE\n$")
match_test(tokenid_kw20 tokenid "False" "^TOK_KW_FALSE\n$")
match_test(tokenid_kw21 tokenid "lambda" "^TOK_KW_LAMBDA\n$")
match_test(tokenid_kw22 tokenid "ensure" "^TOK_KW_ENSURE\n$")
# Operators
match_test(tokenid_op00 tokenid "," "^TOK_OP_COMMA\n$")
match_test(tokenid_op01 tokenid ":=" "^TOK_OP_ASG_DCL\n$")
match_test(tokenid_op02 tokenid "&=" "^TOK_OP_ASG_AND\n$")
match_test(tokenid_op03 tokenid "^=" "^TOK_OP_ASG_XOR\n$")
match_test(tokenid_op04 tokenid "|=" "^TOK_OP_ASG_OR\n$")
match_test(tokenid_op05 tokenid "<<=" "^TOK_OP_ASG_LSH\n$")
match_test(tokenid_op06 tokenid ">>=" "^TOK_OP_ASG_RSH\n$")
match_test(tokenid_op07 tokenid "%=" "^TOK_OP_ASG_MOD\n$")
match_test(tokenid_op08 tokenid "/=" "^TOK_OP_ASG_DIV\n$")
match_test(tokenid_op09 tokenid "*=" "^TOK_OP_ASG_MUL\n$")
match_test(tokenid_op10 tokenid "-=" "^TOK_OP_ASG_SUB\n$")
match_test(tokenid_op11 tokenid "+=" "^TOK_OP_ASG_ADD\n$")
match_test(tokenid_op12 tokenid "=" "^TOK_OP_ASG\n$")
match_test(tokenid_op13 tokenid "null" "^TOK_OP_NULL\n$")
match_test(tokenid_op14 tokenid "||" "^TOK_OP_LOR\n$")
match_test(tokenid_op15 tokenid "&&" "^TOK_OP_LAND\n$")
match_test(tokenid_op16 tokenid "<>" "^TOK_OP_ARG\n$")
match_test(tokenid_op17 tokenid "|" "^TOK_OP_BOR\n$")
match_test(tokenid_op18 tokenid "^" "^TOK_OP_BXOR\n$")
match_test(tokenid_op19 tokenid "&" "^TOK_OP_BAND\n$")
match_test(tokenid_op20 tokenid "==" "^TOK_OP_EQ\n$")
match_test(tokenid_op21 tokenid "!=" "^TOK_OP_NQ\n$")
match_test(tokenid_op22 tokenid "<" "^TOK_OP_LT\n$")
match_test(tokenid_op23 tokenid "<=" "^TOK_OP_LEQ\n$")
match_test(tokenid_op24 tokenid ">" "^TOK_OP_GT\n$")
match_test(tokenid_op25 tokenid ">=" "^TOK_OP_GEQ\n$")
match_test(tokenid_op26 tokenid "<<" "^TOK_OP_LSH\n$")
match_test(tokenid_op27 tokenid ">>" "^TOK_OP_RSH\n$")
match_test(tokenid_op28 tokenid "+" "^TOK_OP_ADD\n$")
match_test(tokenid_op29 tokenid "-" "^TOK_OP_SUB\n$")
match_test(tokenid_op30 tokenid "%" "^TOK_OP_MOD\n$")
match_test(tokenid_op31 tokenid "*" "^TOK_OP_MUL\n$")
match_test(tokenid_op32 tokenid "/" "^TOK_OP_DIV\n$")
match_test(tokenid_op33 tokenid ":" "^TOK_OP_DCL\n$")
match_test(tokenid_op34 tokenid "++" "^TOK_OP_INC\n$")
match_test(tokenid_op35 tokenid "--" "^TOK_OP_DEC\n$")
match_test(tokenid_op36 tokenid "!" "^TOK_OP_LN\n$")
match_test(tokenid_op37 tokenid "~" "^TOK_OP_BN\n$")
match_test(tokenid_op38 tokenid "." "^TOK_OP_MEM\n$")
match_test(tokenid_op39 tokenid "->" "^TOK_OP_DMEM\n$")
# STMT
match_test(tokenid_stmt00 tokenid "\;" "^TOK_STMT\n$")
# IMPL
match_test(Tokenid_impl00 tokenid "=>" "^TOK_IMPL\n$")
# Brackets
match_test(tokenid_br00 tokenid "(" "^TOK_OP_BRACKET_OPEN\n$")
match_test(tokenid_br01 tokenid ")" "^TOK_BRACKET_CLOSE\n$")
match_test(tokenid_br02 tokenid "[" "^TOK_OP_ARR_BRACKET_OPEN\n$")
match_test(tokenid_br03 tokenid "]" "^TOK_ARR_BRACKET_CLOSE\n$")
match_test(tokenid_br04 tokenid "{" "^TOK_OP_TEMPL_BRACKET_OPEN\n$")
match_test(tokenid_br05 tokenid "}" "^TOK_TEMPL_BRACKET_CLOSE\n$")
# Integers
# good
match_test(tokenid_int00 tokenid "0" "^TOK_INT32\n$")
match_test(tokenid_int01 tokenid "0u" "^TOK_UINT32\n$")
match_test(tokenid_int02 tokenid "0s" "^TOK_INT8\n$")
match_test(tokenid_int03 tokenid "0S" "^TOK_INT16\n$")
match_test(tokenid_int04 tokenid "0l" "^TOK_INT32\n$")
match_test(tokenid_int05 tokenid "0L" "^TOK_INT64\n$")
match_test(tokenid_int06 tokenid "0us" "^TOK_UINT8\n$")
match_test(tokenid_int07 tokenid "0uS" "^TOK_UINT16\n$")
match_test(tokenid_int08 tokenid "0ul" "^TOK_UINT32\n$")
match_test(tokenid_int09 tokenid "0uL" "^TOK_UINT64\n$")
match_test(tokenid_int10 tokenid "1234567890" "^TOK_INT32\n$")
match_test(tokenid_int11 tokenid "1234567890u" "^TOK_UINT32\n$")
match_test(tokenid_int12 tokenid "0b01010101" "^TOK_INT32\n$")
match_test(tokenid_int13 tokenid "0b01010101us" "^TOK_UINT8\n$")
match_test(tokenid_int14 tokenid "0o01234567" "^TOK_INT32\n$")
match_test(tokenid_int15 tokenid "0o01234567u" "^TOK_UINT32\n$")
match_test(tokenid_int16 tokenid "0x01234567" "^TOK_INT32\n")
match_test(tokenid_int17 tokenid "0x89ABCDEF" "^TOK_INT32\n")
match_test(tokenid_int18 tokenid "0x89ABCDEFu" "^TOK_UINT32\n")
# bad
match_test(tokenid_int50 tokenid "0b012" "^TOK_ERR\n")
match_test(tokenid_int51 tokenid "0o178" "^TOK_ERR\n")
match_test(tokenid_int52 tokenid "0xEFG" "^TOK_ERR\n")
match_test(tokenid_int53 tokenid "123E" "^TOK_ERR\n")
match_test(tokenid_int54 tokenid "00" "^TOK_ERR\n")
match_test(tokenid_int55 tokenid "01" "^TOK_ERR\n")
# Floats
# good
match_test(tokenid_flt00 tokenid "0.0" "^TOK_FLT64\n$")
match_test(tokenid_flt01 tokenid "0.0f" "^TOK_FLT32\n$")
match_test(tokenid_flt02 tokenid "0.0F" "^TOK_FLT64\n$")
match_test(tokenid_flt03 tokenid "1234567890.0123456789" "^TOK_FLT64\n$")
# bad
match_test(tokenid_flt04 tokenid "0.0u" "^TOK_ERR\n")
match_test(tokenid_flt05 tokenid "1.f" "^TOK_ERR\n")
# Char
# good
match_test(tokenid_char00 tokenid "'a'" "^TOK_CHAR\n$")
match_test(tokenid_char01 tokenid "'\\''" "^TOK_CHAR\n$")
match_test(tokenid_char02 tokenid "'\\\"'" "^TOK_CHAR\n$")
#match_test(tokenid_char03 tokenid "'\\\\'" "^TOK_CHAR\n$")
match_test(tokenid_char04 tokenid "'\\\\a'" "^TOK_CHAR\n$")
match_test(tokenid_char05 tokenid "'\\\\b'" "^TOK_CHAR\n$")
match_test(tokenid_char06 tokenid "'\\\\f'" "^TOK_CHAR\n$")
match_test(tokenid_char07 tokenid "'\\\\n'" "^TOK_CHAR\n$")
match_test(tokenid_char08 tokenid "'\\\\r'" "^TOK_CHAR\n$")
match_test(tokenid_char09 tokenid "'\\\\t'" "^TOK_CHAR\n$")
match_test(tokenid_char10 tokenid "'\\\\v'" "^TOK_CHAR\n$")
match_test(tokenid_char11 tokenid "'\\\\xAA'" "^TOK_CHAR\n$")
match_test(tokenid_char12 tokenid "'\\\\x0A'" "^TOK_CHAR\n$")
# bad
match_test(tokenid_char50 tokenid "'aa'" "^TOK_ERR\n")
match_test(tokenid_char51 tokenid "'\\\\xFG'" "^TOK_ERR\n")
match_test(tokenid_char52 tokenid "'\\\\xGF'" "^TOK_ERR\n")
# String
# good
match_test(tokenid_str00 tokenid "\"hello, world!\"" "^TOK_STR\n$")
# bad
match_test(tokenid_str50 tokenid "\"" "^TOK_ERR\n")
match_test(tokenid_str51 tokenid "\"abcd" "^TOK_ERR\n")
# EOL
match_test(tokenid_eol tokenid "\n" "^TOK_EOL\n$")
# EOF
match_test(tokenid_eof tokenid " " "^TOK_EOF\n$")
# Any
match_test(tokenid_any tokenid "_" "^TOK_ANY\n$")
# Directive
match_test(tokenid_directive tokenid "#" "^TOK_DIRECTIVE\n$")
match_test(tokenid_ensure tokenid "#!" "^TOK_ENSURE\n$")

# comments and spaces
# good
match_test(tokenid_sp00 tokenid "// comment\n123" "^TOK_EOL\n$")
match_test(tokenid_sp01 tokenid "/* comment */ 123" "^TOK_INT32\n$")
match_test(tokenid_sp02 tokenid "    123" "^TOK_INT32\n$")
match_test(tokenid_sp03 tokenid "/* comment\nanother comment*/ 123"
  "^TOK_INT32\n$")
# bad
match_test(tokenid_sp50 tokenid "/* comment" "^TOK_ERR\n")

# kill tests
status_test_arg(tokenid_kill00 tokenidmem "ka\;jsdlkfa\;h")
status_test_arg(tokenid_kill01 tokenidmem "[as]123786!@#~")
status_test_arg(tokenid_kill02 tokenidmem "\\\\")
status_test_arg(tokenid_kill03 tokenidmem "\n")
status_test_arg(tokenid_kill04 tokenidmem "         akl\;sdjfa123")

# tokenstr

# string
match_test(tokenstr_str00 tokenstr "\"hello\""
  "^\"hello\"\n$")
match_test(tokenstr_str01 tokenstr "\"hello0123456789\""
  "^\"hello0123456789\"\n$")
# char
match_test(tokenstr_char00 tokenstr "'h'"
  "^'h'\n$")
# float
match_test(tokenstr_flt00 tokenstr "2.0" "^2.0")
match_test(tokenstr_flt01 tokenstr "2.0f" "^2.0")

# memory tests
valgrind_test(tokenid_memstr00 $<TARGET_FILE:tokenidmem> "\"hello, world!\"")
valgrind_test(tokenid_memstr01 $<TARGET_FILE:tokenidmem> "\"")
valgrind_test(tokenid_memstr02 $<TARGET_FILE:tokenidmem> "\"abcd")
valgrind_test(tokenid_memid00 $<TARGET_FILE:tokenidmem> "abcd")
valgrind_test(tokenid_memflt00 $<TARGET_FILE:tokenidmem> "1.0")
valgrind_test(tokenid_memch00 $<TARGET_FILE:tokenidmem> "'h")
valgrind_test(tokenid_memint00 $<TARGET_FILE:tokenidmem> "1")
valgrind_test(tokenid_memint01 $<TARGET_FILE:tokenidmem> "0x0")
valgrind_test(tokenid_memint02 $<TARGET_FILE:tokenidmem> "0o0")
valgrind_test(tokenid_memint03 $<TARGET_FILE:tokenidmem> "0b0")
valgrind_test(tokenid_memint14 $<TARGET_FILE:tokenidmem> "(")
valgrind_test(tokenid_memeof00 $<TARGET_FILE:tokenidmem> "              ")

# -------------------------------- syntax ------------------------------------
# simple
match_test(ast00 ast "identifier" "^identifier\n$")
match_test(ast01 ast "\\\"String\\\"" "^\"String\"\n$")
match_test(ast02 ast "1234" "^1234\n$")
# binary operators
# good
match_test(ast_biop00 ast "a == b" "^\\\\(== a b\\\\)\n$")
match_test(ast_biop01 ast "a != b" "^\\\\(!= a b\\\\)\n$")
match_test(ast_biop02 ast "a + b" "^\\\\(\\\\+ a b\\\\)\n$")
match_test(ast_biop03 ast "a - b" "^\\\\(- a b\\\\)\n$")
match_test(ast_biop04 ast "a b" "^\\\\( a b\\\\)\n$")
match_test(ast_biop05 ast "a = a b" "^\\\\(= a \\\\( a b\\\\)\\\\)\n$")
match_test(ast_biop06 ast "a b * c" "^\\\\( a \\\\(\\\\* b c\\\\)\\\\)\n$")
# bad
match_test(ast_biop50 ast "a -" "^ERR\n")
match_test(ast_biop51 ast "/ a" "^ERR\n")
match_test(ast_biop52 ast "a *" "^ERR\n")
match_test(ast_biop53 ast "a %" "^ERR\n")
match_test(ast_biop54 ast "a ==" "^ERR\n")
# precedence/associativity
match_test(ast_prec00 ast "a + b * c"
  "^\\\\(\\\\+ a \\\\(\\\\* b c\\\\)\\\\)\n$")
match_test(ast_prec01 ast "a + b + c"
  "^\\\\(\\\\+ \\\\(\\\\+ a b\\\\) c\\\\)\n$")
match_test(ast_prec02 ast "a * b * c"
  "^\\\\(\\\\* \\\\(\\\\* a b\\\\) c\\\\)\n$")
match_test(ast_prec03 ast "a : b : c"
  "^\\\\(: a \\\\(: b c\\\\)\\\\)\n$")
match_test(ast_prec04 ast "a = b = c"
  "^\\\\(= a \\\\(= b c\\\\)\\\\)\n$")
match_test(ast_prec05 ast "a + b - c"
  "^\\\\(- \\\\(\\\\+ a b\\\\) c\\\\)\n$")
# unary operators
match_test(ast_unop00 ast "++a" "^\\\\(\\\\+\\\\+ a\\\\)\n$")
match_test(ast_unop01 ast "+a" "^\\\\(\\\\+ a\\\\)\n$")
match_test(ast_unop02 ast "-a" "^\\\\(- a\\\\)\n$")
match_test(ast_unop03 ast "&a" "^\\\\(& a\\\\)\n$")
match_test(ast_unop04 ast "*a" "^\\\\(\\\\* a\\\\)\n")
# unayr/binary operator precedence
match_test(ast_biunop00 ast "+a.b"
  "^\\\\(\\\\+ \\\\(\\\\. a b\\\\)\\\\)\n$")
match_test(ast_biunop01 ast "+a-b"
  "^\\\\(- \\\\(\\\\+ a\\\\) b\\\\)\n$")
match_test(ast_biunop02 ast "+a+b"
  "^\\\\(\\\\+ \\\\(\\\\+ a\\\\) b\\\\)\n$")
match_test(ast_biunop03 ast "+a.b-c.d.e/f"
  "^\\\\(- \\\\(\\\\+ \\\\(\\\\. a b\\\\)\\\\) \\\\(/ \\\\(\\\\. \\\\(\\\\. c d\\\\) e\\\\) f\\\\)\\\\)")

# fn, arr, templ calls
# fn
# good
match_test(ast_fncall00 ast "a()" "^\\\\(a\\\\)\n$")
match_test(ast_fncall01 ast "a(b)" "^\\\\(a b\\\\)\n$")
match_test(ast_fncall02 ast "a(b, c)" "^\\\\(a b c\\\\)\n$")
match_test(ast_fncall03 ast "a.b(c, d)" "^\\\\(\\\\(\\\\. a b\\\\) c d\\\\)\n$")
match_test(ast_fncall04 ast "a + b(c, d)" "^\\\\(\\\\+ a \\\\(b c d\\\\)\\\\)\n$")
match_test(ast_fncall05 ast "a - b.c(d, e)" "^\\\\(- a \\\\(\\\\(\\\\. b c\\\\) d e\\\\)\\\\)\n$")
# bad
match_test(ast_fncall50 ast "a(" "^ERR\n")
match_test(ast_fncall51 ast "a(b" "^ERR\n")
match_test(ast_fncall52 ast "a(b, c" "^ERR\n")
match_test(ast_fncall53 ast "a(b]" "^ERR\n")
match_test(ast_fncall54 ast "a(b}" "^ERR\n")
# arr
# good
match_test(ast_arrcall00 ast "a[0]" "^\\\\[a 0\\\\]\n$")
match_test(ast_arrcall01 ast "a[0, 1]" "^\\\\[a 0 1\\\\]\n$")
# bad
match_test(ast_arrcall50 ast "a[" "^ERR\n")
match_test(ast_arrcall51 ast "a[0" "^ERR\n")
match_test(ast_arrcall52 ast "a[0)" "^ERR\n")
match_test(ast_arrcall53 ast "a[]" "^ERR\n")
match_test(ast_arrcall54 ast "a[0}" "^ERR\n")
# templ
# good
match_test(ast_templcall00 ast "a{b}" "^{a b}\n$")
match_test(ast_templcall01 ast "a{b, c}" "^{a b c}\n$")
# bad
match_test(ast_templcall50 ast "a{" "^ERR\n")
match_test(ast_templcall51 ast "a{b" "^ERR\n")
match_test(ast_templcall52 ast "a{}" "^ERR\n")
match_test(ast_templcall53 ast "a{b)" "^ERR\n")
match_test(ast_templcall54 ast "a{b]" "^ERR\n")
match_test(ast_templcall55 ast "a{b]}" "^ERR\n")
match_test(ast_templcall56 ast "a{b)}" "^ERR\n")
match_test(ast_templcall57 ast "a{b,}" "^ERR\n")

# newline tests
match_test(ast_eol00 ast "a +\nb" "^\\\\(\\\\+ a b\\\\)\n$")
match_test(ast_eol01 ast "a +\n\n\nb" "^\\\\(\\\\+ a b\\\\)\n$")
match_test(ast_eol02 ast "++ \na" "^\\\\(\\\\+\\\\+ a\\\\)\n$")
match_test(ast_eol03 ast "+ \na" "^\\\\(\\\\+ a\\\\)\n$")

# function tests
# good
match_test(ast_fn00 ast "func helloworld\;" "^func helloworld\;\n$")
match_test(ast_fn01 ast "func helloworld: i32\;"
	"^func helloworld: i32\;\n$")
match_test(ast_fn02 ast "func helloworld(x: i32): i32\;"
	"^func helloworld\\\\(x: i32\\\\): i32\;\n$")
match_test(ast_fn03 ast "func helloworld(x: i32): i32\nreturn x\n\;"
	"^func helloworld\\\\(x: i32\\\\): i32\nreturn x\n\;\n$")
match_test(ast_fn04 ast "func helloworld(x: i32) = x\n"
	"^func helloworld\\\\(x: i32\\\\):\nreturn x\n\;\n$")
match_test(ast_fn05 ast "func helloworld(x: i32):\nreturn x\n\;"
	"^func helloworld\\\\(x: i32\\\\):\nreturn x\n\;\n$")
match_test(ast_fn06 ast "func helloworld(&x: i32):\nreturn x\n\;"
	"^func helloworld\\\\(&x: i32\\\\):\nreturn x\n\;\n$")
# bad
fail_test(ast_fn50 ast "func helloworld(x: &i32): i32\;")
fail_test(ast_fn51 ast "func helloworld(x: i32): i32")
fail_test(ast_fn52 ast "func helloworld(x: i32): i32\nx+\nx++\n\;")

# trait tests
# good
match_test(ast_trait00 ast "trait MyTrait\n\;"
  "^trait MyTrait\n\;\n$")
match_test(ast_trait01 ast "trait{X: MyOtherTrait} MyTrait\n\;"
  "^trait{X: MyOtherTrait} MyTrait\n\;\n$")
match_test(ast_trait02 ast "trait{X: MyOtherTrait} MyTrait : MyInheritedTrait\n\;"
  "^trait{X: MyOtherTrait} MyTrait: MyInheritedTrait\n\;\n")
match_test(ast_trait03 ast "trait MyTrait\nfunc helloworld\;\n\;"
  "^trait MyTrait\nfunc helloworld\;\n\;\n$")
match_test(ast_trait04 ast "trait MyTrait\nfunc helloworld0\;\nfunc helloworld1\;\n\;"
  "^trait MyTrait\nfunc helloworld0\;\nfunc helloworld1\;\n\;\n$")
# bad
fail_test(ast_trait50 ast "trait MyTrait")
fail_test(ast_trait51 ast "trait MyTrait\nfunc helloworld\n\;\n\;")
fail_test(ast_trait52 ast "trait{X: Option<Null>} MyTrait\n\;")
fail_test(ast_trait53 ast "trait MyTrait\nfunc helloworld\;hello\n\;")

# type tests
# good
match_test(ast_type00 ast "type Option{MyTrait} MyTraitOption"
  "^type {Option MyTrait} MyTraitOption\n$")
match_test(ast_type01 ast "type i32 myi32"
  "^type i32 myi32\n$")
# bad
fail_test(ast_type50 ast "type MyTrait")
fail_test(ast_type51 ast "type Option{MyTrait MyTraitOption}")
fail_test(ast_type52 ast "type")
fail_test(ast_type53 ast "type\n")
fail_test(ast_type54 ast "type\;")
fail_test(ast_type55 ast "type hello \;")

# lambda tests
# good
match_test(ast_lambda00 ast "lambda\nreturn x\n\;"
	"^lambda \nreturn x\n\;\n$")
match_test(ast_lambda01 ast "lambda (x: i32)\nreturn x\n\;"
	"^lambda \\\\(\\\\(: x i32\\\\)\\\\)\nreturn x\n\;\n$")
match_test(ast_lambda03 ast "lambda (x: i32) = x"
	"^lambda \\\\(\\\\(: x i32\\\\)\\\\)\nreturn x\n\;\n$")
match_test(ast_lambda04 ast "lambda (x: i32) = x / x"
	"^lambda \\\\(\\\\(: x i32\\\\)\\\\)\nreturn \\\\(/ x x\\\\)\n\;\n$")
# bad
fail_test(ast_lambda50 ast "lambda (x: i32) = x /")
fail_test(ast_lambda51 ast "lambda (x: i32) = ")
fail_test(ast_lambda52 ast "lambda (x: i32")
fail_test(ast_lambda53 ast "lambda (")
fail_test(ast_lambda54 ast "lambda")

# use tests
# good
match_test(ast_use00 ast "use mymod" "use mymod\n$")
match_test(ast_use01 ast "use mymod.mymod" "use \\\\(\\\\. mymod mymod\\\\)\n$")
match_test(ast_use02 ast "use mymod.mymod, amod"
	"use \\\\(\\\\. mymod mymod\\\\), amod\n$")
# bad
fail_test(ast_use50 ast "use")
fail_test(ast_use51 ast "use mymod0,")
fail_test(ast_use52 ast "use mymod0,mymod1,")
fail_test(ast_use53 ast "use mymod0.")

# module tests
# good
match_test(ast_mod00 ast "module mymodule\n;" "module mymodule\n;\n$")
match_test(ast_mod01 ast
	"module mymodule\nfunc myfunc \;\n\;" "module mymodule\nfunc myfunc\;\n\;\n$")
match_test(ast_mod02 ast
	"module mymodule\nfunc myfunc0 \;\nfunc myfunc1 \;\n\;"
	"module mymodule\nfunc myfunc0\;\nfunc myfunc1\;\n\;\n$")
match_test(ast_mod03 ast
	"module mymod\nx := 10\n\;"
	"module mymod\n\\\\(:= x 10\\\\)\n\;\n$")
match_test(ast_mod04 ast
	"module mymod\nx : i32 = 10\n\;"
	"module mymod\n\\\\(= \\\\(: x i32\\\\) 10\\\\)\n\;\n$")
# bad
fail_test(ast_mod50 ast "module mymodule\;")
fail_test(ast_mod51 ast "module mymodule\n")
fail_test(ast_mod52 ast "module mymodule\nfunc myfunc x\n\;")
fail_test(ast_mod53 ast "module mymodule\nx = 100\n\;")
fail_test(ast_mod54 ast "module mymodule\nuse module hello\n\;")
fail_test(ast_mod55 ast "module mymodule\nuse hello\n\;")
fail_test(ast_mod56 ast "module mymodule\nx = 20\n\;")
fail_test(ast_mod57 ast "module mymodule\nfunc myfunc x\n\;hello\n\;")
fail_test(ast_mod58 ast "module mymodule\nfunc myfunc\;\;")
fail_test(ast_mod59 ast "module")
fail_test(ast_mod60 ast "module mymodule")

# array tests
# good
match_test(ast_arr00 ast "[ hello ]" "\\\\[hello\\\\]\n$")
match_test(ast_arr01 ast "[0, 1]" "\\\\[0, 1\\\\]\n$")
match_test(ast_arr02 ast "[0, 1, 2]" "\\\\[0, 1, 2\\\\]\n$")
match_test(ast_arr03 ast "[Hello()\; 100]" "\\\\[\\\\(Hello\\\\)\; 100\\\\]\n$")
match_test(ast_arr04 ast "[x; 100]"
  "\\\\[x\; 100\\\\]")
# bad
fail_test(ast_arr50 ast "[ hello")
fail_test(ast_arr51 ast "[ Hello(); ]")
fail_test(ast_arr52 ast "[ ; 100 ]")
fail_test(ast_arr53 ast "[ hello, ]")
fail_test(ast_arr54 ast "[ hello )]")

# class tests
# good
match_test(ast_class00 ast "class MyClass(x: i32)\n\;"
	"class MyClass\\\\(\\\\(: x i32\\\\)\\\\)\n\;\n$")
match_test(ast_class01 ast "class MyClass(x: i32, y: i32)\n\;"
	"class MyClass\\\\(\\\\(: x i32\\\\), \\\\(: y i32\\\\)\\\\)\n\;\n$")
match_test(ast_class02 ast "class MyClass\nx: i32\n\;"
	"class MyClass\n\\\\(: x i32\\\\)\n\;\n$")
match_test(ast_class03 ast "class MyClass\nx: i32\ny: i32\n\;"
	"class MyClass\n\\\\(: x i32\\\\)\n\\\\(: y i32\\\\)\n\;\n$")
match_test(ast_class04 ast "class MyClass\nfunc myfunc \;\n\;"
	"class MyClass\nfunc myfunc\;\n\;\n$")
match_test(ast_class05 ast "class MyClass\nfunc myfunc0\;\nfunc myfunc1\;\n\;"
	"class MyClass\nfunc myfunc0\;\nfunc myfunc1\;\n\;\n$")
match_test(ast_class06 ast "class{X: MyTrait} MyClass\nfunc myfunc0\;\nfunc myfunc1\;\n\;"
  "class{X: MyTrait} MyClass\nfunc myfunc0\;\nfunc myfunc1\;\n\;\n$")
# bad
fail_test(ast_class50 ast "class MyClass")
fail_test(ast_class51 ast "class MyClass(x: i32)")
fail_test(ast_class52 ast "class MyClass \;")
fail_test(ast_class53 ast "class MyClass\nx = i32\;")
fail_test(ast_class54 ast "class MyClass\nx := i32\;")
fail_test(ast_class55 ast "class MyClass\n")
fail_test(ast_class56 ast "class MyClass(x: i32, y: i32")
fail_test(ast_class57 ast "class MyClass\nfunc myfunc \;hello\n\;")
fail_test(ast_class58 ast "class \n\;")
fail_test(ast_class59 ast "class")

# class trait tests
# good
match_test(ast_classtrait00 ast "class trait MyClass : MyTrait\nfunc helloworld\n\;\n\;"
  "^class trait MyClass: MyTrait\nfunc helloworld\n\;\n\;\n$")
match_test(ast_classtrait01 ast "class trait MyClass : MyTrait\n\;"
  "^class trait MyClass: MyTrait\n\;\n$")
# bad
fail_test(ast_classtrait50 ast "class trait MyClass : MyTrait\nfunc helloworld\;\n\;")
fail_test(ast_classtrait51 ast "class trait MyClass : MyTrait0, MyTrait1\n\;")
fail_test(ast_classtrait52 ast "class trait MyClass : MyTrait")
fail_test(ast_classtrait53 ast "class trait MyClass : MyTrait\;")
fail_test(ast_classtrait54 ast "class trait MyClass : MyTrait\n")
fail_test(ast_classtrait55 ast "class trait MyClass : MyTrait\nfunc helloworld\;hello\n\;")
fail_test(ast_classtrait56 ast "class trait\n\;")
fail_test(ast_classtrait57 ast "class trait")

# enum tests
# good
match_test(ast_enum00 ast "enum MyEnum\n\;"
  "^enum MyEnum\n\;\n$")
match_test(ast_enum01 ast "enum MyEnum\nMyConstructor0\n\;"
  "^enum MyEnum\nMyConstructor0\n\;\n$")
match_test(ast_enum02 ast "enum MyEnum\nMyConstructor0(MyClass)\n\;"
  "^enum MyEnum\nMyConstructor0\\\\(MyClass\\\\)\n\;\n$")
match_test(ast_enum03 ast "enum MyEnum\nMyConstructor0\nMyConstructor1\n\;"
  "^enum MyEnum\nMyConstructor0\nMyConstructor1\n\;\n$")
match_test(ast_enum04 ast "enum MyEnum\nMyConstructor0\nMyConstructor1(MyClass0,MyClass1)\n\;"
  "^enum MyEnum\nMyConstructor0\nMyConstructor1\\\\(MyClass0, MyClass1\\\\)\n\;\n$")
match_test(ast_enum05 ast "enum MyEnum\nMyConstructor0\nMyConstructor1(MyClass0,MyClass1,MyClass2)\n\;"
  "^enum MyEnum\nMyConstructor0\nMyConstructor1\\\\(MyClass0, MyClass1, MyClass2\\\\)\n\;\n$")
# bad
fail_test(ast_enum50 ast "enum MyEnum")
fail_test(ast_enum51 ast "enum MyEnum\n")
fail_test(ast_enum52 ast "enum MyEnum\nMyConstructor\;")
fail_test(ast_enum53 ast "enum MyEnum\nMyConstructor(\n\;")
fail_test(ast_enum54 ast "enum MyEnum\nMyConstructor(MyClass\n\;")
fail_test(ast_enum55 ast "enum MyEnum\nMyConstructor(MyClass,\n\;")
fail_test(ast_enum56 ast "enum MyEnum\nMyConstructor,\n\;")
fail_test(ast_enum57 ast "enum MyEnum\n,\n\;")
fail_test(ast_enum58 ast "enum\n\;")
fail_test(ast_enum59 ast "enum")

# safe expr
# good
match_test(ast_safe00 ast "safe MyClass()" "safe \\\\(MyClass\\\\)\n$")
match_test(ast_safe01 ast "safe [0,1]" "safe \\\\[0, 1\\\\]\n$")
match_test(ast_safe02 ast "safe [MyClass()\; 100]"
	"safe \\\\[\\\\(MyClass\\\\)\; 100\\\\]\n$")
match_test(ast_safe03 ast "safe [MyClass]" "safe \\\\[MyClass\\\\]\n$")
# bad
fail_test(ast_safe50 ast "safe 100")
fail_test(ast_safe51 ast "safe arr[42]")
fail_test(ast_safe52 ast "safe var")
fail_test(ast_safe53 ast "safe")

# capabilities
# good
match_test(ast_caps00 ast "#Inline\nfunc helloworld\n\;"
	"^#Inline\nfunc helloworld\n\;\n$")
match_test(ast_caps01 ast "#Unused\n#Inline\n#Constant\nfunc helloworld\;"
	"^#Unused\n#Inline\n#Constant\nfunc helloworld\;\n$")
match_test(ast_caps02 ast "#Unused\n#Constant\n#Inline\nfunc helloworld\;"
	"^#Unused\n#Inline\n#Constant\nfunc helloworld\;\n$")
match_test(ast_caps03 ast "#!requires x == true\nfunc hello\;"
	"^#!requires \\\\(== x true\\\\)\nfunc hello\;\n$")
match_test(ast_caps04 ast "#!ensures x == true\nfunc hello\;"
	"^#!ensures \\\\(== x true\\\\)\nfunc hello\;\n$")
match_test(ast_caps05 ast "#!ensures x == true\n#!requires x == false\nfunc hello\;"
	"^#!requires \\\\(== x false\\\\)\n#!ensures \\\\(== x true\\\\)\nfunc hello\;\n$")
match_test(ast_caps06 ast "#Inline\nclass MyClass\n\;"
	"^#Inline\nclass MyClass\n\;\n$")
match_test(ast_caps07 ast "#Inline\ntrait MyTrait\n\;"
	"^#Inline\ntrait MyTrait\n\;\n$")
match_test(ast_caps08 ast "#Inline\nclass trait MyClass : MyTrait\n\;"
	"^#Inline\nclass trait MyClass: MyTrait\n\;\n$")
match_test(ast_caps09 ast "#Inline\ntype i32 myi32"
	"^#Inline\ntype i32 myi32\n$")
# bad
fail_test(ast_caps50 ast "#Inline\nenum Option\n\;")
fail_test(ast_caps51 ast "#Inline\nx := 100\n\;")
fail_test(ast_caps52 ast "#Inline\n#Inline\nfunc hello\;")
fail_test(ast_caps53 ast "#NotADirective\nfunc hello\;")
fail_test(ast_caps54 ast "#")
fail_test(ast_caps55 ast "#!")
fail_test(ast_caps56 ast "#Inline\n")
fail_test(ast_caps57 ast "#!requires x\n")
fail_test(ast_caps58 ast "#Inline")
fail_test(ast_caps59 ast "#!requires x")

# for loop
# good
match_test(ast_for00 ast "for True\n\;"
	"^for True\n\;\n$")
match_test(ast_for01 ast "for i < 100\; ++i\n\;"
	"^for \\\\(< i 100\\\\)\; \\\\(\\\\+\\\\+ i\\\\)\n\;\n$")
match_test(ast_for02 ast "for i := 0\; i < 100\; ++i\n\;"
	"^for \\\\(:= i 0\\\\)\; \\\\(< i 100\\\\)\; \\\\(\\\\+\\\\+ i\\\\)\n\;\n$")
match_test(ast_for03 ast "for True\nhello()\n\;"
	"^for True\n\\\\(hello\\\\)\n\;\n$")
match_test(ast_for04 ast "for True\n\n\nhello()\n\n\;"
	"^for True\n\\\\(hello\\\\)\n\;\n$")
match_test(ast_for05 ast "for i := 0\;\n i < 100\;\n ++i\n\;"
	"^for \\\\(:= i 0\\\\)\; \\\\(< i 100\\\\)\; \\\\(\\\\+\\\\+ i\\\\)\n\;\n$")
# bad
fail_test(ast_for50 ast "for\n\;")
fail_test(ast_for51 ast "for")
fail_test(ast_for52 ast "for\nhello()\n")
fail_test(ast_for53 ast "for\nhello()")

# do loop
# good
match_test(ast_do00 ast "do\n\; for True"
	"^do\n\; for True\n$")
match_test(ast_do01 ast "do\n\; for i < 100\; ++i"
	"^do\n\; for \\\\(< i 100\\\\)\; \\\\(\\\\+\\\\+ i\\\\)\n$")
match_test(ast_do02 ast "do i := 0\n\; for i < 100\; ++i"
	"^do \\\\(:= i 0\\\\)\n\; for \\\\(< i 100\\\\)\; \\\\(\\\\+\\\\+ i\\\\)\n$")
match_test(ast_do03 ast "do\nhello()\n\; for True"
	"^do\n\\\\(hello\\\\)\n\; for True\n$")
match_test(ast_do04 ast "do\n\n\nhello()\n\n\n\; for True"
	"^do\n\\\\(hello\\\\)\n\; for True\n$")
match_test(ast_do05 ast "do i := 0\n\; for i < 100\;\n ++i"
	"^do \\\\(:= i 0\\\\)\n\; for \\\\(< i 100\\\\)\; \\\\(\\\\+\\\\+ i\\\\)\n$")
# bad
fail_test(ast_do50 ast "do\n\; for")
fail_test(ast_do51 ast "do")
fail_test(ast_do52 ast "do\n")
fail_test(ast_do53 ast "do\n\;")
fail_test(ast_do54 ast "do\nfor True")
fail_test(ast_do55 ast "do i := 0\n\; for")

# continue
fail_test(ast_ctn00 ast "for True\ncontinue\n\;"
  "^for True\ncontinue\n\;\n$")
fail_test(ast_ctn01 ast "do\ncontinue\n\; for True"
  "^do\ncontinue\n\; for True\n$")
# break
fail_test(ast_brk00 ast "for True\nbreak\n\;"
  "^for True\nbreak\n\;\n$")
fail_test(ast_brk01 ast "do\nbreak\n\; for True"
  "^do\nbreak\n\; for True\n$")

# if
# good
match_test(ast_if00 ast "if True\n\;"
	"^if True\n\;\n$")
match_test(ast_if01 ast "if False\nelse if True\n\;"
	"^if False\nelse if True\n\;")
match_test(ast_if02 ast "if False\nelse if True\nelse\n\;"
	"^if False\nelse if True\nelse\n\;\n$")
match_test(ast_if03 ast "if x == y\n\;"
	"^if \\\\(== x y\\\\)\n\;\n$")
match_test(ast_if04 ast "if True\nhello()\n\;"
	"^if True\n\\\\(hello\\\\)\n\;\n$")
match_test(ast_if05 ast "if True\nhello()\nelse\nworld()\n\;"
	"^if True\n\\\\(hello\\\\)\nelse\n\\\\(world\\\\)\n\;\n$")
# bad
fail_test(ast_if50 ast "if")
fail_test(ast_if51 ast "if\n")
fail_test(ast_if52 ast "if\n\;")
fail_test(ast_if53 ast "if\nelse\n")
fail_test(ast_if54 ast "if\nelse")
fail_test(ast_if55 ast "if x == y")
fail_test(ast_if56 ast "if True\nelse if\n\;")
fail_test(ast_if57 ast "if True\nelse False\n\;")

# ensure
# good
match_test(ast_ensure00 ast "ensure True\n\;"
	"^ensure True\n\;\n$")
match_test(ast_ensure01 ast "ensure False\nelse ensure True\n\;"
	"^ensure False\nelse ensure True\n\;")
match_test(ast_ensure02 ast "ensure False\nelse ensure True\nelse\n\;"
	"^ensure False\nelse ensure True\nelse\n\;\n$")
match_test(ast_ensure03 ast "ensure x == y\n\;"
	"^ensure \\\\(== x y\\\\)\n\;\n$")
match_test(ast_ensure04 ast "ensure True\nhello()\n\;"
	"^ensure True\n\\\\(hello\\\\)\n\;\n$")
match_test(ast_ensure05 ast "ensure True\nhello()\nelse\nworld()\n\;"
	"^ensure True\n\\\\(hello\\\\)\nelse\n\\\\(world\\\\)\n\;\n$")
# bad
fail_test(ast_ensure50 ast "ensure")
fail_test(ast_ensure51 ast "ensure\n")
fail_test(ast_ensure52 ast "ensure\n\;")
fail_test(ast_ensure53 ast "ensure\nelse\n")
fail_test(ast_ensure54 ast "ensure\nelse")
fail_test(ast_ensure55 ast "ensure x == y")
fail_test(ast_ensure56 ast "ensure True\nelse ensure\n\;")
fail_test(ast_ensure57 ast "ensure True\nelse False\n\;")

# match
# good
match_test(ast_match00 ast "match True\nTrue => \;\n\;"
  "^match True\nTrue => \;\n\;\n$")
match_test(ast_match01 ast "match True\nTrue => hello()\n\;\n\;"
  "^match True\nTrue => \\\\(hello\\\\)\n\;\n\;\n$")
match_test(ast_match02 ast "match True\nTrue => \;\nFalse => \;\n\;"
  "^match True\nTrue => \;\nFalse => \;\n\;\n$")
match_test(ast_match03 ast "match True\nTrue => \;\nFalse => \;\n_ => \;\n\;"
  "^match True\nTrue => \;\nFalse => \;\n_ => \;\n\;\n$")
match_test(ast_match04 ast "match myvar\nVarValue => \;\n\;"
  "^match myvar\nVarValue => \;\n\;\n$")
match_test(ast_match05 ast "match myvar\n100 => \;\n\;"
  "^match myvar\n100 => \;\n\;\n$")
match_test(ast_match06 ast "match myvar\n'c' => \;\n\;"
  "^match myvar\n'c' => \;\n\;\n$")
match_test(ast_match07 ast "match myvar\nCase0(x) => \;\nCase1(x, y) => \;\n\;"
 "^match myvar\nCase0\\\\(x\\\\) => \;\nCase1\\\\(x, y\\\\) => \;\n\;\n$")
# bad
fail_test(ast_match50 ast "match")
fail_test(ast_match51 ast "match\nTrue => \;\n\;")
fail_test(ast_match52 ast "match myvar\nTrue =>")
fail_test(ast_match53 ast "match myvar\nTrue => \;\n")
fail_test(ast_match54 ast "match myvar\nTrue => \n\;")
fail_test(ast_match55 ast "match myvar\nTrue => hello()\n")
fail_test(ast_match56 ast "match myvar\nTrue => hello()")
fail_test(ast_match57 ast "match myvar\nCase0(x) => \;\nCase1(x, y) => \;\n_(x) => \;\n\;")

# return with direct ';'
match_test(ast_return00 ast "func hello:\n100\;"
  "^func hello:\nreturn 100\n\;\n$")
match_test(ast_return01 ast "func hello:\nx := 100 + 1\nx\;"
  "^func hello:\n\\\\(:= x \\\\(\\\\+ 100 1\\\\)\\\\)\nreturn x\n\;\n$")

# templates
# good
match_test(ast_templ00 ast "class{T} MyClass\n\;"
  "^class{T} MyClass\n\;\n$")
match_test(ast_templ01 ast "class{T, S} MyClass\n\;"
  "^class{T, S} MyClass\n\;\n$")
match_test(ast_templ02 ast "enum{T} Option\nNone\nJust(x: T)\n\;"
  "^enum{T} Option\nNone\nJust\\\\(\\\\(: x T\\\\)\\\\)\n\;\n$")
match_test(ast_templ03 ast "trait{T} MyTrait\n\;"
  "^trait{T} MyTrait\n\;\n$")
match_test(ast_templ04 ast "func{T} myfunc\n\;"
  "^func{T} myfunc\n\;\n$")
# bad
fail_test(ast_templ50 ast "func{} myfunc\n\;")
fail_test(ast_templ51 ast "class{} Class\n\;")
fail_test(ast_templ52 ast "enum{} Class\n\;")
fail_test(ast_templ53 ast "trait{} Class\n\;")
fail_test(ast_templ54 ast "trait{T:} Class\n\;")

# memory tests
valgrind_test(ast_mem00 $<TARGET_FILE:astmem> "a + b")
valgrind_test(ast_mem01 $<TARGET_FILE:astmem> "a + b + c * d * e / hello")
valgrind_test(ast_mem02 $<TARGET_FILE:astmem> "\"Hello\" + \"World\"")
valgrind_test(ast_mem03 $<TARGET_FILE:astmem> "\"Hello\" + ")
valgrind_test(ast_mem04 $<TARGET_FILE:astmem> "abcde + ")
valgrind_test(ast_mem05 $<TARGET_FILE:astmem> "++ ")
valgrind_test(ast_mem06 $<TARGET_FILE:astmem> "a(b, c, d, e)")
valgrind_test(ast_mem07 $<TARGET_FILE:astmem> "a.f.g(b, c, d, e)")
valgrind_test(ast_mem08 $<TARGET_FILE:astmem> "lambda (x: i32) = x / x")
valgrind_test(ast_mem09 $<TARGET_FILE:astmem> "func helloworld(x: i32):\nreturn x\n\;")
valgrind_test(ast_mem10 $<TARGET_FILE:astmem> "class MyClass(x: i32)\n\;")
valgrind_test(ast_mem11 $<TARGET_FILE:astmem> "class MyClass(x: i32, y: i32)\n\;")
valgrind_test(ast_mem12 $<TARGET_FILE:astmem> "class enum func trait class trait type")
valgrind_test(ast_mem13 $<TARGET_FILE:astmem> "class{T} MyClass\n\;")
valgrind_test(ast_mem14 $<TARGET_FILE:astmem> "class{T, S} MyClass\n\;")

# ---------------------------- syntax optimizer ------------------------------

# str str
match_test(astopt_strstr00 astopt "\"Hello, \" \"World!\""
  "^\"Hello, World!\"\n1\n$")
valgrind_test(astopt_mem00 $<TARGET_FILE:astoptmem> "\"Hello, \" \"World!\"")
match_test(astopt_strstr01 astopt "\"Hello, \" \"World\" \" to you\""
  "^\"Hello, World to you\"\n2\n$")
valgrind_test(astopt_mem01 $<TARGET_FILE:astoptmem> "\"Hello, \" \"World!\" \"to you\"")

# num op num
match_test(astopt_numadd00 astopt "10 + 2"
	"^12\n1\n$")
valgrind_test(astopt_mem02 $<TARGET_FILE:astoptmem> "10 + 2")
match_test(astopt_numadd01 astopt "4 + 7 + 2"
	"^13\n2\n$")
valgrind_test(astopt_mem03 $<TARGET_FILE:astoptmem> "4 + 7 + 2")

match_test(astopt_numsub00 astopt "10 - 2"
	"^8\n1\n$")
match_test(astopt_numsub01 astopt "10 - 2 - 3"
	"^5\n2\n$")

match_test(astopt_nummod00 astopt "10 % 2"
	"^0\n1\n$")
valgrind_test(astopt_mem04 $<TARGET_FILE:astoptmem> "10 % 2")
match_test(astopt_nummod01 astopt "10 % 7 % 2"
	"^1\n2\n$")
valgrind_test(astopt_mem05 $<TARGET_FILE:astoptmem> "10 % 7 % 2")

match_test(astopt_nummul00 astopt "10 * 2"
	"^20\n1\n$")

match_test(astopt_numdiv00 astopt "10 / 2"
	"^5\n1\n$")

match_test(astopt_cmp00 astopt "10 < 2"
	"^False\n1\n$")
match_test(astopt_cmp01 astopt "10.0f < 2.0f"
	"^False\n1\n$")
match_test(astopt_cmp02 astopt "10.0 < 2.0"
	"^False\n1\n$")
match_test(astopt_cmp03 astopt "2 < 4"
	"^True\n1\n$")
match_test(astopt_cmp04 astopt "2.0f < 4.0f"
	"^True\n1\n$")
match_test(astopt_cmp05 astopt "2.0 < 4.0"
	"^True\n1\n$")
match_test(astopt_cmp06 astopt "2 == 2"
	"^True\n1\n$")
match_test(astopt_cmp07 astopt "2 == 4"
	"^False\n1\n$")
match_test(astopt_cmp08 astopt "2 != 2"
	"^False\n1\n$")
match_test(astopt_cmp09 astopt "2 != 4"
	"^True\n1\n$")
match_test(astopt_cmp10 astopt "2 > 4"
	"^False\n1\n$")
match_test(astopt_cmp11 astopt "4 > 2"
	"^True\n1\n$")
match_test(astopt_cmp12 astopt "2 <= 2"
	"^True\n1\n$")
match_test(astopt_cmp13 astopt "4 <= 2"
	"^False\n1\n$")
match_test(astopt_cmp14 astopt "2 >= 2"
	"^True\n1\n$")
match_test(astopt_cmp15 astopt "2 >= 4"
	"^False\n1\n$")
match_test(astopt_cmp16 astopt "2 <= 4"
	"^True\n1\n$")
match_test(astopt_cmp17 astopt "4 >= 2"
	"^True\n1\n$")

match_test(astopt_cmpnum00 astopt "4 == (3 + 1)"
	"^True\n2\n$")
match_test(astopt_cmpnum01 astopt "4 == (3 + 2)"
	"^False\n2\n$")

valgrind_test(astop_mem06 $<TARGET_FILE:astoptmem> "4 == (3 + 2)")
