/*
 * =====================================================================================
 *
 *       Filename:  type_traits.h
 *
 *    Description:  type traits
 *
 *        Version:  1.0
 *        Created:  2016年05月18日 20时51分18秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  qianlv (), qianlv7 at gmail dot com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef TINY_STL_TYPE_TRAITS_H_
#define TINY_STL_TYPE_TRAITS_H_ 

namespace tinystl
{
    struct __true_type { };
    struct __false_type { };

    template <typename Type>
    struct __type_traits
    {
        typedef __true_type     this_dummy_member_must_be_first;
        typedef __false_type    has_trivial_default_constructor;
        typedef __false_type    has_trivial_copy_constructor;
        typedef __false_type    has_trivial_assigment_operator;
        typedef __false_type    has_trivial_destructor;
        typedef __false_type    is_POD_type;
    };

    template <>
    struct __type_traits<bool>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<char>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<unsigned char>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<wchar_t>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<short>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<unsigned short>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<int>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<unsigned int>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<long>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<unsigned long>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<long long>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<unsigned long long>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<float>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<double>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <>
    struct __type_traits<long double>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <typename T>
    struct __type_traits<T*>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };

    template <typename T>
    struct __type_traits<const T*>
    {
        typedef __true_type    has_trivial_default_constructor;
        typedef __true_type    has_trivial_copy_constructor;
        typedef __true_type    has_trivial_assigment_operator;
        typedef __true_type    has_trivial_destructor;
        typedef __true_type    is_POD_type;
    };


} // end namespace tinystl

#endif // TINY_STL_TYPE_TRAITS_H_
