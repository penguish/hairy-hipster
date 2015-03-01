//
//  main.cpp
//  triggerTypelistParseTest
//
//  Created by sroe on 15/02/2015.
//  Copyright (c) 2015 sroe. All rights reserved.
//

#include <iostream>
#include "TypeInformation.h"
#include "EDM_TypeInformation.h"
#include <boost/type_traits.hpp>
#include <boost/type_traits.hpp>

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
    
    typedef list<testY,nil> single_entry_list;
    std::cout << "single entry" << std::endl;
    std::cout << typeid(single_entry_list::get<0>::type).name() << std::endl;
    
    return 0;
}




