#ifndef SIMPLIFY_FILTEROPTIONS_H
#define SIMPLIFY_FILTEROPTIONS_H

#include <vector>
#include <filesystem>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

namespace Simplify {
	class DLL_PUBLIC FilterOptions {
		public:
			typedef std::filesystem::path Path;
			typedef std::vector<Path> PathCollection;

			FilterOptions();

			PathCollection excludes;
			bool excludeMounts;
	};
}

#endif

