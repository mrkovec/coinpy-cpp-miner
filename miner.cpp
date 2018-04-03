#include "blockproxi.hpp"

#include <iostream>
#include <chrono>

#include "httplib.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <IP_ADDR> <PORT>" << std::endl;
    return -1;
  }

  unsigned int block_count = 0;
  double global_elapsed_seconds = 0;

  httplib::Client cli(argv[1], std::stoi(argv[2]));
  while(true) {
    auto res = cli.get("/");
    if (res && res->status == 200 && res->get_header_value("Content-Type") == "application/json") {
      auto start = std::chrono::system_clock::now();
      cminer::BlockProxi block(res->body);
      auto res_resp = cli.post("/", block.mine(), "application/json");
      if (res_resp && res_resp->status == 200) {
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        ++block_count;
        global_elapsed_seconds = global_elapsed_seconds + elapsed_seconds.count();
        std::cout << "Block mined in " << elapsed_seconds.count() << "s" << " (average block time " << global_elapsed_seconds/block_count << "s)" << std::endl;
      } else {
        std::cerr << "Error: "<< res->status << " - " << res->body << std::endl;
      }
    } else {
      std::cerr << "Error: "<< res->status << " - " << res->get_header_value("Content-Type") << " - " << res->body << std::endl;
    }
  }
  return 0;
}
