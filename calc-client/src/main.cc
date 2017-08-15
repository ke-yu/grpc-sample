#include <iostream>
#include <string>
#include <thread>
#include <grpc++/grpc++.h>
#include "calculator.grpc.pb.h"
using namespace std;

class CalculatorClient {
public:
	CalculatorClient(std::shared_ptr<::grpc::Channel> channel): stub_(GrpcSample::Calculator::NewStub(channel)) {
	}
	
	int Add(int lhs, int rhs) {
		GrpcSample::AddReq request;
		request.set_lhs(lhs);
		request.set_rhs(rhs);

		GrpcSample::AddResp response;
		::grpc::ClientContext context;
		
		unsigned client_connection_timeout = 5;
		std::chrono::system_clock::time_point deadline =
		    std::chrono::system_clock::now() + std::chrono::seconds(client_connection_timeout);

		context.set_deadline(deadline);

		::grpc::Status status = stub_->Add(&context, request, &response);
		if (status.ok()) {
			return response.result();
		}
		else {
			std::cout<<status.error_code()<<" : "<<status.error_message()<<std::endl;
			return -1;
		}
	}
private:
	std::unique_ptr<GrpcSample::Calculator::Stub> stub_;
};

int main()
{
	CalculatorClient calc(grpc::CreateChannel("localhost:50051", ::grpc::InsecureChannelCredentials()));
	while (true) {
		int lhs, rhs;
		std::cout<<"Input two numbers to do addition, two 0 to exit: ";
		std::cin>>lhs>>rhs;

		if (lhs == 0 && rhs == 0) {
			break;
		}

		std::cout<<"Calling grpc...";
		int result = calc.Add(lhs, rhs);
		std::cout<<" = "<<result<<endl;
	}
	return 0;
}
