#include <iostream>
#include <string.h>
#include <string>

using namespace std;

struct poly
{
    int k;
    int deg;
    struct poly *next;
};

typedef struct poly poly;

void poly_free(poly *p)
{
    if (p == NULL)
        return;
    poly *temp;
    while (p)
    {
        temp = p;
        p = p->next;
        free(temp);
    }
    return;
}

poly *poly_get_monomial(int k, int deg)
{
    poly *result = (poly *)malloc(sizeof(poly));
    result->k = k;
    result->deg = deg;
    result->next = NULL;
    return result;
}

void poly_merge(poly **p1, poly *p2)
{
    int flag = 0;
    while (p2)
    {
        poly *main = *p1;
        poly *prev = NULL;
        while (main)
        {
            flag = 0;
            if (main->deg == 0 && main->k == 0)
            {
                main->deg = p2->deg;
                main->k = p2->k;
                break;
            }
            if (p2->deg > main->deg)
            {
                poly *temp = poly_get_monomial(p2->k, p2->deg);
                if (main == *p1)
                {
                    temp->next = *p1;
                    *p1 = temp;
                }
                else
                {
                    prev->next = temp;
                    temp->next = main;
                    
                }
                break;
            }
            else if (p2->deg == main->deg)
            {
                main->k += p2->k;
                if (main->k == 0)
                {
                    // main->deg = 0;
                    if (prev)
                    {
                        prev->next = main->next;
                    }
                    else if (main->next)
                    {
                        poly *temp = main;
                        main = main->next;
                        free(temp);
                        prev = main;
                    }
                    flag = 1;
                }
                break;
            }
            prev = main;
            main = main->next;
        }
        if (main == NULL && flag == 0 && prev != NULL)
        {
            poly *temp = poly_get_monomial(p2->k, p2->deg);
            prev->next = temp;
        }
        p2 = p2->next;
    }
    return;
}

poly *poly_get(const char *str)
{
    if (str == NULL)
    {
        return (poly *)NULL;
    }

    int sgn = 1, state = 0, i = 0, num = 0;
    char *end;

    poly *result = poly_get_monomial(0, 0);
    poly *temp_monomial = poly_get_monomial(0, 0);

    while (str[i] != '\0')
    {
        if (str[i] == '-' || str[i] == '+')
        {
            sgn = 1 - 2 * (str[i] == '-');
            i++;
        }
        if (str[i] >= '0' || str[i] <= '9')
        {
                num = strtol(str + i, &end, 10);
                temp_monomial->k = sgn * num;
                i = end - str;
                if (str[i] != 'x')
                {
                    poly_merge(&result, temp_monomial);
                    temp_monomial->k = 0;
                    temp_monomial->deg = 0;
                }
               
        }

        if (str[i] == 'x')
        {
            if (str[i - 1] == '-' || str[i - 1] == '+' || i == 0)
                temp_monomial->k = sgn;
            
            if (str[i + 1] != '^')
                {
                    temp_monomial->deg = 1;
                    poly_merge(&result, temp_monomial);
                    temp_monomial->k = 0;
                    temp_monomial->deg = 0;
                    sgn = 1;
                    i++;
            }
            
            else if (str[i + 1] == '^')
            {
                num = strtol(str + i + 2, &end, 10);
                i = end - str;
                temp_monomial->deg = num;
                poly_merge(&result, temp_monomial);
                temp_monomial->k = 0;
                temp_monomial->deg = 0;
                sgn = 1;
            }
                
            
            else if (str[i + 1] == '-' || str[i + 1] == '+' || str[i + 1] == '\0')
            {
                temp_monomial->deg = 1;
                poly_merge(&result, temp_monomial);
                temp_monomial->k = 0;
                temp_monomial->deg = 0;
                sgn = 1;
                i++;
            }
            else
            {
                cout << "error line";
                return NULL;
            }
            
        }
    }
    return result;
}

void poly_monomial_print(int k, int deg) {
    if (k != 1 && k != -1 && deg > 0)
        cout << k;
    else if (k == -1 && deg > 0)
        cout << "-";
    else if (deg == 0)
        cout <<  k;
    if (deg > 1)
        cout << "x^" << deg;
    else if (deg == 1)
        cout << "x";    
}

void poly_print(const poly *p)
{
    if (p)
    {
        poly_monomial_print(p->k, p->deg);
        p = p->next;
    }
    while (p)
    {
        if (p->k > 0)
            cout << "+";
        poly_monomial_print(p->k, p->deg);
        p = p->next;
    }
    cout << endl;
}

poly* poly_add(poly *p1, poly *p2){
    poly_merge(&p1, p2);
    return p1;
}

poly *poly_multiply(poly *p1, poly *p2){
    poly* result = poly_get_monomial(0, 0);
	poly* temp;
	while (p1) {
		poly* temp = p2;
		while (temp) {
            poly *monom = poly_get_monomial((p1->k * temp->k), (p1->deg + temp->deg));
            poly_merge(&result,monom);
			temp = temp->next;
		}
		p1 = p1->next;
	}
	return result;
}

int main()
{
    poly *p1, *p2, *result;
    const char *str1 = "x^2-3x+x+x^10";
    const char *str2 = "4x^2+3x+x^8";
    p1 = poly_get(str1);
    p2 = poly_get(str2);
    cout << endl;
    //result = poly_add(p1,p2);
    result = poly_multiply(p1, p2);
    poly_print(result);
    return 0;
}