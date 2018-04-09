#include "blockproxi.hpp"

#include <iostream>
#include <chrono>

#include "httplib.h"

int main(int argc, char* argv[]) {
  const char* host = "127.0.0.1";
  int port = 2020;
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <IP_ADDR> <PORT>" << std::endl;
    std::cerr << "Using defaul parameters" << std::endl;
  } else {
    host = argv[1];
    port = std::stoi(argv[2]);
  }
  std::cerr << "Starting miner with parameters " << host << " " << port << std::endl;

  unsigned int block_count = 0;
  double global_elapsed_seconds = 0;

  httplib::Client cli(host, port, 5);
  while(true) {
    auto res = cli.get("/");
    if (res && res->status == 200 && res->get_header_value("Content-Type") == "application/json") {
      std::cout << "New block to mine" << std::endl;
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
