%skeleton "lalr1.cc" /* -*- C++ -*- */
%require  "3.0"
%debug 
%defines 
%define api.prefix {ltlf}
%define api.namespace {cynthia::parser::ltlf}

/*
 * bison 3.3.2 deprecates %define parser_class_name {}
 * for %define api.parser.class {}, but
 * we want backward compatibility for bison 3.0.4.
 */
%define api.parser.class {LTLfParser}

%code requires{
   #include "cynthia/logic/base.hpp"
   #include "cynthia/parser/parser_stype.h"
namespace cynthia::parser::ltlf {
      class LTLfDriver;
      class LTLfScanner;
}

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { LTLfScanner  &scanner  }
%parse-param { LTLfDriver  &d  }

%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   
   /* include for all driver functions */
   #include "cynthia/parser/scanner.hpp"
   #include "cynthia/parser/driver.hpp"

#undef yylex
#define yylex scanner.ltlflex
}

%define parse.assert

%define api.value.type {struct cynthia::parser::ltlf::LTLf_YYSTYPE}

%type<formula> input ltlf_formula
%type<symbol_name> SYMBOL

%token                  LPAR
%token                  RPAR
%token                  TRUE_
%token                  FALSE_
%token                  TT
%token                  FF
%token                  END
%token                  LAST
%token                  SYMBOL
%token                  NEWLINE
%token                  END_OF_FILE    0

%left                   EQUIVALENCE
%right                  IMPLICATION
%left                   UNTIL
%left                   RELEASE
%left                   OR
%left                   AND
%right                  EVENTUALLY
%right                  ALWAYS
%right                  NEXT
%right                  WEAK_NEXT
%right                  NOT
%nonassoc               LPAR

%locations

%start input

%%

input: ltlf_formula                                                                     { $$ = $1;
                                                                                          d.result = $$; };

ltlf_formula: ltlf_formula EQUIVALENCE ltlf_formula                                     {  }
            | ltlf_formula IMPLICATION ltlf_formula                                     {  }
            | ltlf_formula RELEASE ltlf_formula                                         {  }
            | ltlf_formula UNTIL ltlf_formula                                           {  }
            | ltlf_formula OR ltlf_formula                                              {  }
            | ltlf_formula AND ltlf_formula                                             {  }
            | ALWAYS ltlf_formula                                                       {  }
            | EVENTUALLY ltlf_formula                                                   {  }
            | WEAK_NEXT ltlf_formula                                                    {  }
            | NEXT ltlf_formula                                                         {  }
            | NOT ltlf_formula                                                          { $$ = d.add_LTLfNot($2); }
            | TT                                                                        { $$ = d.add_LTLfTrue(); }
            | FF                                                                        { $$ = d.add_LTLfFalse(); }
            | END                                                                       {  }
            | LAST                                                                      {  }
            | FALSE_                                                                    {  }
            | TRUE_                                                                     {  }
            | SYMBOL                                                                    { $$ = d.add_LTLfAtom($1); }
            ;

ltlf_formula: LPAR ltlf_formula RPAR                                                    { $$ = $2; };

%%

void cynthia::parser::ltlf::LTLfParser::error(const location_type &l, const std::string &err_message) {
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
