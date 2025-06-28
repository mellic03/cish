#include <cish/interpret.hpp>

#include <stdio.h>
#include <assert.h>


int cish::interpret( Node *root )
{
    assert((root->m_type == Type::Grouping));

    for (Node *expr: ((NodeGrouping*)root)->m_nodes)
    {
        printf("%s\n", TypeToStr(expr->m_type));
        expr->print();
    }

    return 0;
}



void testfn()
{
    // left->
}


