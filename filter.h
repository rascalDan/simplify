#ifndef SIMPLIFY_CORE_H
#define SIMPLIFY_CORE_H

#include "filterOptions.h"
#include <set>
#include <istream>
#include <ostream>

#define DLL_PUBLIC __attribute__ ((visibility ("default")))

namespace Simplify {
	class DLL_PUBLIC Filter {
		public:
			typedef std::filesystem::path Path;
			typedef std::set<Path> PathSet;

			void initiailize(FilterOptions &);
			void filterStream(std::istream & input, std::ostream & output) const;
			void find(const Path & root, std::ostream & output) const;
			void reset();

		protected:
			void addMountpoints();
			bool accept(const Path & input) const;
			bool pathPrefixesExclusion(const Path &) const;
			bool pathPrefixesAccepted(const Path &) const;

			static auto remove_trailing_separator(const Path & p)
			{
				if (!p.has_filename()) {
					return p.parent_path();
				}
				else {
					return p;
				}
			}

			PathSet exclude;
			mutable PathSet accepted;
	};
}

#endif

