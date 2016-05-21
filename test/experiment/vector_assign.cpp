/*
 * =====================================================================================
 *
 *       Filename:  vector_assign.cpp
 *
 *    Description:  assign dtor
 *
 *        Version:  1.0
 *        Created:  2016年05月21日 16时49分18秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>

using namespace std;

class A
{
public:
    A(int v): value(v) {};
    ~A() 
    {
        cout << "~A()" << endl;
    };
    int value;
};

int main()
{
    vector<A> v;
    cout << "0000000000" << endl;
    A a(10);
    v.assign(5, a);
    cout << "0000000000" << endl;
    v.assign(1, a);
    cout << "0000000000" << endl;
}
