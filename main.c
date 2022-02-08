//
//  main.c
//  66diophantine_continued_frac.c
//
//  Created by Matthew Lee on 26/01/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct node
{
    int x;
    struct node *next;
}node;

int square(int x);
int diophantine(long x, long y, int d);
long min(node *a, int d);
void freelist(node *list);
int pop(node **list);
node *frac(int i, int n);
node *multiply(node *x, node *y);
node *carry(node *list);
node *reverse(node *list);
node *add(node *x, node *y);
node *mini(node *a, int d);
node *split(int x);
int diophantine2(node *x, node *y, int d);
int len(node *list);
void printlist(node *list);

int main(void)
{
    clock_t begin = clock();
    int highest = 0;
    int d = 0;
    for (int i = 1; i <= 1000; i++)
    {
        if (!square(i))
        {
            //node *a = frac(i, 15);
            //long x = min(a, i);
            //freelist(a);
            
            node *a = frac(i, 100);
            //x = mini(a, i);
            printf("%4i  ", i);
            node *list = mini(a, i);
            printlist(list);
            printf("\n");
            
            int x = len(list);
            if(x > highest)
            {
                highest = x;
                d = i;
            }
            freelist(a);
            freelist(list);
        }
    }
    printf("%i  %i\n", d, highest);
    printf("%f\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
}

//checks if it is a square number
int square(int x)
{
    /*
    if (pow((int)sqrt(x), 2) == x)
        return 1;
    return 0;*/
    
    int i;
    for(i = 0; i * i < x; i++);
    return (i * i == x);
}

long min(node *a, int d)
{
    
    for(int i = 2; i < 15; i++)
    {
        node *pntr = a;
        node *stack = NULL;
        for (int j = 0; j < i; j++)
        {
            node *n = malloc(sizeof(node));
            n -> x = pntr -> x;
            n -> next = stack;
            stack = n;
            pntr = pntr -> next;
        }
        long bottom = 1;
        long top = pop(&stack);
        while(stack != NULL)
        {
            long temp = bottom;
            bottom = top;
            top = temp;
            
            int x = pop(&stack);
            top += x * bottom;
        }
        if(diophantine(top, bottom, d))
            return top;
    }
    
    return 0;
}

node *mini(node *a, int d)
{
    node *pntr = a;
    node *stack = NULL;
    node *m = malloc(sizeof(node));
    m -> x = a -> x;
    m -> next = NULL;
    stack = m;
    
    for(int i = 2; i < 100; i++)
    {
        pntr = pntr -> next;
        node *n = malloc(sizeof(node));
        n -> x = pntr -> x;
        n -> next = stack;
        stack = n;
        
        node *tstack = stack;
        node *bottom = split(1);
        node *top = split(stack -> x);
        tstack = tstack -> next;
        while(tstack != NULL)
        {
            node *temp = bottom;
            bottom = top;
            top = temp;
            
            int x = tstack -> x;
            tstack = tstack -> next;
            node *spl = split(x);
            node *multi = multiply(spl, bottom);
            temp = add(top, multi);
            freelist(top);
            freelist(spl);
            freelist(multi);
            top = temp;
        }
        if(diophantine2(top, bottom, d))
        {
            freelist(stack);
            freelist(bottom);
            return top;
        }
        freelist(top);
        freelist(bottom);
    }
    freelist(stack);
    return 0;
}

int diophantine2(node *x, node *y, int d)
{
    node *sum = multiply(x, x); //freed
    node *square = multiply(y, y); //freed
    node *spl = split(d);
    node *sum1 = multiply(spl, square); //freed
    node *one = split(1);
    node *sum2 = add(sum1, one); //freed
    
    node *temp = sum;
    node *temp1 = sum2;
    int a = len(sum);
    if(a == len(sum2))
    {
        while(sum != NULL)
        {
            if(sum -> x != sum2 -> x)
            {
                freelist(temp);
                freelist(temp1);
                freelist(square);
                freelist(sum1);
                freelist(spl);
                freelist(one);
                return 0;
            }
            sum = sum -> next;
            sum2 = sum2 -> next;
        }
        freelist(temp);
        freelist(temp1);
        freelist(square);
        freelist(sum1);
        freelist(spl);
        freelist(one);
        return 1;
    }
    freelist(temp);
    freelist(temp1);
    freelist(square);
    freelist(sum1);
    freelist(spl);
    freelist(one);
    return 0;
}

node *split(int x)
{
    node *a = NULL;
    while(x)
    {
        node *n = malloc(sizeof(node));
        n -> x = x % 10;
        x /= 10;
        n -> next = a;
        a = n;
    }
    return a;
}

int diophantine(long x, long y, int d)
{
    int sum = pow(x, 2) - d * pow(y, 2);
    if(sum == 1)
        return 1;
    return 0;
}

node *frac(int i, int n)
{
    int root = sqrt(i);
    node *a = malloc(sizeof(node));
    a -> x = root;
    a -> next = NULL;
    
    int minus = -a -> x;
    node *end = a;
    int prev = 1;
    for (int j = 0; j < n; j++)
    {
        int y = i - pow(minus, 2);
        if (!(y % prev))
        {
            y /= prev;
            prev = 1;
        }
        node *m = malloc(sizeof(node));
        m -> next = NULL;
        end -> next = m;
        end = m;
        m -> x = (sqrt(i) - minus) / y;
        minus = (prev * -minus) - (y * m -> x);
        prev = y;
    }
    return a;
}

void freelist(node *list)
{
    node *prev = list;
    while (list != NULL)
    {
        list = list -> next;
        free(prev);
        prev = list;
    }
}

int pop(node **list)
{
    node *n = *list;
    if (n != NULL)
    {
        int x = n -> x;
        *list = n -> next;
        free(n);
        return x;
    }
    return 0;
}

node *multiply(node *x, node *y)
{
    node *result = malloc(sizeof(node));
    result -> x = 0;
    result -> next = NULL;
    
    if (len(x) > len(y))
    {
        node *temp = y;
        y = x;
        x = temp;
    }
    
    node *b = result;
    if (x != y)
    {
        x = reverse(x);
        y = reverse(y);
    }
    else
    {
        x = reverse(x);
        y = x;
    }
    node *a = b;
    node *hold = x;
    
    while(x != NULL)
    {
        a = b;
        node *temp = y;
        node *prev = a;
        while(temp != NULL)
        {
            if(a == NULL)
            {
                node *n = malloc(sizeof(node));
                n -> x = temp -> x * x -> x;
                n -> next = NULL;
                prev -> next = n;
                a = prev -> next;
            }
            else
            {
                a -> x += temp -> x * x -> x;
            }
            prev = a;
            a = a -> next;
            temp = temp -> next;
        }
        x = x -> next;
        b = b -> next;
    }
    if (x != y)
    {
        x = reverse(hold);
        y = reverse(y);
    }
    else
    {
        x = reverse(hold);
        y = x;
    }
    return reverse(carry(result));
}

node *add(node *x, node *y)
{
    node *result = NULL;
    x = reverse(x);
    y = reverse(y);
    node *temp = x;
    node *temp1 = y;
    while(x != NULL || y != NULL)
    {
        node *n = malloc(sizeof(node));
        if(x != NULL && y != NULL)
        {
            n -> x = x -> x + y -> x;
            x = x -> next;
            y = y -> next;
        }
        else if(x != NULL)
        {
            n -> x = x -> x;
            x = x -> next;
        }
        else
        {
            n -> x = y -> x;
            y = y -> next;
        }
        n -> next = result;
        result = n;
    }
    reverse(temp);
    reverse(temp1);
    return reverse(carry(reverse(result)));
}

node *carry(node *list)
{
    int carry = 0;
    node *temp = list;
    while (temp != NULL || carry != 0)
    {
        temp -> x += carry;
        carry = temp -> x / 10;
        temp -> x = temp -> x % 10;
        if(temp -> next == NULL && carry != 0)
        {
            temp -> next = malloc(sizeof(node));
            temp -> next -> x = 0;
            temp -> next -> next = NULL;
        }
        temp = temp -> next;
    }
    
    return list;
}

node *reverse(node *list)
{
    node *prev = NULL;
    node *temp = NULL;
    while (list != NULL)
    {
        temp = list -> next;
        list -> next = prev;
        prev = list;
        list = temp;
    }
    return prev;
}

int len(node *list)
{
    int count = 0;
    while (list != NULL)
    {
        count++;
        list = list -> next;
    }
    return count;
}

void printlist(node *list)
{
    while (list != NULL)
    {
        printf("%i", list -> x);
        list = list -> next;
    }
}
