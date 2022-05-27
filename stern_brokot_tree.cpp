#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

typedef struct sbt
{
    int num;
    int denom;
    sbt *left;
    sbt *right;
} sbt;

typedef struct sbf
{
    int left_num;
    int left_denom;
    int right_num;
    int right_denom;
} sbf;

sbt *fill_sbt(int num, int denom, sbt *left, sbt *right)
{
    sbt *tree = (sbt *)malloc(sizeof(sbt));
    tree->num = num;
    tree->denom = denom;
    tree->left = left;
    tree->right = right;
    return tree;
}

sbf *fill_sbf(int l_n, int l_d, int r_n, int r_d)
{
    sbf *st = (sbf *)malloc(sizeof(sbf));
    st->left_num = l_n;
    st->left_denom = l_d;
    st->right_num = r_n;
    st->right_denom = r_d;
    return st;
}

sbt *get_stern_brocot_subtree(int depth, sbf *f)
{
    sbt *node = (sbt *)malloc(sizeof(sbt));
    if (depth != 0)
    {
        node->num = f->left_num + f->right_num;
        node->denom = f->left_denom + f->right_denom;
        // cout << node->num << "/"<< node->denom << " ";
        sbf *left_sbf = fill_sbf(f->left_num, f->left_denom, node->num, node->denom);
        sbf *right_sbf = fill_sbf(node->num, node->denom, f->right_num, f->right_denom);
        node->left = get_stern_brocot_subtree(depth - 1, left_sbf);
        node->right = get_stern_brocot_subtree(depth - 1, right_sbf);
        depth--;
    }
    return node;
}

sbt *get_stern_brocot_tree(int n)
{
    sbt *tree = (sbt *)malloc(sizeof(sbt));
    sbf *start = fill_sbf(0, 1, 1, 0);
    tree = get_stern_brocot_subtree(n, start);
    return tree;
}

void tree_print(sbt *tree)
{
    if (tree != NULL)
    {
        tree_print(tree->left);
        cout << tree->num << "/" << tree->denom << " ";
        tree_print(tree->right);
    }
}
// round(value*100)/100

string sb_number(double num, sbt *tree)
{
    string result = " ";
    if(tree) {
        double cur = (double)tree->num / tree->denom;
        // cout << cur << endl;

        if (num < cur)
            result += "L " + sb_number(num, tree->left);
        else if (num > cur)
            result += "R " + sb_number(num, tree->right);
        if (num == cur)
            return result;
    }
    return result;
}

int main()
{
    // sbf *sbf_st = fill_sbf(1, 1, 1, 0);
    // sbt *sb_subtree = get_stern_brocot_subtree(3, sbf_st);
    // tree_print(sb_subtree);
    sbt *tree = get_stern_brocot_tree(10);
    string way = sb_number(2.718281828, tree);
    cout << way << endl;
    return 0;
}