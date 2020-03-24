//Written by Chris Vasquez
//March 26, 2016

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* The following code is for an interpreter for a 
   boolean expression. The user will be prompted to enter
   a valid expression into stdin. Note that every valid
   expression should end with a '.' to indicate the end of 
   the expression.*/

//Note: For each of the boolean rules an integer
//will be used to indicate true (1) or false (0)


//--------Stack for semantic evaluation-----------

typedef struct _node node_type;
typedef struct _node* node;

typedef struct _Stack stack_type;
typedef struct _Stack* stack;

struct _node{
    char symbol;
    node next;
};

struct _Stack{
    node top;
    int size;
};

//Using global stack
stack semStack;

//--------Function declarations------------------
int B(char *, int *);
int IT(char *, int *);
int IT_TAIL(char *, int *);
int OT(char *, int *);
int OT_TAIL(char *, int *);
int AT(char *, int *);
int AT_TAIL(char *, int *);
int L(char *, int *);
int A(char *, int *);

void advance(char *, int *);

stack createStack();
void push(char);
char pop();
char peek();


//--------------Error Messages--------------------
void ITTAIL_Error(char *stream, int *index){
    printf("Error: Expected one of the following: '->', '.', ')'\n");

    printf("Found: %c at the following place.\n\n", stream[*index]);
    int i;
    //Print original expression
    i = 0;
    while(stream[i] != '\0'){
        printf("%c", stream[i]);
        i++;
    }

    //Print caret to indicate error.
    i = 0;
    while(i < *index){
        printf(" ");
        i++;
    }
    printf("^\n");

    printf("Did you accidently space between '-' and '>'?\n\n");

    return;
}

void OTTAIL_Error(char *stream, int *index){
    printf("Error: Expected one of the following: 'v', '->', '.', ')'\n");

    printf("Found: %c at the following place:\n\n", stream[*index]);
    int i;
    //Print original expression
    i = 0;
    while(stream[i] != '\0'){
        printf("%c", stream[i]);
        i++;
    }

    //Print caret to indicate error.
    i = 0;
    while(i < *index){
        printf(" ");
        i++;
    }
    printf("^\n\n");
    return;
}

void ATTAIL_Error(char *stream, int *index){
    printf("Error: Expected one of the following: '^', 'v', '->', '.', ')'.\n");

    printf("Found: %c at the following place:\n\n", stream[*index]);
    int i;
    //Print original expression
    i = 0;
    while(stream[i] != '\0'){
        printf("%c", stream[i]);
        i++;
    }

    //Print caret to indicate error.
    i = 0;
    while(i < *index){
        printf(" ");
        i++;
    }
    printf("^\n\n");

    return;
}

void L_Error(char *stream, int *index){
    printf("Error: Expected one of the following: 'T', 'F', '(', '~'\n");
    printf("Found: %c at the following place:\n\n", stream[*index]);
    int i;

    //Print original expression
    i = 0;
    while(stream[i] != '\0'){
        printf("%c", stream[i]);
        i++;
    }

    //Print caret to indicate error.
    i = 0;
    while(i < *index){
        printf(" ");
        i++;
    }
    printf("^\n\n");

    return;
}

void A_Error(char *stream, int *index){
    printf("Error: Expected one of the following: 'T', 'F', '('\n");
    printf("Found: %c at the following place:\n\n", stream[*index]);
    int i;

    //Print original expression
    i = 0;
    while(stream[i] != '\0'){
        printf("%c", stream[i]);
        i++;
    }

    //Print caret to indicate error.
    i = 0;
    while(i < *index){
        printf(" ");
    }
    printf("^\n\n");
    return;
}


//--------Helper Functions----------------

//Advance will advance the index to the next non-whitespace character
void advance(char *stream, int *index){
    (*index)++;
    if(stream[*index] == '\0'){
        return;
    } else {
        while(isspace(stream[*index])){
            (*index)++;
        }
        return;     
    }

}


stack createStack(){
    stack s = (stack)malloc(sizeof(stack_type));
    s->top = NULL;
    s->size = 0;
    return s;
}

void push(char c){
    node n = (node)malloc(sizeof(node_type));
    n->symbol = c;
    n->next = semStack->top;
    semStack->top = n;
    semStack->size++;
    return;
}

char pop(){
    if(semStack == NULL || semStack->size == 0){
        printf("Stack is empty.\n");
        return '\0';
    } else {
        node temp = semStack->top;
        char ret = temp->symbol;

        semStack->top = semStack->top->next;
        temp->next = NULL;

        free(temp);
        semStack->size--;

        return ret; 
    }
}

char peek(){
    if(semStack->top == NULL){
        printf("Stack is empty.\n");
        return '\0';
    }
    printf("Peeking: %c\n", semStack->top->symbol);
    return semStack->top->symbol;
}


