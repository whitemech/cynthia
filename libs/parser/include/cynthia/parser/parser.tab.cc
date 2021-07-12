// A Bison parser, made by GNU Bison 3.5.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2020 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

// Take the name prefix into account.
#define yylex ltlflex

#include "parser.tab.hh"

// Unqualified %code blocks.
#line 37 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"

#include <cstdlib>
#include <fstream>
#include <iostream>

/* include for all driver functions */
#include "cynthia/parser/driver.hpp"
#include "cynthia/parser/scanner.hpp"

#undef yylex
#define yylex scanner.ltlflex

#line 60 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"

#ifndef YY_
#if defined YYENABLE_NLS && YYENABLE_NLS
#if ENABLE_NLS
#include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#define YY_(msgid) dgettext("bison-runtime", msgid)
#endif
#endif
#ifndef YY_
#define YY_(msgid) msgid
#endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
#if defined __GNUC__ && !defined __EXCEPTIONS
#define YY_EXCEPTIONS 0
#else
#define YY_EXCEPTIONS 1
#endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
#define YYLLOC_DEFAULT(Current, Rhs, N)                                        \
  do                                                                           \
    if (N) {                                                                   \
      (Current).begin = YYRHSLOC(Rhs, 1).begin;                                \
      (Current).end = YYRHSLOC(Rhs, N).end;                                    \
    } else {                                                                   \
      (Current).begin = (Current).end = YYRHSLOC(Rhs, 0).end;                  \
    }                                                                          \
  while (false)
#endif

// Enable debugging if requested.
#if LTLFDEBUG

// A pseudo ostream that takes yydebug_ into account.
#define YYCDEBUG                                                               \
  if (yydebug_)                                                                \
  (*yycdebug_)

#define YY_SYMBOL_PRINT(Title, Symbol)                                         \
  do {                                                                         \
    if (yydebug_) {                                                            \
      *yycdebug_ << Title << ' ';                                              \
      yy_print_(*yycdebug_, Symbol);                                           \
      *yycdebug_ << '\n';                                                      \
    }                                                                          \
  } while (false)

#define YY_REDUCE_PRINT(Rule)                                                  \
  do {                                                                         \
    if (yydebug_)                                                              \
      yy_reduce_print_(Rule);                                                  \
  } while (false)

#define YY_STACK_PRINT()                                                       \
  do {                                                                         \
    if (yydebug_)                                                              \
      yystack_print_();                                                        \
  } while (false)

#else // !LTLFDEBUG

#define YYCDEBUG                                                               \
  if (false)                                                                   \
  std::cerr
#define YY_SYMBOL_PRINT(Title, Symbol) YYUSE(Symbol)
#define YY_REDUCE_PRINT(Rule) static_cast<void>(0)
#define YY_STACK_PRINT() static_cast<void>(0)

#endif // !LTLFDEBUG

#define yyerrok (yyerrstatus_ = 0)
#define yyclearin (yyla.clear())

#define YYACCEPT goto yyacceptlab
#define YYABORT goto yyabortlab
#define YYERROR goto yyerrorlab
#define YYRECOVERING() (!!yyerrstatus_)

