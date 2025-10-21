#pragma once

#include <string>
#include <boost/uuid.hpp>

#include "VttCore.hpp"

namespace VTT::Core {
	struct Point {
		float x, y;
	};

	struct Contour {
		std::vector<Point> points;
	};

	struct Universe {
		boost::uuids::uuid id;
		std::string title;
	};

	struct World {
		boost::uuids::uuid id;
		std::string name;
	};

	struct Region {
		boost::uuids::uuid id;
		std::string name;

		boost::uuids::uuid parent_world;

		/**
		 * @brief List of region border contours.
		 *
		 * Multiple contours can be used for fully disconnected region parts (a la russia with the tiny chunk between poland and lithuania).
		 */
		std::vector<Contour> borders;
	};

	struct Location {
		boost::uuids::uuid id;
		std::string name;

		/// Set this to zero to represent a separated location
		/// (i.e. not accessible from any world map, useful for some special locations like inside someone's mind).
		boost::uuids::uuid parent_region;

		/// set this to the uuid-equivalent of 0 to represent a sublocation
		boost::uuids::uuid parent_location;
	};
}
