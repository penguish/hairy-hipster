//
//  TypeListJap.h
//  triggerTypelistParseTest
//
//  Created by sroe on 21/02/2015.
//  Copyright (c) 2015 sroe. All rights reserved.
//

#ifndef triggerTypelistParseTest_TypeListJap_h
#define triggerTypelistParseTest_TypeListJap_h
#include <tuple>
namespace JAP{
    using std::tuple;
    //individual elements
    template <class Head, class... Tail> struct head {
        typedef Head type;
    };
    template <class Head, class... Tail> struct tail {
        typedef tuple<Tail...> type; };
    
    //with tuples
    template <class Head, class... Tail> struct head<tuple<Head, Tail...>> {
        typedef Head type;
    };
    template <class Head, class... Tail> struct tail<tuple<Head, Tail...>> {
        typedef tuple<Tail...> type;
    };
    
    //length
    template <class... Args>
    struct length {
      static constexpr int value = sizeof...(Args);
    };
    //sroe length
    template <class... Args>
    struct length<tuple<Args...> >{
        static constexpr int value = sizeof...(Args); 
    };
    
    //at
    template <int I, typename Arg>
    struct at;
    template <int I, typename Head, typename... Tail> struct at<I, tuple<Head, Tail...>> {
        typedef typename at<I - 1, tuple<Tail...>>::type type; };
    template <class Head, typename... Tail> struct at<0, tuple<Head, Tail...>> {
        typedef Head type;
    };
    //at2
    template <int I, typename Arg>
    struct at2;

    template <int I, class...Args>
    struct at2<I, tuple<Args...> >{
        typedef tuple<Args...> t;
        typedef typename std::tuple_element<I,t>::type type;
        
    };
    
    //concat
    template <class Seq1, class Seq2>
    struct concat{};
    template <class... Seq1, class... Seq2>
    struct concat<tuple<Seq1...>, tuple<Seq2...>> {
        typedef tuple<Seq1..., Seq2...> type;
    };
    
    //replicate
    template <int N, class T>
    struct replicate {
        typedef typename concat<typename replicate<N-1, T>::type,tuple<T>>::type type;
    };
    template <class T> struct replicate<0, T> {
        typedef tuple<> type;
    };
    
    //take
    template <int N, class Seq>
    struct take{};
    template <int N, class Head, class... Tail>
    struct take<N, tuple<Head, Tail...>> {
        typedef typename concat<tuple<Head>,typename take<N-1, tuple<Tail...>>::type>::type type;
    };
    template <class Head, class... Tail>
    struct take<1, tuple<Head, Tail...>> {
        typedef tuple<Head> type;
    };
    template <class Head, class... Tail>
    struct take<0, tuple<Head, Tail...>> {
        typedef tuple<> type;
    };
    
    //map
    template <template <class T> class F, class... Seq>
    struct map{};
    template <template <class T> class F, class... Seq>
    struct map<F, tuple<Seq...>> {
        typedef tuple<typename F<Seq>::type...> type;
    };
    //e.g. add pointer
    template <class T>
    struct add_pointer {
        typedef T* type;
    };
    
    //filter
    template <bool B, class T, class Seq> struct add_if_c{};
    template <class T, class... Seq>
    struct add_if_c<true, T, tuple<Seq...>> {
        typedef tuple<Seq..., T> type;
    };
    
    template <class T, class... Seq>
    struct add_if_c<false, T, tuple<Seq...>> {
        typedef tuple<Seq...> type;
    };
    template <template <class> class P, class T, class... Seq> struct add_if{};
    template <template <class> class P, class T, class... Seq> struct add_if<P, T, tuple<Seq...>> :
    public add_if_c<P<T>::value, T, tuple<Seq...>> {};
    
    //filter2
    template <template <class T> class P, class Seq1, class Seq2>
    struct filter_impl;
    
    template <template <class T> class P, class... Seq, class Head, class... Tail>
    struct filter_impl<P, std::tuple<Seq...>, tuple<Head, Tail...>> {
       typedef typename filter_impl<P,typename add_if< P ,Head,tuple<Seq...>>::type,tuple<Tail...>>::type type;
    };
    
    template <template <class T> class P, class... Seq>
    struct filter_impl<P, tuple<Seq...>, tuple<>> {
        typedef tuple<Seq...> type;
    };
    
    template <template <class T> class P, class... Seq>
    struct filter;
    template <template <class T> class P, class... Seq>
    struct filter<P, tuple<Seq...>> {
        typedef typename filter_impl<P, tuple<>, tuple<Seq...>>::type type;
    };
    //any
    template <template <class T> class P, class Seq>
    struct any;
    template <template <class T> class P, class Head, class... Tail> struct any<P, tuple<Head, Tail...>> {
        static const bool value = P<Head>::value ?true :any<P, tuple<Tail...>>::value;
    };
    template <template <class T> class P> struct any<P, tuple<>> {
        static const bool value = false;
    };
    
   
    template <class f, class element, class... Args>
    struct for_each{
        typedef for_each<f, Args...> recurse;
        typedef for_each<f, element> result;
        static void do_it()  {
            result::do_it();
            recurse::do_it();
        }
    };
    
    template <class element, class...t>
    struct mylist{
        typedef element Head;
        typedef mylist<t...> Tail;
    };
    

    
    template <class f, class element, class... Args>
    struct for_each<f,mylist<element, Args...>>{
        typedef for_each<f, Args...> recurse;
        typedef for_each<f, element> result;
        static void do_it()  {
            result::do_it();
            recurse::do_it();
        }
    };

    template <class f, class element>
    struct for_each<f, mylist<element>>{
        void static do_it()  {f::template do_it<element>();}
    };

    
    template <class f, class element>
    struct for_each<f, element>{
        void static do_it()  {f::template do_it<element>();}
    };
    
  

    template <class element, class... Args>
    struct printer{
        typedef printer<Args...> recurse;
        typedef printer<element> result;
        static void do_it()  {
            result::do_it();
            recurse::do_it();
        }
    };
    
    
    template <class element>
    struct printer<element>{
        void static do_it()  {std::cout << "["<< typeid(element).name() << "] ";}
    };
    
    struct simple_printer{
        template <class element>
        void static do_it() {std::cout << "["<< typeid(element).name() << "] ";}
    };
    
}
#endif

