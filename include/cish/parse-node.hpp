#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <new>
#include <linkedlist.hpp>


namespace cish
{
    class Parser;
    class TokenStream;

    enum class ParseTag: uint8_t
    {
        Base,
        Error,
        List,
        Binary,
        Unary
    };

    inline const char *ParseTagStr( ParseTag tag )
    {
        switch (tag)
        {
            default: return "INVALID TAG";
            case ParseTag::Base: return "Base";
            case ParseTag::Error: return "Error";
            case ParseTag::List: return "List";
            case ParseTag::Binary: return "Binary";
            case ParseTag::Unary: return "Unary";
        }
    }

    template <ParseTag Tag = ParseTag::Base>
    struct ParseBase;
    struct ErrorNode;
    struct ListNode;
    struct BinaryNode;
    struct UnaryNode;
    union  ParseNode;
}




template <cish::ParseTag Tag>
struct cish::ParseBase
{
    static constexpr ParseTag tag = Tag;
};



struct cish::ErrorNode: ParseBase<ParseTag::Error>
{
    ErrorNode( TokenStream& );
};


struct cish::ListNode: ParseBase<ParseTag::List>, knl::LinkedList<ParseNode>
{
    ListNode() {  };
};


struct cish::BinaryNode: ParseBase<ParseTag::Binary>
{
    ParseNode *m_lhs, *m_rhs;

    BinaryNode( TokenStream& );
    BinaryNode( ParseNode *lhs, ParseNode *rhs )
    :   m_lhs(lhs), m_rhs(rhs) {  }
};


struct cish::UnaryNode: ParseBase<ParseTag::Unary>
{
    ParseNode *m_target;

    UnaryNode( TokenStream& );
    UnaryNode( ParseNode *target )
    :   m_target(m_target) {  }
};


union cish::ParseNode
{
    ParseTag        as_Tag;
    ParseBase<>     as_Base;
    ErrorNode       as_Error;
    ListNode        as_List;
    BinaryNode      as_Binary;
    UnaryNode       as_Unary;

    bool IsError() { return as_Tag == ParseTag::Error; }

    template <typename T, typename... Args>
    static T *New( Args... args ) { return new (malloc(sizeof(ParseNode))) T(args...); }
};

