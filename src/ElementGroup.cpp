/*
 * ElementGroup.cpp
 *
 *  Created on: 03/07/2014
 *      Author: Hernan
 */

#include "ElementGroup.h"

ElementGroup::ElementGroup(int count, string groupType) {
	this->count = count;
	this->groupType = groupType;
}

void ElementGroup::addElement(Incidence element){
	elements.push_back(element);
}

ElementGroup::~ElementGroup() {
	// TODO Auto-generated destructor stub
}

