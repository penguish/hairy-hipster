//
//  main.cpp
//  triggerTypelistParseTest
//
//  Created by sroe on 15/02/2015.
//  Copyright (c) 2015 sroe. All rights reserved.
//

#include <iostream>
#include <tuple>
#include <vector>
#include "TypeInformation.h"
#include "TypeList14.h"
#include "TypeListJap.h"

#include "EDM_TypeInformation.h"


using namespace HLT::TypeInformation;
struct testX{};
struct testY{};
struct testZ{};
struct testX1{};
struct testY1{};
struct testZ1{};
int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    HLT_BEGIN_TYPE_REGISTRATION
    HLT_REGISTER_TYPE(testX,testY,testZ)
    addWithChecking<HLT::TypeInformation::type_info<testX1 ,HLT::TypeInformation::list<testY1 ,HLT::\
    TypeInformation::nil>, testZ1, testX> > ::go::
    HLT_REGISTER_TYPE(testX1,testY1,testZ1)

    HLT_END_TYPE_REGISTRATION(test3)
    std::cout<<"Using macros"<<std::endl;
    
    std::cout << typeid(at<TypeInfo_test3,0>::type).name() << std::endl;
    std::cout << typeid(at<TypeInfo_test3,1>::type).name() << std::endl;
    std::cout << typeid(TypeInfo_test3::get<0>::type).name() << std::endl;

    std::cout << "Basic_test" << std::endl;
    typedef testX pX;
    typedef newlist::add<testX>::go::add<testY>::go::done mylist;
    std::cout << "with newlist" << std::endl;
    std::cout << typeid(at<mylist,0>::type).name() << std::endl;
    std::cout << typeid(at<mylist,1>::type).name() << std::endl;
    std::cout << typeid(mylist::get<0>::type).name() << std::endl;
    std::cout << typeid(mylist::get<1>::type).name() << std::endl;
    
    typedef list<testX,list<testY,nil> > manual_list;
    std::cout << "manual list" << std::endl;
    std::cout << typeid(at<manual_list,0>::type).name() << std::endl;
    std::cout << typeid(at<manual_list,1>::type).name() << std::endl;
    std::cout << typeid(manual_list::get<0>::type).name() << std::endl;
    std::cout << typeid(manual_list::get<1>::type).name() << std::endl;
    typedef do_update<manual_list, int, 1>::result newlist1;
    std::cout << "after update " <<typeid(newlist1::get<1>::type).name() << std::endl;
    
    typedef list<testY,nil> single_entry_list;
    std::cout << "single entry" << std::endl;
    std::cout << typeid(single_entry_list::get<0>::type).name() << std::endl;
    
    //
    for_each_type<manual_list,simple_printer> g;
    g.do_it();
    std::cout << "\nJapanese" << std::endl;
    typedef std::tuple<int,testX,testY> MyList;
    JAP::printer<int, char*, long> myprint;
    myprint.do_it();
    std::cout << "length = " << JAP::length<MyList>::value<<std::endl;
    JAP::for_each<JAP::simple_printer, int, char*, long> myprint2;
    myprint2.do_it();
    std::cout<<"\nC14: "<<std::endl;
    typedef C14::MyList<int, long, char, testY> m;
    std::cout << "["<< typeid(m).name() << "] \n";
    std::cout << "member at(1)["<< typeid(m::at<1>).name() << "] \n";
    std::cout << "generic at(2)["<< typeid(C14::at<m,2>).name() << "] \n";
    std::cout << "member 'size' ["<< (m::size()) << "] \n";
    std::cout << "generic 'size' ["<< (C14::size<m>::value) << "] \n";
    typedef m::add<double> newlist;
    
    C14::for_each<C14::simple_printer, newlist> myprint3;
    myprint3.do_it();
    std::cout<<std::endl;
    C14::accumulate<C14::add,C14::counter, newlist> myprint4;
    std::cout<<myprint4.do_it()<<std::endl;
    C14::accumulate<C14::orElements,C14::findType<float>,newlist> myprint5;
    std::cout<<std::boolalpha<<myprint5.do_it()<<std::endl;
    typedef C14::join<m,m> j;
    C14::for_each<C14::simple_printer, j> myprint6;
    myprint6.do_it();
    std::cout<<std::endl;


    std::cout << "any 'pointer' ["<< (C14::any<std::is_pointer,m>::value) << "] \n";
    using n=m::add<int *>;
    std::cout << "any 'pointer' ["<< (C14::any<std::is_pointer,n>::value) << "] \n";

    typedef C14::is_same<int> same_as_int;
    std::cout << "any 'int' ["<< (C14::any<same_as_int::type,n>::value) << "] \n";
    std::cout << "any 'int' ["<< (C14::any<C14::is_int,n>::value) << "] \n";
    
    C14::for_each<C14::simple_printer, n> list1;
    list1.do_it();
    std::cout<<std::endl;

    typedef C14::first_n<2, n> twolist;
    C14::for_each<C14::simple_printer, twolist> list2;
    list2.do_it();
    
    std::cout<<"\n3"<<std::endl;
    typedef C14::drop_n<2, n> three;
    C14::for_each<C14::simple_printer, three> list3;
    list3.do_it();
    
    std::cout<<"\n4"<<std::endl;
    typedef C14::update<double,4,n> four;
    C14::for_each<C14::simple_printer, n>::do_it();
    std::cout<<std::endl;
    C14::for_each<C14::simple_printer, four>::do_it();

    std::cout<<std::endl;
    
    return 0;
}




