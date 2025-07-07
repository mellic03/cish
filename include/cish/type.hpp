#pragma once
#include <stddef.h>
#include <stdint.h>

namespace cish
{
    struct Type;
    struct SubType;
    inline const char *TypeToStr( uint32_t );
    inline const char *TypeToLit( uint32_t );
}




struct cish::Type { enum ____: uint32_t
{
    None = 0,
    Error,
    Eof,

    Keyword = 16,
    Kwd_BEGIN = 16,
    KwdSwitch = Kwd_BEGIN,
    KwdLet, KwdConst,
    KwdIf,  KwdElse,
    KwdFor, KwdWhile,
    KwdFunc, KwdReturn,
    Kwd_END,

    Identifier,

    SemiColon,
    Comma,
    Period,
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,

    Op_BEGIN,
    EqualEqual=Op_BEGIN,         Equal,
    BangEqual,                   Bang,
    LessEqual,                   Less,
    GreaterEqual,                Greater, 
    PlusEqual,     PlusPlus,     Plus,
    MinusEqual,    MinusMinus,   Minus,
    StarEqual,   /*StarStar,  */ Star,
    SlashEqual,  /*SlashSlash,*/ Slash,

    TildeEqual,                  Tilde,
    HatEqual,                    Hat,
    BarEqual,      BarBar,       Bar,
    AmpsndEqual,   AmpAmpsnd,    Ampsnd,
    Op_END,

    Number,
    String,
    Variable,
    Function,
    FunctionCall,

    Expr,

    ProgRoot,
    ProgData,

    Grouping,
    Ternary,
    Binary,
    Unary,
    Leaf,

    Symbol,
    Literal,

    DataType_BEGIN,
    u00 = DataType_BEGIN,
    i64, i32, i16, i08,
    u64, u32, u16, u08,
    f64, f32,
    DataType_END,

};

    struct KeyPair { const char *str; uint32_t type; };

    static constexpr KeyPair KwTypes[] = {
        {"switch", KwdSwitch},
        {"let", KwdLet}, {"const", KwdConst},
        {"if",  KwdIf},  {"else",  KwdElse}, 
        {"for", KwdFor}, {"while", KwdWhile},
        {"func", KwdFunc}, {"return", KwdReturn},
    };

    static constexpr KeyPair OpTypes[] = {
        {"==", EqualEqual},                                 {"=", Type::Equal},
        {"!=", BangEqual},                                  {"!", Type::Bang},
        {"<=", LessEqual},                                  {"<", Type::Less},
        {">=", GreaterEqual},                               {">", Type::Greater}, 
        {"+=", PlusEqual},     {"++", Type::PlusPlus},      {"+", Type::Plus},
        {"-=", MinusEqual},    {"--", Type::MinusMinus},    {"-", Type::Minus},
        {"*=", StarEqual},   /*{"**", Type::StarStar},  */  {"*", Type::Star},
        {"/=", SlashEqual},  /*{"//", Type::SlashSlash},*/  {"/", Type::Slash},
        {"~=", TildeEqual},                                 {"~", Type::Tilde},
        {"^=", HatEqual},                                   {"^", Type::Hat},
        {"|=", BarEqual},      {"||", Type::BarBar},        {"|", Type::Bar},
        {"&=", AmpsndEqual},   {"&&", Type::AmpAmpsnd},     {"&", Type::Ampsnd},
    };

    static constexpr KeyPair DataTypes[] = {
        {"u00", u00},
        {"i64", i64}, {"i32", i32}, {"i16", i16}, {"i08", i08},
        {"u64", u64}, {"u32", u32}, {"u16", u16}, {"u08", u08},
        {"f64", f64}, {"f32", f32},
    };


};







inline
const char *cish::TypeToStr( uint32_t type )
{
    if (Type::Kwd_BEGIN<=type && type<Type::Kwd_END)
        return Type::KwTypes[type - Type::Kwd_BEGIN].str;

    if (Type::DataType_BEGIN<=type && type<Type::DataType_END)
        return Type::DataTypes[type - Type::DataType_BEGIN].str;

    // if (Type::Op_BEGIN<=type && type<Type::Op_END)
    //     return Type::OpTypes[type - Type::Op_BEGIN].str;

    switch (type)
    {
        default:                    return "INVALID_TYPE";
        case Type::None:            return "None";
        case Type::Error:           return "Error";
        case Type::Eof:             return "Eof";

        case Type::SemiColon:       return "SemiColon";
        case Type::Comma:           return "Comma";
        case Type::Period:          return "Period";
        case Type::LeftParen:       return "LeftParen";
        case Type::RightParen:      return "RightParen";
        case Type::LeftBrace:       return "LeftBrace";
        case Type::RightBrace:      return "RightBrace";
        case Type::LeftBracket:     return "LeftBracket";
        case Type::RightBracket:    return "RightBracket";

        case Type::EqualEqual: return "EqualEqual";
        case Type::Equal:   return "Equal";
        case Type::BangEqual: return "BangEqual";
        case Type::Bang:  return "Bang";
        case Type::LessEqual: return "LessEqual";
        case Type::Less:        return "Less";
        case Type::GreaterEqual: return "GreaterEqual";
        case Type::Greater:   return "Greater";
        case Type::PlusEqual: return "PlusEqual";
        case Type::PlusPlus:  return "PlusPlus";
        case Type::Plus:      return "Plus";
        case Type::MinusEqual: return "MinusEqual";
        case Type::MinusMinus:    return "MinusMinus";
        case Type::Minus:         return "Minus";
        case Type::StarEqual: return "StarEqual";
          /*StarStar,  */ case Type::Star: return "Star";
        case Type::SlashEqual: return "SlashEqual";
         /*SlashSlash,*/ case Type::Slash: return "Slash";

        case Type::TildeEqual:    return "TildeEqual";
        case Type::Tilde:    return "Tilde";
        case Type::HatEqual:  return "HatEqual";
        case Type::Hat:  return "Hat";
        case Type::BarEqual:  return "BarEqual";
        case Type::BarBar:  return "BarBar";
        case Type::Bar:  return "Bar";
        case Type::AmpsndEqual:   return "AmpsndEqual";
        case Type::AmpAmpsnd:   return "AmpAmpsnd";
        case Type::Ampsnd:   return "Ampsnd";

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
    }
}

