#include <iostream>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

typedef struct list
{
    int num;
    list *next;
} list;

typedef struct adj_node
{
    int num;
    list *edge_list;
    adj_node *next;
} adj_node;

typedef struct graph
{
    int num;
    adj_node *adj_list;
} graph;

list *create_node(int num, list *next)
{
    list *result = (list *)malloc(sizeof(list));
    result->num = num;
    result->next = next;
    return result;
}

adj_node *create_adj_node(int num, list *edge = NULL, adj_node *next = NULL)
{
    adj_node *result = (adj_node *)malloc(sizeof(adj_node));
    result->num = num;
    result->edge_list = edge;
    result->next = next;
    return result;
}

graph *graph_init(int n)
{
    graph *result = (graph *)malloc(sizeof(graph));
    if (result)
    {
        result->num = n;
        adj_node *adj_list = create_adj_node(0);
        result->adj_list = adj_list;
        for (int i = 1; i < n; i++)
        {
            adj_node *t = create_adj_node(i);
            adj_list->next = t;
            adj_list = adj_list->next;
        }
    }
    return result;
}

void add_edge(graph *g, int a, int b)
{
    adj_node *i = g->adj_list;
    while (i->num != a)
    {
        i = i->next;
    }
    list *ptr = i->edge_list;
    while (ptr && ptr->next)
    {
        if (ptr->num == b)
            return;
        ptr = ptr->next;
    }
    list *t = create_node(b, NULL);
    if (!ptr)
    {
        i->edge_list = t;
    }
    else
    {
        ptr->next = t;
    }
    return;
}

void del_edge(graph *g, int a, int b)
{
    adj_node *i = g->adj_list;
    while (i->num != a)
    {
        i = i->next;
    }
    list *ptr = i->edge_list, *prev = NULL;
    while (ptr->num != b && ptr)
    {
        prev = ptr;
        ptr = ptr->next;
    }
    if (!ptr)
    {
        cout << "no edje" << a << " - " << b << endl;
    }
    else if (ptr == i->edge_list)
    {
        i->edge_list = ptr->next;
        free(ptr);
    }
    else
    {
        prev->next = ptr->next;
        free(ptr);
    }
    return;
}

void graph_print(graph *g)
{
    adj_node *adj_ptr = g->adj_list;
    for (int i = 0; i < g->num; i++)
    {
        cout << i << ": ";
        list *arc_ptr = adj_ptr->edge_list;
        while (arc_ptr)
        {
            cout << arc_ptr->num << " ";
            arc_ptr = arc_ptr->next;
        }
        cout << endl;
        adj_ptr = adj_ptr->next;
    }
    return;
}

void graph_free(graph *g)
{
    adj_node *adj_ptr = g->adj_list;
    while (adj_ptr)
    {
        list *arc_ptr = adj_ptr->edge_list;
        while (arc_ptr)
        {
            list *t = arc_ptr;
            arc_ptr = arc_ptr->next;
            free(t);
        }
        adj_node *t = adj_ptr;
        adj_ptr = adj_ptr->next;
        free(t);
    }
    free(g);
    return;
}

int color_inverse(int color)
{
    if (color == 1)
        return 2;
    else
        return 1;
}

adj_node *find_ver(int num, graph *g)
{
    adj_node *result = (adj_node *)malloc(sizeof(adj_node));
    adj_node *ptr = g->adj_list;
    while (num != ptr->num)
        ptr = ptr->next;
    return ptr;
}

int *bpt_check_dfs(graph *g)
{
    stack<int> ver;
    int *colors = (int *)calloc(g->num, sizeof(int));
    bool visited[g->num];

    int root_color, adj_color;
    adj_node *temp = g->adj_list;

    colors[temp->num] = root_color = 1;

    visited[temp->num] = true;
    
    ver.push(0);

    while (!ver.empty())
    {
        list *ptr = temp->edge_list;
        while (ptr)
        {
            if (visited[ptr->num] == false)
            {
                ver.push(ptr->num);
            }
            ptr = ptr->next;
        }
        adj_color = color_inverse(root_color);
        if (adj_color == root_color)
        {
            free(colors);
            return NULL;
        }
        else
        {
            colors[ver.top()] = adj_color;
            visited[ver.top()] = true;

            root_color = adj_color;
        }
        temp = find_ver(ver.top(), g);

        ver.pop();
        if (ver.empty())
        {
            for (int i = 0; i < g->num; i++)
            {
                if (visited[i] == false)
                {
                    temp = find_ver(i, g);
                    colors[temp->num] = root_color = 1;
                    visited[temp->num] = true;
                    ver.push(temp->num);
                    continue;
                }
            }
        }
    }

    return colors;
}

