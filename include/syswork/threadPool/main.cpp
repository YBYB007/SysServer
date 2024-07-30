#include <iostream>
#include "threadPool.cpp"

int test(int i)
{
    std::cout << "我是" << i << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return i * i;
}

int main(int argc, char const *argv[])
{
    /* code */
    ThreadPool pool(4);
    // 结果集

    std::future<int> result1=pool.enqueue(test,19);
    std::future<int> result2=pool.enqueue(test,12);
    std::future<int> result3=pool.enqueue(test,11);
    std::future<int> result4=pool.enqueue(test,10);
    std::cout<<"main";
    std::cout<<result1.get()<<std::endl;
    std::cout<<result2.get()<<std::endl;
    std::cout<<result3.get()<<std::endl;
    std::cout<<result4.get()<<std::endl;


    // std::vector<std::future<int>> results;
    // for (int i = 0; i < 8; ++i)
    // {
    //     results.emplace_back(
    //         pool.enqueue([i]
    //         {
    //             std::cout << "world " << i << std::endl;
    //             return i*i; }));
    // }
    // for (auto &&result : results)
    //     std::cout << result.get() << ' ';
    // std::cout << std::endl;

    return 0;
}
