#include <exec/lists.h>
#include <proto/exec.h>

void Enqueue(struct List * list, struct Node * node)
{
    struct Node * next;
    
    if (list != NULL && node != NULL)
    {
        /* Look through the list */
        ForeachNode (list, next) 
        {
            /*
                Look for the first node with a lower pri as the node
                we have to insert into the list.
            */  
            if (node->ln_Pri > next->ln_Pri)
                break;
        }       
        
        /* Insert the node before(!) next. The situation looks like this:
        
                A<->next<->B *<-node->*
                
            ie. next->ln_Pred points to A, A->ln_Succ points to next,
            next->ln_Succ points to B, B->ln_Pred points to next.
            ln_Succ and ln_Pred of node contain illegal pointers.
        */  
        
        /* Let node point to A: A<-node */
        node->ln_Pred          = next->ln_Pred;
        
        /* Make node point to next: A<->node->next<->B */
        node->ln_Succ          = next;
        
        /* Let A point to node: A->node */
        next->ln_Pred->ln_Succ = node;
        
        /* Make next point to node: A<->node<-next<->B */
        next->ln_Pred          = node;
    }
}