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
    //introduction
    template <class...t>
    struct MyList;
    //empty list; cant do much
    template <class...t>
    struct MyList{
        //member add
        template<class newelement>
        using add=MyList<newelement>;
        //member 'size'
        static constexpr int size(){ return 0;}
    };
    //normal list
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
    struct size_impl {
        static constexpr int value = std::tuple_size<typename mylist::tuple>::value;
    };
    
    template <class myList>
    static constexpr int size = size_impl<myList>::value;

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
    template<class head, class...tail>
    struct first_n_impl<0,MyList<head,tail...>>{
        typedef  MyList<> type;
    };
    //utility
    template<int i, class mylist>
    using first_n=typename first_n_impl<i,mylist>::type;
    
    
    //drop the first n elements
    //introduction
    template<int i, class mylist>
    struct drop_n_impl;
    //recursive match
    template<int i, class head, class...tail>
    struct drop_n_impl<i,MyList<head,tail...>>{
        typedef typename drop_n_impl< i-1, MyList<tail...> >::type type;
    };
    //terminating match
    template<class head, class...tail>
    struct drop_n_impl<0,MyList<head, tail...>>{
        typedef  MyList<head, tail...> type;
    };
    //terminating match : need to clear up design, if I need two terminators!
    template<>
    struct drop_n_impl<0,MyList<>>{
        typedef  MyList<> type;
    };
    //utility
    template<int i, class mylist>
    using drop_n=typename drop_n_impl<i,mylist>::type;

    //update an element at index i to a new value
    //introduction
    template <class newElement, int i, class mylist>
    struct update_impl;
    //implementation
    template <class newElement, int i, class head, class...tail>
    struct update_impl<newElement, i, MyList<head, tail...>>{
        typedef MyList<head, tail...> thisList;
        //take the list before the index
        typedef first_n<i,thisList> list1;
        //join on the new element
        typedef typename list1::template add<newElement> newList1;
        //take the list after the index
        static constexpr int indexAfter=i+1;
        typedef drop_n<indexAfter,thisList> list2;
        typedef join<newList1,list2> type;
    };
    //utility
    template <class mylist, int i,class newElement>
    using update=typename update_impl<newElement, i, mylist>::type;
    
    //any argument meets condition
    //any
    //introduction
    template <template <class element> class condition, class mylist>
    struct any;
    //recursing match
    template <template <class element> class condition, class head, class... tail>
    struct any<condition, MyList<head, tail...>> {
        static constexpr bool found = condition<head>::value or any<condition, MyList<tail...>>::found;
        static constexpr int idx = condition<head>::value ? 0 : 1 + any<condition, MyList<tail...>>::idx;
    };
    //terminating match
    template <template <class element> class condition, class head>
    struct any<condition, MyList<head>> {
        static constexpr bool found = condition<head>::value;
        static constexpr int idx = found ? 0 : 1;
    };
   
    
    //curry one template parameter
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
    
    
    
    //*************************************************************************************
    //edm specific
    struct no_aux{};
    
    template<typename iobjt,typename ilist_of_feats, typename icont, typename iaux = no_aux>
    struct type_info {
        typedef iobjt object;
        typedef ilist_of_feats list_of_features;
        typedef icont container;
        typedef iaux aux;
    };
    template<class thelist, int index,class new_element>
    struct do_my_add;
    
    template<class thelist,  class O,class F,class... ContAux>
    struct do_my_add<thelist,  -1, type_info<O,F,ContAux...>>{
        typedef typename thelist::template add<type_info<O,F,ContAux...>> result;
    };
    
    template<class thelist, int index,class O,class F,class...ContAux>
    struct do_my_add<thelist,index,type_info<O,F,ContAux...>>{
        typedef at<thelist,index> old_entry;
        typedef typename old_entry::list_of_features old_feats;
        typedef typename type_info<O,F,ContAux...>::list_of_features new_feats;
        typedef join<old_feats, new_feats> extended;
        typedef update<thelist, index,type_info<typename old_entry::object,extended,typename old_entry::container,typename old_entry::aux> > result;
    };


    template<class list_of_types, class newElement>
    struct addWithChecking;
    
    //I don't think this is the same as the original!
    template<class list_of_types,class O,class F,class...contAux>
    struct addWithChecking<list_of_types, type_info<O,F,contAux...> >{
        typedef type_info<O,F,contAux...> edmType;
        typedef list_of_types thisList;
        template<class T>
        using is_this_container=is_same<typename edmType::container> ;
        static constexpr int thisIndex = any<is_this_container,thisList>::idx;
        typedef do_my_add<thisList,thisIndex,edmType> result;
    };


    
    
    
    /** example
     typedef C14::MyList<int, long, char, testY> m;
     std::cout << "["<< typeid(m).name() << "] \n";
     std::cout << "member at(1)["<< typeid(m::at<1>).name() << "] \n";
     std::cout << "generic at(2)["<< typeid(C14::at<m,2>).name() << "] \n";
     std::cout << "member 'size' ["<< (m::size()) << "] \n";
     std::cout << "generic 'size' ["<< (C14::size<m>::value) << "] \n";
     typedef m::add<double> newlist;
     
     C14::for_each<C14::simple_printer, newlist>::do_it();
     using n=m::add<int *>;
     std::cout << "any 'pointer' ["<< (C14::any<std::is_pointer,n>::value) << "] \n";
     
     typedef C14::is_same<int> same_as_int;
     std::cout << "any 'int' ["<< (C14::any<same_as_int::type,n>::value) << "] \n";
     std::cout << "any 'int' ["<< (C14::any<C14::is_int,n>::value) << "] \n";
     **/


}
#endif
