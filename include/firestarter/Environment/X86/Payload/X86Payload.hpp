#ifndef INCLUDE_FIRESTARTER_ENVIRONMENT_X86_PAYLOAD_PAYLOAD_H
#define INCLUDE_FIRESTARTER_ENVIRONMENT_X86_PAYLOAD_PAYLOAD_H

#include <firestarter/Environment/Payload/Payload.hpp>
#include <firestarter/Logging/Log.hpp>

namespace firestarter::environment::x86::payload {

	class X86Payload : public environment::payload::Payload {
		private:
			// we can use this to check, if our platform support this payload
			llvm::StringMap<bool> *_supportedFeatures;
			std::list<std::string> _featureRequests;
	
		public:
			X86Payload(llvm::StringMap<bool> *supportedFeatures, std::initializer_list<std::string> featureRequests, std::string name) :
				Payload(name), _supportedFeatures(supportedFeatures), _featureRequests(featureRequests) {};
			~X86Payload() {};

			bool isAvailable(void) override {
				bool available = true;

				for (std::string feature : _featureRequests) {
					available &= (*_supportedFeatures)[feature];
				}

				return available;
			};

			// A generic implemenation for all x86 payloads
			// use cpuid and usleep as low load
			void lowLoadFunction(...) override;
	};

}

#endif