#line 6 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
namespace cynthia {
namespace parser {
namespace ltlf {
#line 152 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"

/// Build a parser object.
LTLfParser::LTLfParser(LTLfScanner& scanner_yyarg, LTLfDriver& d_yyarg)
#if LTLFDEBUG
    : yydebug_(false), yycdebug_(&std::cerr),
#else
    :
#endif
      scanner(scanner_yyarg), d(d_yyarg) {
}

LTLfParser::~LTLfParser() {}

LTLfParser::syntax_error::~syntax_error() YY_NOEXCEPT YY_NOTHROW {}

/*---------------.
| Symbol types.  |
`---------------*/

// basic_symbol.
#if 201103L <= YY_CPLUSPLUS
template <typename Base>
LTLfParser::basic_symbol<Base>::basic_symbol(basic_symbol&& that)
    : Base(std::move(that)), value(std::move(that.value)),
      location(std::move(that.location)) {}
#endif

template <typename Base>
LTLfParser::basic_symbol<Base>::basic_symbol(const basic_symbol& that)
    : Base(that), value(that.value), location(that.location) {}

/// Constructor for valueless symbols.
template <typename Base>
LTLfParser::basic_symbol<Base>::basic_symbol(typename Base::kind_type t,
                                             YY_MOVE_REF(location_type) l)
    : Base(t), value(), location(l) {}

template <typename Base>
LTLfParser::basic_symbol<Base>::basic_symbol(typename Base::kind_type t,
                                             YY_RVREF(semantic_type) v,
                                             YY_RVREF(location_type) l)
    : Base(t), value(YY_MOVE(v)), location(YY_MOVE(l)) {}

template <typename Base>
bool LTLfParser::basic_symbol<Base>::empty() const YY_NOEXCEPT {
  return Base::type_get() == empty_symbol;
}

template <typename Base>
void LTLfParser::basic_symbol<Base>::move(basic_symbol& s) {
  super_type::move(s);
  value = YY_MOVE(s.value);
  location = YY_MOVE(s.location);
}

// by_type.
LTLfParser::by_type::by_type() : type(empty_symbol) {}

#if 201103L <= YY_CPLUSPLUS
LTLfParser::by_type::by_type(by_type&& that) : type(that.type) { that.clear(); }
#endif

LTLfParser::by_type::by_type(const by_type& that) : type(that.type) {}

LTLfParser::by_type::by_type(token_type t) : type(yytranslate_(t)) {}

void LTLfParser::by_type::clear() { type = empty_symbol; }

void LTLfParser::by_type::move(by_type& that) {
  type = that.type;
  that.clear();
}

int LTLfParser::by_type::type_get() const YY_NOEXCEPT { return type; }

// by_state.
LTLfParser::by_state::by_state() YY_NOEXCEPT : state(empty_state) {}

LTLfParser::by_state::by_state(const by_state& that) YY_NOEXCEPT
    : state(that.state) {}

void LTLfParser::by_state::clear() YY_NOEXCEPT { state = empty_state; }

void LTLfParser::by_state::move(by_state& that) {
  state = that.state;
  that.clear();
}

LTLfParser::by_state::by_state(state_type s) YY_NOEXCEPT : state(s) {}

LTLfParser::symbol_number_type
LTLfParser::by_state::type_get() const YY_NOEXCEPT {
  if (state == empty_state)
    return empty_symbol;
  else
    return yystos_[+state];
}

LTLfParser::stack_symbol_type::stack_symbol_type() {}

LTLfParser::stack_symbol_type::stack_symbol_type(YY_RVREF(stack_symbol_type)
                                                     that)
    : super_type(YY_MOVE(that.state), YY_MOVE(that.value),
                 YY_MOVE(that.location)) {
#if 201103L <= YY_CPLUSPLUS
  // that is emptied.
  that.state = empty_state;
#endif
}

LTLfParser::stack_symbol_type::stack_symbol_type(state_type s,
                                                 YY_MOVE_REF(symbol_type) that)
    : super_type(s, YY_MOVE(that.value), YY_MOVE(that.location)) {
  // that is emptied.
  that.type = empty_symbol;
}

#if YY_CPLUSPLUS < 201103L
LTLfParser::stack_symbol_type&
LTLfParser::stack_symbol_type::operator=(const stack_symbol_type& that) {
  state = that.state;
  value = that.value;
  location = that.location;
  return *this;
}

LTLfParser::stack_symbol_type&
LTLfParser::stack_symbol_type::operator=(stack_symbol_type& that) {
  state = that.state;
  value = that.value;
  location = that.location;
  // that is emptied.
  that.state = empty_state;
  return *this;
}
#endif

template <typename Base>
void LTLfParser::yy_destroy_(const char* yymsg,
                             basic_symbol<Base>& yysym) const {
  if (yymsg)
    YY_SYMBOL_PRINT(yymsg, yysym);

  // User destructor.
  YYUSE(yysym.type_get());
}

#if LTLFDEBUG
template <typename Base>
void LTLfParser::yy_print_(std::ostream& yyo,
                           const basic_symbol<Base>& yysym) const {
  std::ostream& yyoutput = yyo;
  YYUSE(yyoutput);
  symbol_number_type yytype = yysym.type_get();
#if defined __GNUC__ && !defined __clang__ && !defined __ICC &&                \
    __GNUC__ * 100 + __GNUC_MINOR__ <= 408
  // Avoid a (spurious) G++ 4.8 warning about "array subscript is
  // below array bounds".
  if (yysym.empty())
    std::abort();
#endif
  yyo << (yytype < yyntokens_ ? "token" : "nterm") << ' ' << yytname_[yytype]
      << " (" << yysym.location << ": ";
  YYUSE(yytype);
  yyo << ')';
}
#endif

void LTLfParser::yypush_(const char* m, YY_MOVE_REF(stack_symbol_type) sym) {
  if (m)
    YY_SYMBOL_PRINT(m, sym);
  yystack_.push(YY_MOVE(sym));
}

void LTLfParser::yypush_(const char* m, state_type s,
                         YY_MOVE_REF(symbol_type) sym) {
#if 201103L <= YY_CPLUSPLUS
  yypush_(m, stack_symbol_type(s, std::move(sym)));
#else
  stack_symbol_type ss(s, sym);
  yypush_(m, ss);
#endif
}

void LTLfParser::yypop_(int n) { yystack_.pop(n); }

#if LTLFDEBUG
std::ostream& LTLfParser::debug_stream() const { return *yycdebug_; }

void LTLfParser::set_debug_stream(std::ostream& o) { yycdebug_ = &o; }

LTLfParser::debug_level_type LTLfParser::debug_level() const {
  return yydebug_;
}

void LTLfParser::set_debug_level(debug_level_type l) { yydebug_ = l; }
#endif // LTLFDEBUG

LTLfParser::state_type LTLfParser::yy_lr_goto_state_(state_type yystate,
                                                     int yysym) {
  int yyr = yypgoto_[yysym - yyntokens_] + yystate;
  if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
    return yytable_[yyr];
  else
    return yydefgoto_[yysym - yyntokens_];
}

bool LTLfParser::yy_pact_value_is_default_(int yyvalue) {
  return yyvalue == yypact_ninf_;
}

bool LTLfParser::yy_table_value_is_error_(int yyvalue) {
  return yyvalue == yytable_ninf_;
}

int LTLfParser::operator()() { return parse(); }

int LTLfParser::parse() {
  int yyn;
  /// Length of the RHS of the rule being reduced.
  int yylen = 0;

  // Error handling.
  int yynerrs_ = 0;
  int yyerrstatus_ = 0;

  /// The lookahead symbol.
  symbol_type yyla;

  /// The locations where the error started and ended.
  stack_symbol_type yyerror_range[3];

  /// The return value of parse ().
  int yyresult;

#if YY_EXCEPTIONS
  try
#endif // YY_EXCEPTIONS
  {
    YYCDEBUG << "Starting parse\n";

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear();
    yypush_(YY_NULLPTR, 0, YY_MOVE(yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int(yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;

  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_(yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty()) {
      YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
      {
        yyla.type = yytranslate_(yylex(&yyla.value, &yyla.location));
      }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc) {
        YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
        error(yyexc);
        goto yyerrlab1;
      }
#endif // YY_EXCEPTIONS
    }
    YY_SYMBOL_PRINT("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get()) {
      goto yydefault;
    }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0) {
      if (yy_table_value_is_error_(yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_("Shifting", state_type(yyn), YY_MOVE(yyla));
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Default location.
      {
        stack_type::slice range(yystack_, yylen);
        YYLLOC_DEFAULT(yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT(yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
      {
        switch (yyn) {
        case 2:
#line 88 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
          (yylhs.value.formula) = (yystack_[0].value.formula);
        }
#line 608 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 3:
#line 91 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 614 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 4:
#line 92 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 620 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 5:
#line 93 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 626 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 6:
#line 94 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 632 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 7:
#line 95 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 638 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 8:
#line 96 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 644 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 9:
#line 97 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 650 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 10:
#line 98 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 656 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 11:
#line 99 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 662 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 12:
#line 100 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 668 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 13:
#line 101 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 674 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 14:
#line 102 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 680 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 15:
#line 103 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 686 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 16:
#line 104 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 692 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 17:
#line 105 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 698 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 18:
#line 106 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 704 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 19:
#line 107 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 710 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 20:
#line 108 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
        }
#line 716 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

        case 21:
#line 111 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
        {
          (yylhs.value.formula) = (yystack_[1].value.formula);
        }
#line 722 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"
        break;

#line 726 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"

        default:
          break;
        }
      }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc) {
        YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
        error(yyexc);
        YYERROR;
      }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT("-> $$ =", yylhs);
      yypop_(yylen);
      yylen = 0;
      YY_STACK_PRINT();

      // Shift the result of the reduction.
      yypush_(YY_NULLPTR, YY_MOVE(yylhs));
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_) {
      ++yynerrs_;
      error(yyla.location, yysyntax_error_(yystack_[0].state, yyla));
    }

    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3) {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      // Return failure if at end of input.
      if (yyla.type_get() == yyeof_)
        YYABORT;
      else if (!yyla.empty()) {
        yy_destroy_("Error: discarding", yyla);
        yyla.clear();
      }
    }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;

  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_(yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3; // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;) {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_(yyn)) {
          yyn += yy_error_token_;
          if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_) {
            yyn = yytable_[yyn];
            if (0 < yyn)
              break;
          }
        }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size() == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_("Error: popping", yystack_[0]);
        yypop_();
        YY_STACK_PRINT();
      }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT(error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type(yyn);
      yypush_("Shifting", YY_MOVE(error_token));
    }
    goto yynewstate;

  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty())
      yy_destroy_("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_(yylen);
    while (1 < yystack_.size()) {
      yy_destroy_("Cleanup: popping", yystack_[0]);
      yypop_();
    }

    return yyresult;
  }
#if YY_EXCEPTIONS
  catch (...) {
    YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
    // Do not try to display the values of the reclaimed symbols,
    // as their printers might throw an exception.
    if (!yyla.empty())
      yy_destroy_(YY_NULLPTR, yyla);

    while (1 < yystack_.size()) {
      yy_destroy_(YY_NULLPTR, yystack_[0]);
      yypop_();
    }
    throw;
  }
#endif // YY_EXCEPTIONS
}

void LTLfParser::error(const syntax_error& yyexc) {
  error(yyexc.location, yyexc.what());
}

// Generate an error message.
std::string LTLfParser::yysyntax_error_(state_type, const symbol_type&) const {
  return YY_("syntax error");
}

const signed char LTLfParser::yypact_ninf_ = -13;

const signed char LTLfParser::yytable_ninf_ = -1;

const signed char LTLfParser::yypact_[] = {
    10, 10, -13, -13, -13, -13, -13, -13, -13, 10,  10,  10,
    10, 10, 7,   -12, 24,  -13, -13, -13, -13, -13, -13, 10,
    10, 10, 10,  10,  10,  -13, 29,  29,  18,  31,  -4,  -13};

const signed char LTLfParser::yydefact_[] = {
    0, 0,  19, 18, 14, 15, 16, 17, 20, 0, 0, 0,  0, 0, 0, 2, 0, 10,
    9, 12, 11, 13, 1,  0,  0,  0,  0,  0, 0, 21, 3, 4, 6, 5, 7, 8};

const signed char LTLfParser::yypgoto_[] = {-13, -13, -1};

const signed char LTLfParser::yydefgoto_[] = {-1, 14, 15};

const signed char LTLfParser::yytable_[] = {
    16, 23, 24, 25, 26, 27, 28, 22, 17, 18, 19, 20, 21, 1,  28, 2,  3,
    4,  5,  6,  7,  8,  30, 31, 32, 33, 34, 35, 29, 9,  10, 11, 12, 13,
    26, 27, 28, 23, 24, 25, 26, 27, 28, 24, 25, 26, 27, 28, 27, 28};

const signed char LTLfParser::yycheck_[] = {
    1,  13, 14, 15, 16, 17, 18, 0,  9,  10, 11, 12, 13, 3,  18, 5,  6,
    7,  8,  9,  10, 11, 23, 24, 25, 26, 27, 28, 4,  19, 20, 21, 22, 23,
    16, 17, 18, 13, 14, 15, 16, 17, 18, 14, 15, 16, 17, 18, 17, 18};

const signed char LTLfParser::yystos_[] = {
    0,  3,  5,  6,  7, 8,  9,  10, 11, 19, 20, 21, 22, 23, 25, 26, 26, 26,
    26, 26, 26, 26, 0, 13, 14, 15, 16, 17, 18, 4,  26, 26, 26, 26, 26, 26};

const signed char LTLfParser::yyr1_[] = {0,  24, 25, 26, 26, 26, 26, 26,
                                         26, 26, 26, 26, 26, 26, 26, 26,
                                         26, 26, 26, 26, 26, 26};

const signed char LTLfParser::yyr2_[] = {0, 2, 1, 3, 3, 3, 3, 3, 3, 2, 2,
                                         2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3};

#if LTLFDEBUG
// YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
// First, the terminals, then, starting at \a yyntokens_, nonterminals.
const char* const LTLfParser::yytname_[] = {
    "END_OF_FILE", "error",        "$undefined", "LPAR",        "RPAR",
    "TRUE_",       "FALSE_",       "TT",         "FF",          "END",
    "LAST",        "SYMBOL",       "NEWLINE",    "EQUIVALENCE", "IMPLICATION",
    "UNTIL",       "RELEASE",      "OR",         "AND",         "EVENTUALLY",
    "ALWAYS",      "NEXT",         "WEAK_NEXT",  "NOT",         "$accept",
    "input",       "ltlf_formula", YY_NULLPTR};

const signed char LTLfParser::yyrline_[] = {
    0,  88,  88,  91,  92,  93,  94,  95,  96,  97,  98,
    99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 111};

// Print the state stack on the debug stream.
void LTLfParser::yystack_print_() {
  *yycdebug_ << "Stack now";
  for (stack_type::const_iterator i = yystack_.begin(), i_end = yystack_.end();
       i != i_end; ++i)
    *yycdebug_ << ' ' << int(i->state);
  *yycdebug_ << '\n';
}

// Report on the debug stream that the rule \a yyrule is going to be reduced.
void LTLfParser::yy_reduce_print_(int yyrule) {
  int yylno = yyrline_[yyrule];
  int yynrhs = yyr2_[yyrule];
  // Print the symbols being reduced, and their result.
  *yycdebug_ << "Reducing stack by rule " << yyrule - 1 << " (line " << yylno
             << "):\n";
  // The symbols being reduced.
  for (int yyi = 0; yyi < yynrhs; yyi++)
    YY_SYMBOL_PRINT("   $" << yyi + 1 << " =", yystack_[(yynrhs) - (yyi + 1)]);
}
#endif // LTLFDEBUG

LTLfParser::token_number_type LTLfParser::yytranslate_(int t) {
  // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
  // TOKEN-NUM as returned by yylex.
  static const token_number_type translate_table[] = {
      0, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2, 2,
      2, 2, 2, 2, 2, 2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  2,  3, 4,
      5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23};
  const int user_token_number_max_ = 278;

  if (t <= 0)
    return yyeof_;
  else if (t <= user_token_number_max_)
    return translate_table[t];
  else
    return yy_undef_token_;
}

#line 6 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"
} // namespace ltlf
} // namespace parser
} // namespace cynthia
#line 1087 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.tab.cc"

#line 113 "/home/marcofavorito/workfolder/cynthia/libs/parser/include/cynthia/parser/parser.yy"

void cynthia::parser::ltlf::LTLfParser::error(const location_type& l,
                                              const std::string& err_message) {
  std::cerr << "Error: " << err_message << " at " << l << "\n";
}
