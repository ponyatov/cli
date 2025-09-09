%{
    #include "cli.hpp"
    char *yyfile = nullptr;
%}

%option noyywrap yylineno

%x comment stack

sign  [+\-]
dec   [0-9]
alpha [a-zA-Z_]
alnum [a-zA-Z_0-9]

%%
"#!".*          {}                  // shebang
"//".*          {}                  // line comment

"/*"            {BEGIN(comment);}   // block comment
<comment>"*/"   {BEGIN(INITIAL);}
<comment>.      {}

"("             {BEGIN(stack);}     // stack effect
<stack>")"      {BEGIN(INITIAL);}
<stack>.        {}

"nop"           {yylval.op = Op::nop  ; return CMD0;}
"halt"          {yylval.op = Op::halt ; return CMD0;}

"ret"           {yylval.op = Op::ret  ; return CMD0;}

"lita"          {yylval.op = Op::lita ; return CMDA;}
"litb"          {yylval.op = Op::litb ; return CMDB;}
"lit"           {yylval.op = Op::lit  ; return CMDC;}

"add"           {yylval.op = Op::add  ; return CMD0;}
"sub"           {yylval.op = Op::sub  ; return CMD0;}
"mul"           {yylval.op = Op::mul  ; return CMD0;}
"div"           {yylval.op = Op::div  ; return CMD0;}

"key"           {yylval.op = Op::key  ; return CMD0;}
"emit"          {yylval.op = Op::emit ; return CMD0;}

"begin"         {return BEGIN_;}
"again"         {return AGAIN ;}

":"             {return COLON;}
{alpha}{alnum}* {yylval.s = new std::string(yytext); return ID;}

{sign}?{dec}+   {yylval.n = atoi(yytext); return INT;}

[ \t\r\n]+      {}                  // drop spaces
.               {yyerror("");}      // any undetected char
