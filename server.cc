#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <grpc/grpc.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include <grpc++/server_context.h>
#include <grpc++/status.h>
#include <grpc++/stream.h>
#include "customer_service.pb.h"

using namespace proto;
using namespace grpc;

class CustomerServiceImpl final : public CustomerService::Service {
private:
  std::vector<Person> customers;
public:
  Status AddPerson(ServerContext* context, const Person* customer, ResponseType* response) {
    std::cout << "AddPerson" << std::endl;
    customers.push_back(*customer);
    std::cout << "Done" << std::endl;
    return Status::OK;
  }

  Status ListPerson(ServerContext* context, const RequestType* request, ServerWriter<Person>* writer) {
    std::cout << "ListPerson" << std::endl;
    for (const Person& p : customers) {
      writer->Write(p);
    }
    std::cout << "Done" << std::endl;
    return Status::OK;
  }
};

int
main(int argc, char* argv[]) {
  grpc_init();
  std::string server_address("0.0.0.0:11111");
  CustomerServiceImpl service;

  ServerBuilder builder;
  builder.AddPort(server_address);
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
  grpc_shutdown();
  return 0;
}
