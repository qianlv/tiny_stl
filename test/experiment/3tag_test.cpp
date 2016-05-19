/*
 * =====================================================================================
 *
 *       Filename:  3tag-test.cpp
 *
 *    Description:  tag types
 *
 *        Version:  1.0
 *        Created:  2016年05月18日 22时32分45秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <iostream>

using namespace std;

struct B {};
struct D1 : public B {};
struct D2 : public D1 {};

template <typename I>
void func(I& p, B)
{
    cout << "B version" << endl;
}

template <typename I>
void func(I& p, D2)
{
    cout << "D2 version" << endl;
}

int main()
{
    int* p;
    func(p, B());
    func(p, D1());
    func(p, D2());
}

