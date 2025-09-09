%{
    #include "cli.hpp"
%}

%defines %union { char c; std::string *s; int n; float f; Op op; }

%token     COLON BEGIN_ AGAIN
%token<s>  ID
%token<n>  INT
%token<op> CMD0 CMDB CMDA CMDC
%%
syntax : | syntax ex

ex: COLON ID        { if (trace) fprintf(stderr,"%.4X:\t%s\n",Cp,$2->c_str());
                        label[*$2]=Cp; Ip=Cp; }
  | CMD0            { compile($1); }
  | CMDB INT        { compile($1); compile((byte)$2); }
  | CMDA INT        { compile($1); compile((addr)$2); }
  | CMDC INT        { compile($1); compile((cell)$2); }
  | BEGIN_          { tpush(Cp); }
  | AGAIN           { compile(Op::jmp ); compile(tpop()    ); }
  | ID              { compile(Op::call); compile(label[*$1]); }
