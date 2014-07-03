/*
 * ElementGroup.h
 *
 *  Created on: 03/07/2014
 *      Author: Hernan
 */

#ifndef ELEMENTGROUP_H_
#define ELEMENTGROUP_H_

#include <iostream>
#include <stdlib.h>
#include <vector>

#include "Incidence.h"

using namespace std;

class ElementGroup {
public:
	ElementGroup(int count, string groupType);
	virtual ~ElementGroup();

	void addElement(Incidence);

	// Getters
	int getCount() const {
		return count;
	}

	const string& getGroupType() const {
		return groupType;
	}

	const vector<Incidence>& getElements() const {
		return elements;
	}

private:
	string groupType;
	int count;
	vector<Incidence> elements;
};

#endif /* ELEMENTGROUP_H_ */
