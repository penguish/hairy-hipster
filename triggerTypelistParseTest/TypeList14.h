//
//  TypeList14.h
//  triggerTypelistParseTest
//
//  Created by sroe on 19/02/2015.
//  Copyright (c) 2015 sroe. All rights reserved.
//

#ifndef triggerTypelistParseTest_TypeList14_h
#define triggerTypelistParseTest_TypeList14_h
#include <tuple>
namespace C14{
    
    //a simple typelist with head and tail defined
    template <class...t>
    struct MyList;
    
    template <class element,class...t>
    struct MyList<element, t...>{
        typedef element head;
        typedef MyList<t...> tail;
        typedef std::tuple<element, t...> tuple;
        //member 'at'
        template<int i>
        using at=typename std::tuple_element<i,tuple>::type;
        //member 'size'
        static constexpr int size(){return std::tuple_size<tuple>::value;}
        //member add
        template<class newelement>
        using add=MyList<element, t..., newelement>;
    };
    
    //'join' implementation
    template <class Seq1, class Seq2>
    struct join_impl{};
    template <class... Seq1, class... Seq2>
    struct join_impl<MyList<Seq1...>, MyList<Seq2...>> {
        typedef MyList<Seq1..., Seq2...> type;
    };
    
    //'join' generic function for two lists
    template<class mylist1, class mylist2>
    using join = typename join_impl<mylist1,mylist2>::type;
    
    //implement generic 'at' (index)
    template <class mylist, int i>
    using at=typename std::tuple_element<i,typename mylist::tuple>::type;
    
    //implement generic 'size'
    template <class mylist>
    struct size {
        static constexpr int value = std::tuple_size<typename mylist::tuple>::value;
    };
    
    //first n elements, first_n
    //introduction
    template<int i, class mylist>
    struct first_n_impl;
    //recursing match
    template<int i, class head, class...tail>
    struct first_n_impl<i,MyList<head,tail...>>{
        typedef MyList<head> list1;
        typedef MyList<tail...> list2;
        typedef join<list1, typename first_n_impl< i-1, list2 >::type > type;
    };
    //terminating match
    template<class head, class...tail>
    struct first_n_impl<1,MyList<head,tail...>>{
        typedef  MyList<head> type;
    };
    //utility
    template<int i, class mylist>
    using first_n=typename first_n_impl<i,mylist>::type;
    
    //any argument meets condition
    //any
    //introduction
    template <template <class element> class condition, class mylist>
    struct any;
    //recursing match
    template <template <class element> class condition, class head, class... tail>
    struct any<condition, MyList<head, tail...>> {
        static const bool value = condition<head>::value or any<condition, MyList<tail...>>::value;
    };
    //terminating match
    template <template <class element> class condition, class head>
    struct any<condition, MyList<head>> {
        static const bool value = condition<head>::value;
    };
    
    template <class T>
    struct is_same{
        template <class S>
        using type = std::is_same<T,S>;
        template<class S>
        static constexpr bool value = type<S>::value;
    };
    
    //'for_each'-style algorithms
    //introduction
    template <class functor, class element, class... Args>
    struct for_each;
    //recursing match
    template <class functor, class element, class... Args>
    struct for_each<functor,MyList<element, Args...>>{
        typedef for_each<functor, MyList<Args...> > recurse;
        typedef for_each<functor, MyList<element> > result;
        static void do_it()  {
            result::do_it();
            recurse::do_it();
        }
    };
    //terminating match
    template <class functor, class element>
    struct for_each<functor, MyList<element>>{
        void static do_it()  {functor::template do_it<element>();}
    };
    
    
    //'accumulate'-style algorithms, taking a binary operand on functions f
    //applied to the arguments
    //introduction
    template <class binaryOp , class f, class element, class... Args>
    struct accumulate;
    //recursing match
    template <class binaryOp, class f, class element, class... Args>
    struct accumulate< binaryOp, f, MyList<element, Args...>>{
        typedef accumulate<binaryOp, f, MyList<Args...> > recurse;
        typedef accumulate<binaryOp, f, MyList<element> > result;
        static typename binaryOp::type do_it()  {
            return binaryOp::exec(result::do_it(),
            recurse::do_it());
        }
    };
    //terminating match
    template <class binaryOp, class f, class element>
    struct accumulate<binaryOp, f, MyList<element>>{
        int static do_it()  {return f::template do_it<element>();}
    };
    
    //a simple counting 'functor'
    struct counter{
        typedef int type;
        template <class element>
        type static do_it() {return 1;}
    };
    
    // add, should template this on the functor?
    struct add{
        typedef int type;
        type static exec(type a, type b){return a+b;}
    };
    
    //a simple or-ing 'functor'
    template<class searchType>
    struct findType{
        typedef bool type;
        template <class element>
        type static do_it() {return std::is_same<searchType, element>::value;}
    };
    
    // or, should template this on the functor?
    struct orElements{
        typedef bool type;
        type static exec(type a, type b){return a or b;}
    };

    
    //a simple printing 'functor'
    struct simple_printer{
        template <class element>
        void static do_it() {std::cout << "["<< typeid(element).name() << "] ";}
    };
    // why doesnt this work in 'main' ??
    template <typename S>
    using is_int = std::is_same<int, S>;
    
    ///edm specific
    struct no_aux{};
    
    template<typename _objt,typename _list_of_feats, typename _cont, typename _aux = no_aux>
    struct type_info {
        typedef _objt object;
        typedef _list_of_feats list_of_features;
        typedef _cont container;
        typedef _aux aux;
    };

}
#endif
