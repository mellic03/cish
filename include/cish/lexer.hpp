// #pragma once

// #include <stddef.h>
// #include <stdint.h>
// #include <cish/token.hpp>


// namespace cish
// {
//     class Lexer;
//     struct Token;
// }


// class cish::Lexer
// {
// public:
//     size_t tokenize( const char *src, Token *buf, size_t bufsz );

//     char advance();
//     char peek( int offset=0 );
//     bool match( char );
//     bool matchStr( const char* );
//     bool check( char );
//     bool checkBrk( const char* );
//     bool checkStr( const char* );
//     bool isAtEnd();
//     void emit( uint32_t type );

// private:
//     const char *m_curr;
//     const char *m_end;
//     Token      *m_out;
//     Token      *m_outend;

// };

