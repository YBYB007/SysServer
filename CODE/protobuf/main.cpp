#include<iostream>
#include"person.pb.h"

void test()
{
    person p;
    p.set_age(12);
    p.set_name("sb");
    p.set_id(123);
    
    std::string output;
    p.SerializeToString(&output);
    std::cout<<output<<std::endl;

    person pp;
    pp.ParseFromString(output);
    std::cout<<pp.id()<<" "<<pp.age()<<" "<<pp.name()<<std::endl;
}

int main(int argc, char const *argv[])
{
    /* code */
    test();
    return 0;
}
