#include <iostream>
#include <string>
#include <thread>
#include <grpc++/grpc++.h>
#include "calculator.grpc.pb.h"
using namespace std;

class CalculatorServiceImpl final : public GrpcSample::Calculator::Service {
    ::grpc::Status Add(::grpc::ServerContext* context, const ::GrpcSample::AddReq* request, ::GrpcSample::AddResp* response) override {
	    int lhs = request->lhs();
		int rhs = request->rhs();
		std::cout<<"Getting request to calculat "<<lhs<<" + "<<rhs<<std::endl;
		std::cout<<"Wait for 10 seconds"<<endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));

		if (context->IsCancelled()) {
			std::cout<<"Call is cancelled"<<endl;
			return ::grpc::Status::CANCELLED;
		}
		int result = lhs + rhs;
		response->set_result(result);
		return ::grpc::Status::OK;
	}
};

int main()
{
	CalculatorServiceImpl service;
	::grpc::ServerBuilder builder;
	builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
	builder.RegisterService(&service);
	std::unique_ptr<::grpc::Server> server(builder.BuildAndStart());
	std::cout<<"Calculator server is running..."<<std::endl;
	server->Wait();
}
