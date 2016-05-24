/*
 * =====================================================================================
 *
 *       Filename:  test_header.h
 *
 *    Description:  header
 *
 *        Version:  1.0
 *        Created:  2016年05月23日 21时28分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

class A
{
public:
    A(int v = 0): value(v) {};
    ~A() 
    {
//        cout << "~A()" << endl;
    };

    bool operator==(const A& a) const
    {
        return value == a.value;
    }

    bool operator!=(const A& a) const
    {
        return !(*this == a);
    }

    bool operator<(const A& a) const
    {
        return value < a.value;
    }

    bool operator<=(const A& a) const
    {
        return value <= a.value;
    }

    bool operator>(const A& a) const
    {
        return value > a.value;
    }

    bool operator>=(const A& a) const
    {
        return value >= a.value;
    }

    int value;
    // int value1;
    // int value2;
    // int value3;
    // int value4;
    // int value5;
    // int value6;
    // int value7;
    // int value8;
    // int value9;
    // int value10;
    // int value11;
    // int value12;
    // int value13;
    // int value14;
    // int value15;
    // int value90;
    // int value91;
    // int value92;
    // int value93;
    // int value94;
    // int value95;
    // int value96;
    // int value97;
    // int value99;
    // int value98;
    // int value910;
    // int value911;
    // int value912;
    // int value913;
};
