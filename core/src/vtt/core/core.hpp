#pragma once

#if defined(WIN32) && defined(VTTCORE_SHARED)
#ifdef VTTCORE_EXPORTS
#define VTTCOREAPI __declspec(dllexport)
#else
#define VTTCOREAPI __declspec(dllimport)
#endif
#else
#define VTTCOREAPI
#endif

namespace VTT::Core {
	/**
	 * @brief This value represents the version of the headers for this library which are being used.
	 */
	constexpr unsigned long long HeaderVersion = 0;

	VTTCOREAPI unsigned long long GetLibraryVersion();
}
