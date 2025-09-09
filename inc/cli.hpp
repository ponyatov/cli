#pragma once

/// @defgroup config config
/// @ingroup vm
/// @{

/// @ref M size, @ref bytes (64K max)
#define Msz 0x1000
/// @ref Rsz @ref addr esses (FF max)
#define Rsz 0x100
/// @ref Dsz @ref cell s (FF max)
#define Dsz 0x10

/// @}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <map>
#include <string>

/// @defgroup main main
/// @{

/// POSIX entry point
extern int main(int argc, char *argv[]);
/// print command line argument
extern void arg(int argc, char *argv);
/// @}

/// @defgroup types types
/// @ingroup vm
/// @{
typedef uint8_t byte;
typedef uint16_t addr;
typedef int32_t cell;
/// @}

/// @defgroup memory memory
/// @ingroup vm
/// @{

/// @name main memory
/// @{
extern byte M[Msz];  ///< main memory
extern addr Cp;      ///< @ref compiler pointer
extern addr Ip;      ///< instruction pointer

/// bytecode header
struct Header {
    byte magic[4];  ///< magic number: `bcx\0`
    addr entry;     ///< program entry point
    addr latest;    ///< last word in vocabulary
    addr heap;      ///< @ref Cp value
};

/// ( -- ) dump @ref vm registers into @ref Header (before bytecode save)
extern void sync_();
/// ( -- ) save @ref vm state into bytecode file
extern void save();

/// @}

/// @name return stack
/// @{
extern addr R[Rsz];  ///< @ref call / @ref ret return stack
extern byte Rp;      ///< @ref R pointer

extern void rpush(addr a);  ///< `(R: -- a )`
extern addr rpop();         ///< `(R: a -- )`
/// @}

/// @name data stack
/// @{
extern cell D[Dsz];  ///< data stack
extern byte Dp;      ///< @ref D pointer

extern void push(cell n);  ///< `( -- n )`
extern cell pop();         ///< `( n -- )`
/// @}

/// @}

/// @defgroup vm vm
/// @ingroup main
/// @{

extern void vm();

extern bool trace;
/// @}

/// @defgroup command command
/// @ingroup vm
/// @{

/// initialize @ref vm
/// @param[in] argc number of system startup parameters
/// @param[in] argv system parameters ( = command line or boot arguments)
extern void init(int argc, char *argv[]);

/// @ref command opcode
enum class Op {
    nop = 0x00,   ///< `( -- )` @ref nop
    halt = 0xFF,  ///< `( -- )` @ref halt
    jmp = 0x01,   ///< `( -- )` @ref jmp
    jnz = 0x02,   ///< `( bool -- )` @ref jnz
    call = 0x03,  ///< `(R: -- addr )` @ref call
    ret = 0x04,   ///< `(R: addr -- )` @ref ret
    lit = 0x05,   ///< `( -- cell )` @ref lit
    lita = 0x06,  ///< `( -- addr )` @ref lita
    litb = 0x07,  ///< `( -- byte )` @ref litb

    dup = 0x10,
    drop,
    swap,
    over,
    rot,
    mrot,
    pick,
    depth,

    fetch = 0x20,

    add = 0x30,  ///< `( a b -- a+b )`
    sub,         ///< `( a b -- a-b )`
    mul,         ///< `( a b -- a*b )`
    div,         ///< `( a b -- a/b )`

    key = 0x40,  ///< `( -- char )` @ref key
    emit,        ///< `( char -- )` @ref emit

    sync = 0x80,  ///< `( -- )` @ref sync_
    save,         ///< `( -- )` @ref save
};

/// @name flow control

/// `( -- )` empty command: do nothing
extern void nop();
/// `( -- )` stop system
extern void halt();

/// `( -- )` unconditional jump
extern void jmp();
/// `( bool -- )` jump if `false`
extern void jnz();
/// `(R: -- addr )` nested call
extern void call();
/// `(R: addr -- )` return from nested @ref call
extern void ret();

/// @name number literals in code

/// `( -- cell )` push @ref cell literal
extern void lit(cell c);
/// `( -- cell )` push @ref cell literal
extern void lit();
/// `( -- addr )` push @ref addr literal
extern void lita();
/// `( -- byte )` push @ref byte literal
extern void litb();

/// @name stack manipulation

/// `( a -- a a )` duplicate top of stack
extern void dup();

/// @name imput/output

/// `( -- char )` get ASCII character from console
extern void key();
/// `( char -- )` emit single ASCII character
extern void emit();

/// @}

/// @defgroup compiler compiler
/// @ingroup main
/// @{

/// @ref T size, @ref addr esses
#define Tsz 0x10
extern addr T[Tsz];  ///< control group stack
extern byte Tp;      ///< @ref T pointer

extern void tpush(addr a);
extern addr tpop();

extern std::map<std::string, addr> label;

extern addr compile(Op op);
extern addr compile(byte b);
extern addr compile(addr a);
extern addr compile(cell c);

/// @}

/// @defgroup parser parser
/// @ingroup compiler
/// @{
extern int yylex();
extern FILE *yyin;
extern char *yyfile;
extern char *yytext;
extern int yylineno;
extern int yyparse();
extern void yyerror(const char *msg);
#include "cli.yacc.hpp"
/// @}
