/*
 * Incidence.h
 *
 *  Created on: 03/07/2014
 *      Author: Hernan
 */

#ifndef INCIDENCE_H_
#define INCIDENCE_H_

#include <iostream>
#include <stdlib.h>

using namespace std;

class Incidence {
public:
	Incidence(int, int, int);
	virtual ~Incidence();

	int getA() const {
		return a;
	}

	int getB() const {
		return b;
	}

	int getC() const {
		return c;
	}

private:
	int a, b, c;
};

#endif /* INCIDENCE_H_ */
