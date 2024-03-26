#include <chrono>
#include <iostream>
#include <thread>

#include "../src/context/context.h"

int main(int argc, char** argv) {
    buried::Context::GetGlobalContext().Start();

    buried::Context::GetGlobalContext().GetMainStrand().post([]() {
        std::cout << "Operation 1 executed in main_strand on thread id: "
                << std::this_thread::get_id() << std::endl;
    });

    // buried::Context::GetGlobalContext().GetMainStrand().post([](){
    //     std::cout << "aaaaa" << std::endl;
    // });

    buried::Context::GetGlobalContext().GetReportStrand().post([]() {
        std::cout << "Operation 2 executed in report_strand on thread id: "
                << std::this_thread::get_id() << std::endl;
    });

    buried::Context::GetGlobalContext().GetReportStrand().post([]() {
        std::cout << "Operation 3 executed in report_strand on thread id: "
                << std::this_thread::get_id() << std::endl;
    });

    buried::Context::GetGlobalContext().GetMainStrand().post([]() {
        std::cout << "Operation 4 executed in main_strand on thread id: "
                << std::this_thread::get_id() << std::endl;
    });

    buried::Context::GetGlobalContext().GetReportStrand().post([]() {
        std::cout << "Operation 5 executed in report_strand on thread id: "
                << std::this_thread::get_id() << std::endl;
    });

    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
