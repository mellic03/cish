#pragma once
#include <stddef.h>
#include <stdint.h>

namespace cish
{
    // enum class Type: uint32_t;
    struct Symbol;
    struct Type;
    struct SubType;
    inline const char *TypeToStr( uint32_t );
    inline const char *TypeToLit( uint32_t );
}



#define CISH_ASCII\
    SemiColon = ASCII_BASE,\
    Comma,\
    Period,\
    Tilde,\
    Bang,\
    BangEqual,\
    Equal,\
    Plus,\
    Minus,\
    Star,\
    Slash,\
    Less,\
    LessEqual,\
    Greater,\
    GreaterEqual,\
    EqualEqual,\
    PlusPlus,\
    MinusMinus,\
    StarStar,\
    SlashSlash,\
    PlusEqual,\
    MinusEqual,\
    StarEqual,\
    SlashEqual,\
    LeftParen,\
    RightParen,\
    LeftBrace,\
    RightBrace,\
    LeftBracket,\
    RightBracket,\




struct cish::Symbol
{
    enum ____: uint32_t
    {
        Eof,

        Keyword = 16,
        KwdIf,
        KwdElse,
        KwdWhile,
        KwdSwitch,
        KwdLet,
        KwdConst,
        KwdFunction,
        KwdReturn,
    
        Identifier,

        ASCII_BASE,
        CISH_ASCII
        ASCII_END,
    };

};



struct cish::Type { enum ____: uint32_t
{
    None = 0,
    Error,
    Eof,
    Comment,

    Keyword = 16,
    KwdIf,
    KwdElse,
    KwdWhile,
    KwdSwitch,
    KwdLet,
    KwdConst,
    KwdFunction,
    KwdReturn,

    Identifier,

    ASCII_BASE,
    CISH_ASCII
    ASCII_END,

    Number,
    String,
    Variable,
    Function,
    FunctionCall,

    Expr,

    Grouping,
    Ternary,
    Binary,
    Unary,
    Leaf,

    Symbol,
    Literal,

    Decl,
    DataType,
    DTypePrimitive,
    DTypeComposite,

    i08, i16, i32, i64,
    u08, u16, u32, u64,
    f32, f64,

}; };






struct cish::SubType { enum ____: uint32_t
{
    None = 0,
    Error,
    Eof,
    Comment,

    Keyword = 16,
    KwdIf,
    KwdElse,
    KwdWhile,
    KwdSwitch,
    KwdLet,
    KwdConst,
    KwdFunction,
    KwdReturn,

    Identifier,

    ASCII_BASE,
    CISH_ASCII
    ASCII_END,

    Number,
    String,
    Variable,
    Function,
    FunctionCall,

    Expr,

    Grouping,
    Ternary,
    Binary,
    Unary,
    Leaf,

    Decl,
    DataType,
    DTypeBasic,
    DTypeStruct,

    i08, i16, i32, i64,
    u08, u16, u32, u64,
    f32, f64,

}; };
















inline
const char *cish::TypeToStr( uint32_t type )
{
    switch (type)
    {
        default:                    return "INVALID_TYPE";
        case Type::None:            return "None";
        case Type::Error:           return "Error";
        case Type::Eof:             return "Eof";
        case Type::Comment:         return "Comment";

        case Type::SemiColon:       return ".";
        case Type::Comma:           return ",";
        case Type::Period:          return ".";
        case Type::Tilde:           return "~";
        case Type::Bang:            return "!";
        case Type::BangEqual:       return "!=";
        case Type::Equal:           return "=";
        case Type::Plus:            return "+";
        case Type::Minus:           return "-";
        case Type::Star:            return "*";
        case Type::Slash:           return "/";
        case Type::Less:            return "<";
        case Type::LessEqual:       return "<=";
        case Type::Greater:         return ">";
        case Type::GreaterEqual:    return ">=";
        case Type::EqualEqual:      return "==";
        case Type::PlusPlus:        return "++";
        case Type::MinusMinus:      return "--";
        case Type::StarStar:        return "**";
        case Type::SlashSlash:      return "//";
        case Type::PlusEqual:       return "+=";
        case Type::MinusEqual:      return "-=";
        case Type::StarEqual:       return "*=";
        case Type::SlashEqual:      return "/=";
        case Type::LeftParen:       return "(";
        case Type::RightParen:      return ")";
        case Type::LeftBrace:       return "{";
        case Type::RightBrace:      return "}";
        case Type::LeftBracket:     return "[";
        case Type::RightBracket:    return "]";

        case Type::Keyword:         return "Keyword";
        case Type::KwdIf:           return "KwdIf";
        case Type::KwdElse:         return "KwdElse";
        case Type::KwdWhile:        return "KwdWhile";
        case Type::KwdSwitch:       return "KwdSwitch";
        case Type::KwdLet:          return "KwdLet";
        case Type::KwdConst:        return "KwdConst";
        case Type::KwdFunction:     return "KwdFunction";
        case Type::KwdReturn:       return "KwdReturn";
        case Type::Identifier:      return "Identifier";
        case Type::Number:          return "Number";
        case Type::String:          return "String";
        case Type::Variable:        return "Variable";
        case Type::Function:        return "Function";
        case Type::FunctionCall:    return "FunctionCall";
        case Type::Expr:            return "Expr";
        case Type::Grouping:        return "Grouping";
        case Type::Ternary:         return "Ternary";
        case Type::Binary:          return "Binary";
        case Type::Unary:           return "Unary";
        case Type::Leaf:            return "Leaf";
        case Type::Symbol:          return "Symbol";
        case Type::Literal:         return "Literal";
        case Type::Decl:            return "Decl";
        case Type::DataType:        return "DataType";
    }
}

