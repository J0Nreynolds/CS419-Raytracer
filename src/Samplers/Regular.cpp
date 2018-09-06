// 	Copyright (C) Jonathan Reynolds 2018
//	This C++ code is for non-commercial purposes only.
//	This C++ code is licensed under the GNU General Public License Version 2.


// This file contains the definition of the class Regular
#include <math.h>

#include "Regular.h"

Regular::Regular()
: Sampler(1, 1) {
}

Regular::Regular(int num_samples)
: Sampler(num_samples, 1) {
    generate_samples();
}

Regular::Regular(const Regular& u)
: Sampler(u) {
	generate_samples();
}

Regular& Regular::operator= (const Regular& rhs) {
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

Regular* Regular::clone() const {
	return (new Regular(*this));
}

Regular::~Regular() {}

void Regular::generate_samples(){
    	int n = (int) sqrt(num_samples);

    	for (int p = 0; p < num_sets; p++)
    		for (int j = 0; j < n; j++)
    			for (int k = 0; k < n; k++) {
    				Point2D sp((k + 0.5) / n, (j + 0.5) / n);
    				samples.push_back(sp);
    			}
}