int *bpt_check_dfs_1(graph *g)
{
    stack<int> ver;
    int *colors = (int *)calloc(g->num, sizeof(int));
    bool visited[g->num];

    int root_color, adj_color;

    for (int i = 0; i < g->num; i++)
    {
        adj_node *temp;
        if (visited[i] == false)
        {
            temp = find_ver(i, g);
            colors[temp->num] = root_color = 1;
            visited[temp->num] = true;
            ver.push(temp->num);
        }
        while (!ver.empty())
        {
            list *ptr = temp->edge_list;
            while (ptr)
            {
                if (visited[ptr->num] == false)
                {
                    ver.push(ptr->num);
                }
                ptr = ptr->next;
            }
            adj_color = color_inverse(root_color);
            if (adj_color == root_color)
            {
                free(colors);
                return NULL;
            }
            else
            {
                colors[ver.top()] = adj_color;
                visited[ver.top()] = true;

                root_color = adj_color;
            }
            temp = find_ver(ver.top(), g);
            ver.pop();
        }
    }
    return colors;
}

int *bpt_check_bfs(graph *g)
{
    queue<int> ver;
    int *colors = (int *)calloc(g->num, sizeof(int));
    bool visited[g->num];
    
    adj_node *temp = g->adj_list;
    colors[temp->num]= 1;
    ver.push(0);
    while(!ver.empty()){
        list *ptr = temp->edge_list;
        while(ptr){
            if(visited[ptr->num] == false){
                ver.push(ptr->num);
                colors[ptr->num] = color_inverse(colors[temp->num]);
            }
            ptr = ptr->next;
        }
        visited[temp->num] = true;
        temp = find_ver(ver.front(), g);
        ver.pop();
        if (ver.empty())
        {
            cout << "hiii" << endl;
            for (int i = 0; i < g->num; i++)
            {
                if (visited[i] == false)
                {
                    temp = find_ver(i, g);
                    colors[i] = 1;
                    ver.push(i);
                    continue;
                }
            }
        }
        cout << endl;
        }
    return colors;
}

void top_sort_help(int num, bool visited[], stack<int> &ver, graph* g)
{
    visited[num] = true;
    adj_node *temp = find_ver(num, g);
    list *ptr = temp->edge_list;
    while (ptr)
    {
        if (!visited[ptr->num])
            top_sort_help(ptr->num, visited, ver, g);
        ptr = ptr->next;
    }
    ver.push(num);
}

int *top_sort(graph *g)
{
    int* res = new int[g->num];
    stack<int> ver;

    bool visited[g->num];
    for (int i = 0; i < g->num; i++)
        visited[i] = false;

    for (int i = 0; i < g->num; i++)
        if (visited[i] == false)
            top_sort_help(i, visited, ver, g);

    int i = 0;
    while (!ver.empty())
    {
        res[i] = ver.top();
        ver.pop();
        i++;
    }
    return res;
}

int main()
{

///////////// bfs & dfs check ////////////
/*  
    int n = 6;
    graph *g = graph_init(n);
    add_edge(g, 2, 5);
    add_edge(g, 5, 2);
    add_edge(g, 0, 3);
    add_edge(g, 3, 0);
    add_edge(g, 0, 4);
    add_edge(g, 4, 0);
    add_edge(g, 1, 4);
    add_edge(g, 4, 1);
    add_edge(g, 2, 3);
    add_edge(g, 3, 2);
*/

////////////// topology sort //////////////////
/* 
    int n = 6;
    graph *g = graph_init(n);
    add_edge(g, 5, 0);
    add_edge(g, 5, 2);
    add_edge(g, 4, 0);
    add_edge(g, 4, 1);
    add_edge(g, 2, 3);
    add_edge(g, 3, 1);
*/

int n = 4;
graph *g = graph_init(n);
add_edge(g, 0, 3);
add_edge(g, 3, 1);
add_edge(g, 3, 2);
add_edge(g, 2, 1);

int* res = top_sort(g);
for(int i = 0; i < g->num; i++){
    cout << res[i] << " ";
}
/////// graph print ///////////
/*
   graph_print(g);
   int *result = bpt_check_bfs(g);
   //int *result = bpt_check_bfs(g);

   cout << "red: " << endl;
   for (int i = 0; i < n; i++)
   {
       if (result[i] == 1)
           cout << i << endl;
   }
   cout << "blue: " << endl;
   for (int i = 0; i < n; i++)
   {
       if (result[i] == 2)
           cout << i << endl;
   }
   cout << endl;
*/

return 0;
}