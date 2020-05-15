#ifndef INCLUDE_FIRESTARTER_ENVIRONMENT_X86_PLATFORM_SKYLAKECONFIG_H
#define INCLUDE_FIRESTARTER_ENVIRONMENT_X86_PLATFORM_SKYLAKECONFIG_H

#include <firestarter/Environment/X86/Platform/PlatformConfig.hpp>
#include <firestarter/Environment/X86/Payload/FMAPayload.hpp>

namespace firestarter::environment::x86::platform {
	class SkylakeConfig : public PlatformConfig {

		public:
			SkylakeConfig(llvm::StringMap<bool> *supportedFeatures, unsigned family, unsigned model, unsigned threads) :
				PlatformConfig("SKL_COREI", 6, {78,94}, {1,2}, family, model, threads, new payload::FMAPayload(supportedFeatures)) {};

			~SkylakeConfig() {};
	};
}

#endif