#include "cli.hpp"

int main(int argc, char *argv[]) {
    arg(0, argv[0]);
    init(argc, argv);
    for (int i = 1; i < argc; i++) {  //
        arg(i, argv[i]);
        yyfile = argv[i];
        assert(yyin = fopen(yyfile, "r"));
        yyparse();
        fclose(yyin);
        yyfile = nullptr;
    }
    save();
    vm();
}

void arg(int argc, char *argv) {  //
    fprintf(stderr, "arg[%i] = <%s>\n", argc, argv);
}

void yyerror(const char *msg) {
    fprintf(stderr, "\n\n %s:%i %s [%s]\n\n", yyfile, yylineno, msg, yytext);
    exit(-1);
}

void init(int argc, char *argv[]) {}

void save() {
    if (trace) fprintf(stderr, " save");
    sync_();
    FILE *bcx = fopen("tmp/cli.bcx", "wb");
    assert(bcx);
    fwrite(M, sizeof(byte), Cp, bcx);
    fclose(bcx);
}

void sync_() {
    if (trace) fprintf(stderr, " sync");
    const static char signature[] = "bcx";
    memcpy(M, signature, sizeof(signature));
    ((Header *)&M)->entry = Ip;
    ((Header *)&M)->heap = Cp;
}

byte M[Msz];
addr Cp = sizeof(Header);
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
    if (trace) fprintf(stderr, " vm\n");
    for (;;) {
        assert(Ip < Cp);
        Op op = (Op)M[Ip++];
        fprintf(stderr, "\n%.4X: %.2X", Ip - 1, op);
        switch (op) {
            case Op::nop:
                nop();
                break;
            case Op::halt:
                halt();
                break;
            case Op::call:
                call();
                break;
            case Op::ret:
                ret();
                break;
            case Op::lit:
                lit();
                break;
            case Op::lita:
                lita();
                break;
            case Op::litb:
                litb();
                break;
            case Op::key:
                key();
                break;
            case Op::emit:
                emit();
                break;
            default:
                fprintf(stderr, " ???\t???\n\n");
                exit(-1);
        }
    }
}

bool trace = true;

void nop() {
    if (trace) fprintf(stderr, " nop");
}

void halt() {
    if (trace) fprintf(stderr, " halt\n");
    exit(0);
}

addr R[Rsz];
byte Rp = 0;

void rpush(addr a) {
    assert(Rp < Rsz);
    R[Rp++] = a;
}

addr rpop() {
    assert(Rp > 0);
    return R[--Rp];
}

void call() {
    rpush(Ip + sizeof(addr));
    Ip = M[Ip];
    if (trace) fprintf(stderr, " call\t%.4X", Ip);
}

void ret() {
    Ip = rpop();
    if (trace) fprintf(stderr, " ret\t%.4X", Ip);
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

void lit() {
    assert(Ip + sizeof(cell) < Cp);
    cell n = M[Ip];
    Ip += sizeof(cell);
    if (trace) fprintf(stderr, " lit\t%.8X", n);
    push(n);
};

void lita() {
    assert(Ip + sizeof(addr) < Cp);
    addr a = *(addr *)&M[Ip];
    Ip += sizeof(addr);
    if (trace) fprintf(stderr, " lita\t%.4X", a);
    push(a);
}

void litb() {
    assert(Ip + sizeof(byte) < Cp);
    byte b = M[Ip++];
    if (trace) fprintf(stderr, " litb\t%.2X", b);
    push(b);
}

void key() {
    // push(getchar());
    push('A');
    if (trace) fprintf(stderr, " key\t%.2X", D[Dp - 1]);
}

void emit() {
    byte c = pop();
    if (trace) fprintf(stderr, " emit\t%.2X", c);
    putchar(c);
}