//-----------------Syntax Rules -------------------

/* The following rules will take a character pointer representing the stream that
   the user input and an index to indicate the current symbol we are looking at. */

int B(char *stream, int *index){
    //Check for initial whitespace
    if(isspace(stream[*index])){
        advance(stream, index);
    }

    if(IT(stream, index) == 1){
        if(stream[*index] == '.'){
            //Skip whitespace
            advance(stream, index);

            if(stream[*index] == '\0'){
                return 1;
            } else {
                printf("Error: Expected end of input but end of expression not reached.\n");
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}


int IT(char *stream, int *index){
    if(OT(stream, index) == 1){
        if(IT_TAIL(stream, index)){
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}


int IT_TAIL(char *stream, int *index){
    if(stream[*index] == '-'){
        //Note: This code is designed such that -> is valid however - >
        //is not. Whitespace is treated as a delimiter.
        (*index)++;

        if(stream[*index] == '>'){
            //Skip whitespace
            advance(stream, index);

            if(OT(stream, index) == 1){
                if(IT_TAIL(stream, index) == 1){
                    //Semantically evaluating implication expression
                    char tailSym = pop();
                    char headSym = pop();
                    if(headSym == 'F'){
                        push('T');
                    } else {
                        push(tailSym);
                    }

                    return 1;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            ITTAIL_Error(stream, index);
            return 0;   
        }
    } else if(stream[*index] == '.' || stream[*index] == ')'){
        return 1;
    } else {
        return 0;
    }
}


int OT(char *stream, int *index){
    if(AT(stream, index) == 1){
        if(OT_TAIL(stream, index) == 1){
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}


int OT_TAIL(char *stream, int *index){
    if(stream[*index] == 'v'){
        advance(stream, index);
        if(AT(stream, index) == 1){
            if(OT_TAIL(stream, index) == 1){
                //Evaluating OR expression
                char tailSym = pop();
                char headSym = pop();
                if(headSym == 'T' || tailSym == 'T'){
                    push('T');
                } else {
                    push('F');
                }
                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if(stream[*index] == '-' || stream[*index] == '.' || stream[*index] == ')'){
        return 1;
    } else {
        OTTAIL_Error(stream, index);
        return 0;
    }
}

int AT(char *stream, int *index){
    if(L(stream, index) == 1){
        if(AT_TAIL(stream, index)){
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int AT_TAIL(char *stream, int *index){
    if(stream[*index] == '^'){
        advance(stream, index);
        if(L(stream, index) == 1){
            if(AT_TAIL(stream, index) == 1){
                //Evaluating AND expression
                char tailSym = pop();
                char headSym = pop();
                if(tailSym == 'T' && headSym == 'T'){
                    push('T');
                } else {
                    push('F');
                }

                return 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if(stream[*index] == 'v' || stream[*index] == '-' || stream[*index] == '.'
        || stream[*index] == '.' || stream[*index] == ')'){
        return 1;
    } else if(stream[*index] == '\0'){
        printf("Error: Expected '.' to end input.\n");
        return 0;
    } else {
        ATTAIL_Error(stream, index);
        return 0;
    }
}


int L(char *stream, int *index){
    if(stream[*index] == '~'){
        advance(stream, index);
        if(L(stream, index) == 1){
            //Evaluating negation
            char sym = pop();
            if(sym == 'T'){
                push('F');
            } else {
                push('T');
            }
            return 1;
        } else {
            return 0;
        }
    } else if(A(stream, index) == 1) {
        return 1;
    } else {
        L_Error(stream, index);
        return 0;
    }
}


int A(char *stream, int *index){
    if(stream[*index] == 'T'){
        //Push terminal symbol
        push('T');
        advance(stream, index);
        return 1;
    } else if(stream[*index] == 'F'){
        //Push terminal symbol
        push('F');
        advance(stream, index);
        return 1;
    } else if(stream[*index] == '('){
        advance(stream, index);
        if(IT(stream, index) == 1){
            if(stream[*index] == ')'){
                advance(stream, index);
                return 1;
            } else {
                printf("Error: Missing ')'. May cause other errors.\n");
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

int main(){
    int valid = 0;
    int index = 0;
    char boolExp[100];

    semStack = createStack();

    printf("Enter a valid boolean expression.\n");
    printf("Ctrl+C to exit.\n");
    while(1){
        valid = 0;
        index = 0;
        for(int i=0;i<100;++i){boolExp[i]='\0';}
        printf(">>> ");
        fgets(boolExp, 100, stdin);
        valid = B(boolExp, &index);
        if(valid == 1){
            printf("The value of the given expression is: ");
            char value = pop();
            if(value == 'T'){
                printf("True\n");
            } else {
                printf("False\n");
            }
        } else {
            printf("Expression cannot be evaluated. Please try again.\n");
        }
    }

    return 0;
}

