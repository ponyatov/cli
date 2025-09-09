#include "cli.hpp"

int main(int argc, char *argv[]) {
    arg(0, argv[0]);
    for (int i = 1; i < argc; i++) {  //
        arg(i, argv[i]);
        yyfile = argv[i];
        assert(yyin = fopen(yyfile, "r"));
        yyparse();
        fclose(yyin);
        yyfile = nullptr;
    }
    vm();
}

void arg(int argc, char *argv) {  //
    fprintf(stderr, "arg[%i] = <%s>\n", argc, argv);
}

void yyerror(const char *msg) {
    fprintf(stderr, "\n\n %s:%i %s [%s]\n\n", yyfile, yylineno, msg, yytext);
    exit(-1);
}

byte M[Msz];
addr Cp = 0;
addr Ip = -1;

std::map<std::string, addr> label;

addr compile(Op op) { return compile((byte)op); }

addr compile(byte b) {
    assert(Cp + sizeof(byte) < Msz);
    M[Cp] = b;
    Cp += sizeof(byte);
    return Cp;
}

addr compile(addr a) {
    assert(Cp + sizeof(addr) < Msz);
    *(addr *)&M[Cp] = a;
    Cp += sizeof(addr);
    return Cp;
}

addr compile(cell c) {
    assert(Cp + sizeof(cell) < Msz);
    *(cell *)&M[Cp] = c;
    Cp += sizeof(cell);
    return Cp;
}

void tpush(addr a) {
    assert(Tp < Tsz);
    T[Tp++] = a;
}

addr tpop() {
    assert(Tp > 0);
    return T[--Tp];
}

addr T[Tsz];
byte Tp = 0;

void vm() {
    for (;;) {
        assert(Ip < Cp);
        Op op = (Op)M[Ip++];
        fprintf(stderr, "\n%.4X: %.2X ", Ip - 1, op);
        switch (op) {
            case Op::nop:
                nop();
                break;
            case Op::halt:
                halt();
                break;
            case Op::key:
                key();
                break;
            case Op::emit:
                emit();
                break;
            default:
                fprintf(stderr, " unknown command\n\n");
                exit(-1);
        }
    }
}

bool trace = true;

void nop() {
    if (trace) fprintf(stderr, "nop");
}

void halt() {
    if (trace) fprintf(stderr, "halt\n");
    exit(0);
}

cell D[Dsz];
byte Dp = 0;

void push(cell c) {
    assert(Dp < Dsz);
    D[Dp++] = c;
}

cell pop() {
    assert(Dp > 0);
    return D[--Dp];
}

void key() {
    if (trace) fprintf(stderr, "key");
    // push(getchar());
    push('A');
}

void emit() {
    if (trace) fprintf(stderr, "emit ");
    byte c = pop();
    if (trace) fprintf(stderr, "%.2X", c);
    putchar(c);
}
